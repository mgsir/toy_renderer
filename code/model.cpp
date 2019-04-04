/*
加载模型

1. 加载 obj 文件
obj 文件里面有
    1. v 所有的顶点位置 
        格式：v -0.000581696 -0.734665 -0.623267
        用 verts_ 保存
         
    2. vt 顶点的贴图位置
        格式：vt  0.559 0.365 0.000
        用 uv_ 保存

    3. vn 顶点的法向量
        格式：vn  0.480 0.400 0.780
        用 norms_ 保存

    4. f 所有的三角形
        格式：f 293/276/293 68/45/68 67/44/67
                 v  vt  vn 
        用 faces_ 保存

2. 加载纹理
有三种纹理需要加载
    1. 漫反射纹理（普通贴图）
    2. 法线贴图
    3. 镜面反射贴图

*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "geometry.h"
#include "tgaimage.h"
#include "model.h"

Model::Model(const char* filepath) : verts_(), uv_(), norms_(), faces_(), diffusemap_(), normalmap_(), specularmap_() {
    std::ifstream in;
    in.open(filepath, std::ifstream::in);
    if (in.fail()) {
        std::cout<< "读取文件失败" << '\n';
        return;
    }
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;

        // 比较每一行前的字符
        if(!line.compare(0, 2, "v ")) {

            iss >> trash;
            Vec3f v;
            for(size_t i = 0; i < 3; i++) {
                iss >> v[i];
            }
            verts_.push_back(v);

        } else if(!line.compare(0, 3, "vt ")) {

            iss >> trash >> trash;

            Vec2f v;
            for(size_t i = 0; i < 2; i++) {
                iss >> v[i];
            }
            uv_.push_back(v);

        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;

            Vec3f v;
            for(size_t i = 0; i < 3; i++) {
                iss >> v[i];
            }
            norms_.push_back(v);

        } else if(!line.compare(0, 2, "f ")) {

            iss >> trash;
            Vec3i v;
            std::vector <Vec3i> f;
            while(iss >> v[0] >> trash >> v[1] >> trash >> v[2]) {
                // f 索引从 1 开始的
                v[0]--;
                v[1]--;
                v[2]--;
                f.push_back(v);
            }
            faces_.push_back(f);
        }

    }

    std::cout<< "加载模型成功，一共有 " << verts_.size() << " 个顶点" << '\n';
    load_texture(filepath, "_diffuse.tga", diffusemap_);
    load_texture(filepath, "_nm.tga", normalmap_);
    load_texture(filepath, "_spec.tga", specularmap_);
}

Model::~Model() {

}

void Model::load_texture(std::string filepath, const char *suffix, TGAImage &img) {
    size_t dot_idx = filepath.find_last_of(".");
    std::string file;
    if(dot_idx != std::string::npos) {
        file = filepath.substr(0, dot_idx) + std::string(suffix);
        std::cerr << "加载 " << file << (img.read_tga_file(file.c_str()) ? "成功" : "失败") << std::endl;
        img.flip_vertically();
    }
}

int Model::nverts() {
    return verts_.size();
}


int Model::nfaces() {
    return faces_.size();
}

Vec3f Model::vert(int nthface, int nthvert) {
    int vert_idx = faces_[nthface][nthvert][0];
    return verts_[vert_idx];
}

Vec2f Model::uv(int nthface, int nthvert) {
    int vert_idx = faces_[nthface][nthvert][1];
    return uv_[vert_idx];
}

TGAColor Model::diffuse(Vec2f uvf) {
    Vec2i uv(uvf[0]*diffusemap_.get_width(), uvf[1]*diffusemap_.get_height());
    return diffusemap_.get(uv[0], uv[1]);
}

Vec3f Model::normal(Vec2f uvf) {
    Vec2i uv(uvf[0]*normalmap_.get_width(), uvf[1]*normalmap_.get_height());
    TGAColor c = normalmap_.get(uv[0], uv[1]);
    Vec3f res;
    for (int i=0; i<3; i++)
        res[2-i] = (float)c[i]/255.f*2.f - 1.f;
    return res;
}

float Model::specular(Vec2f uvf) {
    Vec2i uv(uvf[0]*specularmap_.get_width(), uvf[1]*specularmap_.get_height());
    return specularmap_.get(uv[0], uv[1])[0]/1.f;
}