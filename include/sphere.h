#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
#include "aabb.h"

AABB surrounding_box(AABB box0, AABB box1);

class Sphere : public Hittable {
    public:
        Sphere() {}
        Sphere(Point3 cen, double r, shared_ptr<Material> m) : center(cen), radius(r), mat_ptr(m) {};

        virtual bool hit(
            const Ray& r, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(
            double time0, double time1, AABB& output_box) const override;
    
    private:
        static void get_sphere_uv(const Point3& p, double& u, double& v) {
            //p为球面上一点
            //u，v为[0, 1]

            auto theta = acos(-p.y());
            auto phi = atan2(-p.z(), p.x()) + PI;

            u = phi / (2 * PI);
            v = theta / PI;
        }
    
    public:
        Point3 center;
        double radius;
        shared_ptr<Material> mat_ptr;
};

class Moving_Sphere : public Hittable {
    public:
        Moving_Sphere(){}
        Moving_Sphere(Point3 cen0, Point3 cen1, double t0, double t1, double r, shared_ptr<Material> m)
                    : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m){};
        
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(double t0, double t1, AABB& output_box) const override;

        Point3 center(double time) const;

    private:
        static void get_sphere_uv(const Point3& p, double& u, double& v) {
            //p为球面上一点
            //u，v为[0, 1]

            auto theta = acos(-p.y());
            auto phi = atan2(-p.z(), p.x()) + PI;

            u = phi / (2 * PI);
            v = theta / PI;
        }

    public:
        Point3 center0, center1;
        double time0, time1;
        double radius;
        shared_ptr<Material> mat_ptr;
};

#endif