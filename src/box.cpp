#include "box.h"

Box::Box(const Point3& p0, const Point3& p1, shared_ptr<Material> ptr) {
    box_min = p0;
    box_max = p1;

    sides.push_back(make_shared<XY_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
    sides.push_back(make_shared<XY_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));

    sides.push_back(make_shared<XZ_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
    sides.push_back(make_shared<XZ_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));

    sides.push_back(make_shared<YZ_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
    sides.push_back(make_shared<YZ_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
}

bool Box::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;
    for (const auto& object : sides) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}