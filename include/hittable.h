#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "myutils.h"
#include "aabb.h"

class Material;

struct hit_record {
    Point3 p;
    Vec3 normal;
    shared_ptr<Material> mat_ptr;
    double t;
    double u;
    double v;
    bool front_face;

    inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

//提供一个Hittable接口
class Hittable {
    public:
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;
        virtual bool bounding_box(double time0, double time1, AABB& output_box) const = 0;
};


class Translate : public Hittable {
    public: 
        Translate(shared_ptr<Hittable> p, const Vec3& displacement) : ptr(p), offset(displacement) {}
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(double t0, double t1, AABB& output_box) const override;
    public:
        shared_ptr<Hittable> ptr;
        Vec3 offset;
};

class Rotate : public Hittable {
    public:
        Rotate(shared_ptr<Hittable> p, double angle, int _axis = 1);
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(double t0, double t1, AABB& output_box) const override {
            output_box = bbox;
            return hasbox;
        }
    public:
        shared_ptr<Hittable> ptr;
        double sin_theta;
        double cos_theta;
        bool hasbox;
        AABB bbox;
        int axis;
};

#endif