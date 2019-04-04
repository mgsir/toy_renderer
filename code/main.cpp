/*
开始实现「渲染器」

1. 不实现贴图
    1. 初始化参数
        图片长度 800 800 255（深度）
        三个矩阵 ViewPort × Projection × ModelView
        眼睛
        光照方向（指向光）
        zbuffer 和 shadowbuffer

    2. 加载模型
        遍历所有三角形
        算出屏幕像素的位置，用顶点着色器判断
        画三角形
            用重心法判断这个点是不是在三角形内
            顶点着色器

    3. 着色器
        深度着色器
            要得到 shadowbuffer
        贴图着色器
            不实现贴图
            (255, 255, 255) * 强度
        

2. 实现贴图
*/

#include <iostream>
#include <limits>

#include "tgaimage.h"
#include "geometry.h"
#include "model.h"
#include "our_gl.h"

const int width = 800;
const int height = 800;

Model *model = NULL;
float *zbuffer = new float[width * height];
float *shadowbuffer = new float[width * height];

// 眼睛位置
Vec3f eye(-1, 2, 4);
// 光照方向
Vec3f light_dir(1, 1, 3);
// 中心
Vec3f center(0, 0, 0);
// upViewPort
Vec3f up(0, 1, 0);

struct DepthShader : public IShader {

    mat<3, 3, float> ver_tri;

    DepthShader() : ver_tri() {}
    virtual Vec4f vertex(int nthface, int nthver) {
        Vec4f ori_ver = embed<4>(model->vert(nthface, nthver));
        Vec4f scr_ver = Viewport * Projection * ModelView * ori_ver;
        ver_tri.set_col(nthver, model->vert(nthface, nthver));
        return scr_ver;
    }

    virtual bool fragment(Vec3f bar, TGAColor &color) {
        Vec3f P = ver_tri * bar;
        color = TGAColor(255, 255, 255) * P.z;
        return false;
    }
};


struct Shader : public IShader {
    Matrix uniform_MS;
    Matrix uniform_M;                   // ModelView
    Matrix uniform_MIT;                 // 转换法线 (ModelView).invert_transpose()
    mat<3, 3, float> ver_tri;           // 顶点三角形坐标
    mat<2, 3, float> ver_uv;            // 顶点 uv

    //

    Shader(Matrix M, Matrix MIT, Matrix MS) : uniform_M(M), uniform_MIT(MIT), uniform_MS(MS), ver_tri(), ver_uv() {}
    virtual Vec4f vertex(int nthface, int nthver) {
        Vec4f ori_ver = embed<4>(model->vert(nthface, nthver));
        Vec4f scr_ver = Viewport * Projection * ModelView * ori_ver;
        ver_tri.set_col(nthver, model->vert(nthface, nthver));

        // 加载顶点 uv 值
        ver_uv.set_col(nthver, model->uv(nthface, nthver));
        return scr_ver;
    }

    virtual bool fragment(Vec3f bar, TGAColor &color) {
        // 由于 Projection 是 0 所以没有投影也就没有必要除以最后一个数字了
        Vec3f shadow_coor = cut<3>(uniform_MS * embed<4>(bar)); 
        int idx = shadow_coor[0] + shadow_coor[1];

        float shadow = .3 + .7 * (shadowbuffer[idx] < shadow_coor[2]);
        Vec3f P = ver_tri * bar;
        Vec2f uvP = ver_uv * bar;
        // 得到法线
        Vec3f nmV = model->normal(uvP);
        // 法线转换
        Vec3f n = cut<3>(uniform_MIT * embed<4>(nmV)).normalize();
        // 改变光线
        Vec3f l = cut<3>(uniform_M * embed<4>(light_dir)).normalize();
        // // 光照强度
        // float intensity = n * l;
        // TGAColor diff = model->diffuse(uvP);
        // for(size_t i = 3; i--; ) {
        //     color[i] = diff[i] * intensity;
        // }

        // Phong 着色
        // 贴图颜色
        TGAColor c = model->diffuse(uvP);
        // 反射光
        Vec3f r = (n * (n * l * 2.f) - l).normalize(); 
        // 镜面反射
        float spec = pow(std::max(r.z, 0.0f), model->specular(uvP));
        // 漫反射
        float diff = std::max(0.f, n * l);
        // 最终 Phong 着色
        for(size_t i = 3; i--;) {
            color[i] = std::min<float>(20 + c[i] * shadow * (1.2 * spec + 0.8 * diff), 255);
        }

        return false;
    }
};


int main() {

    model = new Model("../obj/african_head/african_head.obj");

    // 初始化 zbuffer 以及 shdaowbuffer
    for(size_t i = 0; i < width*height; i++) {
        zbuffer[i] = shadowbuffer[i] = -std::numeric_limits<float>::max();
    }
    light_dir.normalize();
    // shadowbuffer
    {
        TGAImage depthimg(width, height, TGAImage::RGB);

        DepthShader depthshader;

        Vec4f scrren[3];
        lookat(light_dir, center, up);
        viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
        projection(0);

        for(size_t i = 0; i < model->nfaces(); i++) {
            for(size_t j = 0; j < 3; j++) {
                scrren[j] = depthshader.vertex(i, j);
            }

            triangle(scrren, depthshader, depthimg, shadowbuffer);
        }
        depthimg.flip_vertically();
        depthimg.write_tga_file("depth.tga");
    }

        Matrix shadow_m = Viewport * Projection * ModelView;

    {
        TGAImage img(width, height, TGAImage::RGB);
        Vec4f scrren[3];
        lookat(eye, center, up);
        viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
        projection(-1.f / (eye - center).norm());

        Matrix M = ModelView;
        // 由于我存的是原始值, 所以乘以 shadow_m 就可以到 shadowbuffer 了
        // 再乘以 shadow_m
        Matrix MIT = (ModelView).invert_transpose();

        Shader shader(M, MIT, shadow_m);

        for(size_t i = 0; i < model->nfaces(); i++) {
            for(size_t j = 0; j < 3; j++) {
                scrren[j] = shader.vertex(i, j);
            }

            triangle(scrren, shader, img, zbuffer);
        }
        img.flip_vertically();
        img.write_tga_file("final.tga");
    }


    delete model;
    delete [] zbuffer;
    delete [] shadowbuffer;
    return 0;
}


