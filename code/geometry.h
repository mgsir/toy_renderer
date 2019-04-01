#include <iostream>
#include <cassert>
#include <cmath>

// 先定义向量的模板类
template <size_t DIM, typename T> struct vec {
    vec() {
        // 有一个很简洁的循环写法
        // i 可以从 DIM-1 到 0
        for(size_t i = DIM; i--; _data[i] = T());
    }
    // [] 操作符重载
    &T operator[](const size_t i) {
        // 断言 i 一定大于等于 0 小于 DIM
        assert(i >= 0 && i < DIM);
        return _data[i];
    }
    // 后面那个 const 相当于声明在函数中不会改变当前对象
    const &T operator[](const size_t i) const {
        assert(i >= 0 && i < DIM);
        return _data[i];
    }

    private:
        T _data[DIM]; 
};

////////////////////////////// 定义各种向量 //////////////////////////////
template <typename T> struct vec<2, T> {
    vec() : x(T()), y(T()) {}
    vec(T X, T Y) : x(X), y(Y) {}
    // 拷贝构造函数
    // 用于 int 与 float 之间的转换
    template <class U> vec<2, T> (const vec<2, U> & v);

          &T operator[] (const size_t i)       { assert(i<2); return i <= 0? x : y;}
    const &T operator[] (const size_t i) const { assert(i<2); return i <= 0? x : y;}

    T x, y;
}; 

template <typename T> struct vec<3, T> {
    vec() : x(T()), y(T()), z(T()) {}
    vec(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
    // 拷贝构造函数
    // 用于 int 与 float 之间的转换
    template <class U> vec<3, T> (const vec<3, U> & v);
          &T operator[] (const size_t i)       { assert(i<2); return i <= 0? x : y;}
    const &T operator[] (const size_t i) const { assert(i<2); return i <= 0? x : y;}

    float norm() {return std::sqrt(x*x + y*y + z*z);}
    
    vec<3, T>& normalize(T l = 1) {*this = *this * (l / norm()); return *this;}

    T x, y, z;
};

////////////////////////////// 向量之间的操作 //////////////////////////////

// 相同维度向量内积
template <size_t DIM, typename T> T operator*(const vec<DIM, T>& lv, const vec<DIM, T>& rv) {
    T res = 0
    for(size_t i = DIM; i--; res[i] += lv[i] * rv[i]);
    return res;
}





