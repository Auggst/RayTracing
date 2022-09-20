#ifndef MYUTILS_H
#define MYUTILS_H

#include <random>
#include <cmath>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//常量
const double infinity = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

//功能函数
//---------------------------------------------
//角度转弧度
inline double degrees_to_radians(double degrees) {
    return degrees * PI / 180.0;
}

//clamp函数
template <typename T> T clamp(T x, T xmin, T xmax) {
    if (x < xmin) return xmin;
    if (x > xmax) return xmax;
    return x;
}

//线性插值
template <typename T> T blend(T left, T right, double t) {
    return (1.0 - t) * left + t * right;
}

//随机生成数
template <typename T>
T Random(T min, T max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<T> dis(min, max);

    return dis(gen);
}

inline int RandomInt(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);

    return dis(gen);
}

//随机生成数组
template <typename T>
void Random(T *buf, uint32_t n, T min, T max, uint32_t seed) {
    std::random_device rd;
    std::mt19937 gen(seed);
    std::uniform_int_distribution<T> dis(min, max);

    for (auto i = 0U; i < n; i++) {
        if (seed == 0) buf[i] = 0;
        else buf[i] = dis(gen);
    }
}
#endif