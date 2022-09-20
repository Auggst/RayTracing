#include "bvh.h"

BVH_Node::BVH_Node(const std::vector<shared_ptr<Hittable>>& src_objects, size_t start, size_t end, double t0, double t1){
    auto objects = src_objects;

    int axis = static_cast<int>(Random(0.0, 2.0));
    auto comparator = (axis == 0) ? box_x_compare
                    : (axis == 1) ? box_y_compare
                                  : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1) {
        left = right = objects[start];
    } else if (object_span == 2) {
        if (comparator(objects[start], objects[start + 1])) {
            left = objects[start];
            right = objects[start + 1];
        } else {
            left = objects[start + 1];
            right = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + object_span / 2;
        left = make_shared<BVH_Node>(objects, start, mid, t0, t1);
        right = make_shared<BVH_Node>(objects, mid, end, t0, t1);
    }

    AABB box_left, box_right;

    if (!left->bounding_box(t0, t1, box_left) || !right->bounding_box(t0, t1, box_right))
        std::cerr << "No bounding box in BVH_Node constructor.\n";
    
    box = surrounding_box(box_left, box_right);
}

bool BVH_Node::bounding_box(double t0, double t1, AABB& output_box) const {
    output_box = box;
    return true;
}

bool BVH_Node::hit(const Ray& r, double t0, double t1, hit_record& rec) const {
    if (!box.hit(r, t0, t1))    return false;
    bool hit_left = left->hit(r, t0, t1, rec);
    bool hit_right = right->hit(r, t0, hit_left ? rec.t : t1, rec);
    return hit_left || hit_right;
}

bool box_x_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
    return box_compare(a, b, 0);
}

bool box_y_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
    return box_compare(a, b, 1);
}

bool box_z_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
    return box_compare(a, b, 2);
}