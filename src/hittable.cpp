#include "hittable.h"

bool Translate::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
    Ray moved_r(r.origin() - offset, r.direction(), r.time());
    if (!ptr->hit(moved_r, t_min, t_max, rec)) return false;
    rec.p += offset;
    rec.set_face_normal(moved_r, rec.normal);
    return true;
}

bool Translate::bounding_box(double t0, double t1, AABB& output_box) const {
    if (!ptr->bounding_box(t0, t1, output_box)) return false;

    output_box = AABB(output_box.min() + offset,    output_box.max() + offset);

    return true;
}

Rotate::Rotate(shared_ptr<Hittable> p, double angle, int _axis): ptr(p) {
    auto radians = degrees_to_radians(angle);
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = ptr->bounding_box(0, 1, bbox);
    axis = _axis;

    Point3 min(infinity, infinity, infinity);
    Point3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                auto x = i * bbox.max().x() + (1 - i) * bbox.min().x();
                auto y = j * bbox.max().y() + (1 - j) * bbox.min().y();
                auto z = k * bbox.max().z() + (1 - k) * bbox.min().z();

                Vec3 test(x, y, z);
                if (axis == 1) {
                    test.val[0] = cos_theta * x + sin_theta * z;
                    test.val[2] = -sin_theta * x + cos_theta * z;
                } else if (axis == 0) {
                    test.val[1] = cos_theta * y - sin_theta * z;
                    test.val[2] = sin_theta * y + cos_theta * z;
                } else {
                    test.val[0] = cos_theta * x - sin_theta * y;
                    test.val[1] = sin_theta * x + cos_theta * y;
                }
                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], test[c]);
                    max[c] = fmax(max[c], test[c]);
                }
            }
        }
    }

    bbox = AABB(min, max);
}

bool Rotate::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
    auto origin = r.origin();
    auto direction = r.direction();

    //TODO::绕y轴旋转是否添加负号
    if (axis == 1) {
        origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
        origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];
        direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
        direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];
    } else if (axis == 0) {
        origin[1] = cos_theta * r.origin()[1] - sin_theta * r.origin()[2];
        origin[2] = sin_theta * r.origin()[1] + cos_theta * r.origin()[2];
        direction[1] = cos_theta * r.direction()[1] - sin_theta * r.direction()[2];
        direction[2] = sin_theta * r.direction()[1] + cos_theta * r.direction()[2];
    } else {
        origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[1];
        origin[1] = sin_theta * r.origin()[0] + cos_theta * r.origin()[1];
        direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[1];
        direction[1] = sin_theta * r.direction()[0] + cos_theta * r.direction()[1];
    }

    Ray rotated(origin, direction, r.time());

    if (!ptr->hit(rotated, t_min, t_max, rec))  return false;

    auto p = rec.p;
    auto normal = rec.normal;

    if (axis == 1) {
        p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
        p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];
        normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
        normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];
    } else if (axis == 0) {
        p[1] = cos_theta * rec.p[1] - sin_theta * rec.p[2];
        p[2] = sin_theta * rec.p[1] + cos_theta * rec.p[2];
        normal[1] = cos_theta * rec.normal[1] - sin_theta * rec.normal[2];
        normal[2] = sin_theta * rec.normal[1] + cos_theta * rec.normal[2];
    } else {
        p[0] = cos_theta * rec.p[0] - sin_theta * rec.p[1];
        p[1] = sin_theta * rec.p[0] + cos_theta * rec.p[1];
        normal[0] = cos_theta * rec.normal[0] - sin_theta * rec.normal[1];
        normal[1] = sin_theta * rec.normal[0] + cos_theta * rec.normal[1];
    }

    rec.p = p;
    rec.set_face_normal(rotated, normal);

    return true;
}
