#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

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
    T& operator[](const size_t i) {
        // 断言 i 一定大于等于 0 小于 DIM
        assert(i >= 0 && i < DIM);
        return _data[i];
    }
    // 后面那个 const 相当于声明在函数中不会改变当前对象
    const T& operator[](const size_t i) const {
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

          T& operator[] (const size_t i)       { assert(i<2); return i <= 0? x : y;}
    const T& operator[] (const size_t i) const { assert(i<2); return i <= 0? x : y;}

    T x, y;
}; 

template <typename T> struct vec<3, T> {
    vec() : x(T()), y(T()), z(T()) {}
    vec(T X, T Y, T Z) : x(X), y(Y), z(Z) {

    }
    // 拷贝构造函数
    // 用于 int 与 float 之间的转换
    template <class U> vec<3, T> (const vec<3, U> & v);
          T& operator[] (const size_t i)       { assert(i<3); return i <= 0? x : i == 1 ? y : z;}
    const T& operator[] (const size_t i) const { assert(i<3); return i <= 0? x : i == 1 ? y : z;}

    float norm() {return std::sqrt(x*x + y*y + z*z);}
    
    vec<3, T>& normalize(T l = 1) {*this = *this * (l / norm()); return *this;}

    T x, y, z;
};

////////////////////////////// 向量之间的运算 //////////////////////////////
// 相同维度向量内积
// 调用形参的时候会调用拷贝构造
template <size_t DIM, typename T> T operator*(const vec<DIM, T>& lv, const vec<DIM, T>& rv) {
    T res = 0;
    for(size_t i = DIM; i--;) {
        res += lv[i] * rv[i];
    }
    return res;
}

template<size_t DIM,typename T,typename U> vec<DIM,T> operator*(vec<DIM,T> lv, const U& n) {
    for (size_t i=DIM; i--; lv[i]*=n);
    return lv;
}


// 加法
template <size_t DIM, typename T> vec<DIM, T> operator+(vec<DIM, T>lv, const vec<DIM, T>& rv) {
    for(size_t i = DIM; i--; lv[i] += rv[i]);
    return lv;
}

// 减法
template <size_t DIM, typename T> vec<DIM, T> operator-(vec<DIM, T>lv, const vec<DIM, T>& rv) {
    for(size_t i = DIM; i--; lv[i] -= rv[i]);
    return lv;
}

// 不同 typename 之间的向量运算

// 内积
template <size_t DIM, typename T, typename U> T operator*(const vec<DIM, T>& lv, const vec<DIM, U>& rv) {
    T res = 0;
    for(size_t i = DIM; i--; res += lv[i] * rv[i]);
    return res;
}

// 加法
template <size_t DIM, typename T, typename U> vec<DIM, T> operator+(vec<DIM, T>lv, const vec<DIM, U>& rv) {
    for(size_t i = DIM; i--; lv[i] += rv[i]);
    return lv;
}

// 减法
template <size_t DIM, typename T, typename U> vec<DIM, T> operator-(vec<DIM, T>lv, const vec<DIM, U>& rv) {
    for(size_t i = DIM; i--; lv[i] -= rv[i]);
    return lv;
}

// 除法除以一个常数
template <size_t DIM, typename T, typename U> vec<DIM, T> operator/(vec<DIM, T>lv, const U& n) {
    for(size_t i = DIM; i--; lv[i] /= n);
    return lv;
}

// 变换成齐次坐标
template <size_t LEN, size_t DIM, typename T> vec<LEN, T> embed(const vec<DIM, T>& v, T fill = 1) {
    vec<LEN, T> ret;
    for(size_t i = LEN; i--; ret[i] = (i < DIM ? v[i] : fill) );
    return ret;
}

// 这里的 LEN 比 DIM 要小
template <size_t LEN, size_t DIM, typename T> vec<LEN, T> cut(const vec<DIM, T> &v) {
    vec<LEN, T> ret;
    for(size_t i = LEN; i--; ret[i] = v[i]);
    return ret;
}


