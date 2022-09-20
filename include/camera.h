#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "ray.h"

class Camera {
    public:
        Camera(Point3 lookfrom, Point3 lookat, Vec3 vup, double vfov, double aspect_ratio, double aperture, double focus_dist, double time0 = 0, double time1 = 0);
        Camera();

        Ray get_ray(double u, double v) const;
    private:
        Point3 _origin;
        Point3 _lower_left_corner;
        Vec3 _horizontal;
        Vec3 _vertical;
        Vec3 _u, _v, _w;
        double _lens_radius;
        double _time0, _time1;
};

#endif