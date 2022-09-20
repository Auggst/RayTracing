#ifndef AARECT_H
#define AARECT_H

#include "myutils.h"
#include "hittable.h"

class XY_rect : public Hittable {
    public:
        XY_rect() {}
        XY_rect(double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<Material> mat)
                : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};
        
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(double time0, double time1, AABB& output_box) const override {
            output_box = AABB(Point3(x0, y0, k - 0.0001), Point3(x1, y1, k + 0.0001));
            return true;
        }
    public:
        shared_ptr<Material> mp;
        double x0, x1, y0, y1, k;
};

class XZ_rect : public Hittable {
    public:
        XZ_rect() {}
        XZ_rect(double _x0, double _x1, double _z0, double _z1, double _k, shared_ptr<Material> mat)
                : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};
        
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(double time0, double time1, AABB& output_box) const override {
            output_box = AABB(Point3(x0, k - 0.0001, z0), Point3(x1, k + 0.0001, z1));
            return true;
        }
    public:
        shared_ptr<Material> mp;
        double x0, x1, z0, z1, k;
};

class YZ_rect : public Hittable {
    public:
        YZ_rect() {}
        YZ_rect(double _y0, double _y1, double _z0, double _z1, double _k, shared_ptr<Material> mat)
                : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};
        
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(double time0, double time1, AABB& output_box) const override {
            output_box = AABB(Point3(k - 0.0001, y0, z0), Point3(k + 0.0001, y1, z1));
            return true;
        }
    public:
        shared_ptr<Material> mp;
        double y0, y1, z0, z1, k;
};

#endif