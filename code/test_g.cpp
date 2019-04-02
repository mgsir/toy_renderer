#include <iostream> 
#include "geometry.h"
#include <cassert>


void test_vec() {
    Vec2f x(1, 2);
    Vec2f y(0.1, 0.2);

    assert(std::abs(x * y - 0.5) <= 0.001);
    assert(std::abs((x + y).x - 1.1) <= 0.001);
    assert(std::abs((x - y).y - 1.8) <= 0.001);

    std::cout << "Vec2f 测试成功" << '\n';

    Vec2i x1(1, 2);
    Vec2i y1(2, 3);

    assert(x1 * y1 == 8);
    assert((x1 + y1).x == 3);
    assert((x1 - y1).y == -1);

    std::cout << "Vec2i 测试成功" << '\n';

    Vec3f x2(1, 2, 3);
    Vec3f y2(0.1, 0.2, 0.3);

    assert(std::abs(x2 * y2 - 1.4) <= 0.001);
    assert(std::abs((x2 + y2).x - 1.1) <= 0.001);
    assert(std::abs((x2 - y2).z - 2.7) <= 0.001);

    std::cout << "Vec3f 测试成功" << '\n';

    Vec3i x3(1, 2, 3);
    Vec3i y3(2, 3, 4);

    assert(x3 * y3 == 20);
    assert((x3 + y3).x == 3);
    assert((x3 - y3).z == -1);

    std::cout << "Vec3i 测试成功" << '\n';

    assert(std::abs(y2 * x3 - 1.4) <= 0.001);
    assert(std::abs((y2 + x3).x - 1.1) <= 0.001);
    assert(std::abs((y2 - x3).z + 2.7) <= 0.001);

    std::cout << "不同类型向量运算成功" << '\n';

    assert(std::abs((y2 / 2).x - 0.05) <= 0.001);
    assert(std::abs((y2 / 2).y - 0.1) <= 0.001);
    assert(std::abs((y2 / 2).z - 0.15) <= 0.001);

    std::cout << "除法运算成功" << '\n';
}

void test_mat() {
        Matrix m = Matrix::identity(); 
    assert(m[0][0] == m[1][1] == m[2][2] == m[3][3] == 1 && m[0][1] == 0 && m[0][2] == 0);
    std::cout<< "单位矩阵测试成功" <<'\n';

    mat<3, 3, float> m1;
    Vec3f vs[3];
    vs[0].x = 0, vs[0].y = 1, vs[0].z = 0;
    vs[1].x = 0, vs[1].y = 0, vs[1].z = 1;
    vs[2].x = 1, vs[2].y = 1, vs[2].z = 1;
    for(size_t i = 3; i--; ) {
        m1.set_col(i, vs[i]);
    }
    mat<3, 3, float> m1_invert = m1.invert();
    float d = m1_invert.det();
    assert(std::abs(d - 1) < 0.001);
    std::cout << "矩阵的逆测试成功" << '\n';

    mat<3, 3, float> m2;
    Vec3f vs1[3];
    vs1[0].x = 1, vs1[0].y = 2, vs1[0].z = 3;
    vs1[1].x = 4, vs1[1].y = 5, vs1[1].z = 6;
    vs1[2].x = 7, vs1[2].y = 8, vs1[2].z = 10;

    for(size_t i = 3; i--; ) {
        m2.set_col(i, vs1[i]);
    }
    float d1 = m2.det();
    assert(std::abs(d1 + 3) < 0.001);
    std::cout << "矩阵行列式测试成功" << '\n';
}

int main() {
    test_vec();
    test_mat();

    return 0;
}