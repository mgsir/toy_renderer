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
Vec3f eye(1, 1, 1);
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
        // std::cout << "P.z " << P.z << ' ' << depth << '\n';
        color = TGAColor(255, 255, 255) * P.z;
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
    // 着色 shadowbuffer
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


    delete model;
    delete zbuffer;
    delete shadowbuffer;
    return 0;
}


