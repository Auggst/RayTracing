#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
    public:
        Ray() {}
        Ray(const Point3& origin, const Vec3& direction, double time = 0.0)
            :ori(origin), dir(direction), tm(time)
        {}

        Point3 origin() const {return ori;}
        Vec3 direction() const {return dir;}
        double time() const {return tm;}

        Point3 at(double t) const {
            return ori + t * dir;
        }
    
    public:
        Point3 ori;
        Vec3 dir;
        double tm;
};
#endif