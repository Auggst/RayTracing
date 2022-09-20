#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

#include "myutils.h"

using std::sqrt;

class Vec3 {
    //函数
    public:
        Vec3() : val{0, 0, 0} {}
        Vec3(double val0, double val1, double val2) : val{val0, val1, val2} {}

        inline double x() const { return val[0];}
        inline double y() const { return val[1];}
        inline double z() const { return val[2];}
        inline static Vec3 random() {
            return Vec3(Random(double(0.0), double(1.0)), Random(double(0.0), double(1.0)), Random(double(0.0), double(1.0)));
        }
        inline static Vec3 random(double min, double max) {
            return Vec3(Random(min, max), Random(min, max), Random(min, max));
        }

        //操作符重载
        Vec3 operator-() const { return Vec3(-val[0], -val[1], -val[2]);}
        double operator[](int i) const {return val[i];}
        double& operator[](int i) {return val[i];}

        //运算符重载
        Vec3& operator+=(const Vec3 &v) {
            this->val[0] += v.val[0];
            this->val[1] += v.val[1];
            this->val[2] += v.val[2];
            return *this;
        }

        Vec3& operator*=(const double t) {
            this->val[0] *= t;
            this->val[1] *= t;
            this->val[2] *= t;
            return *this;
        }

        Vec3& operator/=(const double t) {
            return *this *= 1/t;
        }

        double length() const {
            return sqrt(length_squared());
        }

        double length_squared() const {
            return val[0] * val[0] + val[1] * val[1] + val[2] * val[2];
        }

        bool near_zero() const {
            const auto s = 1e-8;
            return (fabs(val[0]) < s) && (fabs(val[1]) < s) && (fabs(val[2]) < s);
        }

    
    //数据
    public:
        double val[3];
};

using Point3 = Vec3;
using Color = Vec3;

//功能函数
inline std::ostream& operator<<(std::ostream &out, const Vec3 &v) {
    return out << v.val[0] << " " << v.val[1] << " " << v.val[2];
}

inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.val[0] + v.val[0], u.val[1] + v.val[1], u.val[2] + v.val[2]);
}

inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.val[0] - v.val[0], u.val[1] - v.val[1], u.val[2] - v.val[2]);
}

inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.val[0] * v.val[0], u.val[1] * v.val[1], u.val[2] * v.val[2]);
}

inline Vec3 operator*(double t, const Vec3 &v) {
    return Vec3(t * v.val[0], t * v.val[1], t * v.val[2]);
}

inline Vec3 operator*(const Vec3 &u, double t) {
    return t * u;
}

inline Vec3 operator/(Vec3 u, double t) {
    return (1 / t) * u;
}

inline double dot(const Vec3 &u, const Vec3 &v) {
    return u.val[0] * v.val[0]
        +  u.val[1] * v.val[1]
        +  u.val[2] * v.val[2];
}

inline Vec3 cross(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.val[1] * v.val[2] - u.val[2] * v.val[1],
                u.val[2] * v.val[0] - u.val[0] * v.val[2],
                u.val[0] * v.val[1] - u.val[1] * v.val[0]);
}

inline Vec3 unit_vector(Vec3 v) {
    return v / v.length();
}

inline Vec3 random_in_unit_disk() {
    while (true) {
        auto p = Vec3(Random(-1.0, 1.0), Random(-1.0, 1.0), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

inline Vec3 random_in_unit_sphere() {
    while (true) {
        auto p = Vec3::random(-1, 1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

inline Vec3 random_in_hemisphere(const Vec3& normal) {
    Vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0)  //在跟法线同一半球
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

inline Vec3 random_on_unit_sphere() {
    auto p = Vec3::random(-1, 1);
    return unit_vector(p);
}

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2 * dot(v, n) * n;
}

inline Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}
#endif