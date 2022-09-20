#ifndef AABB_H
#define AABB_H

#include "myutils.h"
#include "vec3.h"
#include "ray.h"

class AABB {
    public:
        AABB(){}
        AABB(const Point3& a, const Point3& b) {minimum = a; maximum = b;}

        Point3 min() const {return minimum;}
        Point3 max() const {return maximum;}

        bool hit(const Ray& r, double t_min, double t_max) const;

        Point3 minimum;
        Point3 maximum;
};

AABB surrounding_box(AABB box0, AABB box1);

#endif