// 向量叉乘
template <typename T> vec<3,T> cross(vec<3,T> v1, vec<3,T> v2) {
    return vec<3,T>(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}


////////////////////////////// 行列式 //////////////////////////////
// 声明矩阵
template <size_t Rows, size_t Cols, typename T> class mat;


// 用余因子矩阵求行列式
// 递归去做
template <size_t DIM, typename T> struct dt {
    static T det(mat<DIM, DIM, T>& m) {
        T ret = 0;
        for (size_t i = DIM; i--; ret += m[0][i] * m.cofactor(0, i));
        return ret;
    } 

};

// 函数重载
template <typename T> struct dt<1, T> {
    static T det(mat<1, 1, T>& m) {
        return m[0][0];
    } 

};
////////////////////////////// 矩阵 //////////////////////////////
template <size_t Rows, size_t Cols, typename T> class mat {
    vec<Cols, T> rows[Rows];

    public:
        mat() {}
        vec<Cols, T>& operator[] (const size_t idx) {
            assert(idx < Rows);
            return rows[idx];
        }

        const vec<Cols, T>& operator[] (const size_t idx) const {
            assert(idx < Rows);
            return rows[idx];
        }

        // 得到某一列
        vec<Rows, T> col(const size_t idx) const {
            assert(idx < Cols);
            vec<Rows, T> ret;
            for(size_t i = Rows; i--; ret[i] = rows[i][idx]);
            return ret; 
        }

        void set_col(size_t idx, const vec<Rows, T> &v) {
            assert(idx < Cols);
            for(size_t i = Rows; i--; rows[i][idx] = v[i]);
        }
        // 单位矩阵
        static mat<Rows, Cols, T> identity() {
            mat<Rows, Cols, T> ret;
            for(size_t i = Rows; i--;) {
                for(size_t j = Cols; j--; ret[i][j] = (i==j));
            }

            return ret;
        }
        // 逆矩阵
        // 逆矩阵在 shader 中用到
        // 用伴随矩阵法求解逆矩阵
        // 这个矩阵的逆 = (余因子矩阵 * (1/该矩阵的行列式)).T

        // 矩阵的行列式用 余因子矩阵计算
        mat<Cols,Rows,T> invert() {
            return invert_transpose().transpose();
        }

        mat<Rows, Cols, T> invert_transpose() {
            // 先求余因子矩阵
            mat<Rows, Cols, T> cm = cofactor_matrix();
            // 计算当前矩阵行列式
            T d =  (*this).det();
            return cm / d;
        }

        // 剪切矩阵用来计算行列式
        mat<Rows-1, Cols-1, T> get_copy(size_t r, size_t c) {
            mat<Rows-1, Cols-1, T> ret;
            for (size_t i=Rows-1; i--; )
                for (size_t j=Cols-1;j--; ret[i][j]=rows[i<r?i:i+1][j<c?j:j+1]);
            return ret;
        }

        T det() {
            return dt<Cols, T>::det(*this);
        }

        T cofactor(size_t r, size_t c) {
            return get_copy(r, c).det() * ((r+c) % 2 == 0 ? -1 : 1);
        }

        // 余因子矩阵
        mat<Rows, Cols, T> cofactor_matrix() {
            mat<Rows, Cols, T> ret;
            for(size_t i = Rows; i--; ) {
                for(size_t j = Cols; j--; ret[i][j] = cofactor(i, j));
            }

            return ret;
        }


        // 转置
        mat<Cols,Rows,T> transpose() {
            mat<Cols,Rows,T> ret;
            for (size_t i=Rows; i--; ret[i]=this->col(i));
            return ret;
        }
};

////////////////////////////// 矩阵运算 //////////////////////////////

template<size_t Rows,size_t Cols,typename T> vec<Rows,T> operator*(const mat<Rows,Cols,T>& lhs, const vec<Cols,T>& rhs) {
    vec<Rows,T> ret;
    for (size_t i=Rows; i--; ret[i]=lhs[i]*rhs);
    return ret;
}

template<size_t R1,size_t C1,size_t C2,typename T>mat<R1,C2,T> operator*(const mat<R1,C1,T>& lhs, const mat<C1,C2,T>& rhs) {
    mat<R1,C2,T> result;
    for (size_t i=R1; i--; )
        for (size_t j=C2; j--; ) {
            
            vec<C1, T> t = lhs[i];
            vec<C1, T> t2 = rhs.col(j);

            result[i][j] = t * t2;
        }
    return result;
}

template<size_t Rows,size_t Cols,typename T>mat<Cols,Rows,T> operator/(mat<Rows,Cols,T> lhs, const T& rhs) {
    for (size_t i=Rows; i--; lhs[i]=lhs[i]/rhs);
    return lhs;
}



typedef vec<2, int> Vec2i;
typedef vec<2, float> Vec2f;
typedef vec<3, int> Vec3i;
typedef vec<3, float> Vec3f;
typedef vec<4, float> Vec4f;
typedef mat<4, 4, float> Matrix;

#endif