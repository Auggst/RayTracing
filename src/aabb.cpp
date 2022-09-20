#include "aabb.h"

bool AABB::hit(const Ray& r, double t_min, double t_max) const {
    for (int i = 0; i < 3; i++) {
        auto t0 = fmin((minimum[i] - r.origin()[i]) / r.direction()[i],
                       (maximum[i] - r.origin()[i]) / r.direction()[i]);
        auto t1 = fmax((maximum[i] - r.origin()[i]) / r.direction()[i],
                       (maximum[i] - r.origin()[i]) / r.direction()[i]);
        t_min = fmax(t0, t_min);
        t_max = fmin(t1, t_max);
        if (t_max <= t_min) return false;
    }
    return true;
}

AABB surrounding_box(AABB box0, AABB box1) {
    Point3 small(fmin(box0.min().x(), box1.min().x()),
                 fmin(box0.min().y(), box1.min().y()),
                 fmin(box0.min().z(), box1.min().z()));
    Point3 big(fmax(box0.max().x(), box1.max().x()),
               fmax(box0.max().y(), box1.max().y()),
               fmax(box0.max().z(), box1.max().z()));
    return AABB(small, big);
}