// #ifndef __OUR_GL_H__
// #define __OUR_GL_H__

// #include "tgaimage.h"
// #include "geometry.h"

// // 三个矩阵
// extern Matrix ModelView;
// extern Matrix Projection;
// extern Matrix ViewPort;

// const float depth = 2000.f;

// //
// void viewport(int x, int y, int w, int h);
// void projection(float c);
// void lookat(Vec3f eye, Vec3f center, Vec3f up);


// // 着色器
// // 顶点着色器为
// struct IShader {
//     virtual ~IShader();
//     // 纯虚函数，子类一定要定义这个函数
//     // 顶点着色器，返回 Vec4f
//     virtual Vec4f vertex(int nthface, int nthver) = 0;

//     // 顶点着色器 记录 顶点的 uv 
//     // 通过重心法算出比例
//     // 用这个比例计算片段的 uv 值
//     virtual bool fragment(Vec3f bar, TGAColor& color) = 0;
// };

// void triangle(Vec4f *pts, IShader &shader, TGAImage &img, float *buffer);
// #endif

#ifndef __OUR_GL_H__
#define __OUR_GL_H__
#include "tgaimage.h"
#include "geometry.h"

extern Matrix ModelView;
extern Matrix Viewport;
extern Matrix Projection;
const float depth = 2000.f;

void viewport(int x, int y, int w, int h);
void projection(float coeff = 0.f); // coeff = -1/c
void lookat(Vec3f eye, Vec3f center, Vec3f up);

struct IShader
{
    virtual ~IShader();
    virtual Vec4f vertex(int iface, int nthvert) = 0;
    virtual bool fragment(Vec3f bar, TGAColor &color) = 0;
};

void triangle(Vec4f *pts, IShader &shader, TGAImage &image, float *zbuffer);
#endif //__OUR_GL_H__