#include <iostream>
#include <limits>
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"
#include "our_gl.h"

Matrix ModelView;
Matrix Projection;
Matrix Viewport;

IShader::~IShader() {}


// void viewport(int x, int y, int w, int h) {
//     Viewport = Matrix::identity();
//     Viewport[0][0] = w;
//     Viewport[1][1] = h;
//     Viewport[2][2] = depth/2;
//     Viewport[3][3] = 1;

//     Viewport[0][3] = x;
//     Viewport[1][3] = y;
//     Viewport[2][3] = depth/2;
// }

void viewport(int x, int y, int w, int h) {
    Viewport = Matrix::identity();
    Viewport[0][3] = x+w/2.f;
    Viewport[1][3] = y+h/2.f;
    Viewport[2][3] = depth/2.f;
    Viewport[0][0] = w/2.f;
    Viewport[1][1] = h/2.f;
    Viewport[2][2] = depth/2.f;
}

void projection(float c) {
    Projection = Matrix::identity();
    Projection[2][3] = c;
}

// 通过右手法则判断方向
void lookat(Vec3f eye, Vec3f center, Vec3f up) {
    Vec3f z = (eye-center).normalize();
    Vec3f x = cross(up, z).normalize();
    Vec3f y = cross(z, x).normalize();

    ModelView = Matrix::identity();
    for(size_t i = 0; i < 3; i++) {
        ModelView[0][i] = x[i];
        ModelView[1][i] = y[i];
        ModelView[2][i] = z[i];

        ModelView[i][3] = -center[i];
    }
}

Vec3f barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P) {
    Vec3f s[2];
    for (int i=2; i--; ) {
        s[i][0] = C[i]-A[i];
        s[i][1] = B[i]-A[i];
        s[i][2] = A[i]-P[i];
    }
    Vec3f u = cross(s[0], s[1]);
    if (std::abs(u[2])>1e-2)
        return Vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
    return Vec3f(-1,1,1); 
}


void triangle(Vec4f *pts, IShader &shader, TGAImage& img, float *buffer) {
    // 找到这三个点的最小矩形
    Vec2f rec_min( std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());
    Vec2f rec_max(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());

    // rec_min 一定是所有三角形中最小的 x 最小的 y
    for(size_t i = 0; i < 3; i++) {
        for(size_t j = 0; j < 2; j++) {
            rec_min[j] = std::min(rec_min[j], pts[i][j]/pts[i][3]);
            rec_max[j] = std::max(rec_max[j], pts[i][j]/pts[i][3]);
        }
    }

    TGAColor color;
    Vec2i P;

    for(P.x = rec_min.x; P.x <= rec_max.x; P.x++) {
        for(P.y = rec_min.y; P.y <= rec_max.y; P.y++) {
            Vec3f c = barycentric(cut<2>(pts[0]/pts[0][3]), cut<2>(pts[1]/pts[1][3]), cut<2>(pts[2]/pts[2][3]), cut<2>(P));
            float z = pts[0][2]*c.x + pts[1][2]*c.y + pts[2][2]*c.z;
            float w = pts[0][3]*c.x + pts[1][3]*c.y + pts[2][3]*c.z;
            int frag_depth = z/w;
            if (c.x<0 || c.y<0 || c.z<0 || buffer[P.x+P.y*img.get_width()]>frag_depth) continue;
            bool discard = shader.fragment(c, color);
            if (!discard) {
                buffer[P.x+P.y*img.get_width()] = frag_depth;
                img.set(P.x, P.y, color);
            }
        }
    }
}