#ifndef BVH_H
#define BVH_H

#include <algorithm>

#include "myutils.h"
#include "hittable.h"
#include "scene.h"

AABB surrounding_box(AABB box0, AABB box1);

class BVH_Node : public Hittable {
    public:
        BVH_Node();
        BVH_Node(const std::vector<shared_ptr<Hittable>>& src_objects, size_t start, size_t end, double t0, double t1);
        BVH_Node(const Scene& scene, double t0, double t1){BVH_Node(scene.objects, 0, scene.objects.size(), t0, t1);}


        virtual bool hit(const Ray& r, double t0, double t1, hit_record& rec) const override;
        virtual bool bounding_box(double t0, double t1, AABB& output_box) const override;
    
    public:
        shared_ptr<Hittable> left;
        shared_ptr<Hittable> right;
        AABB box;
};

inline bool box_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axis) {
    AABB box_a;
    AABB box_b;

    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
        std::cerr << "No bounding box in BVH_Node constructor.\n";
    
    return box_a.min().val[axis] < box_b.min().val[axis];
}

bool box_x_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b);
bool box_y_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b);
bool box_z_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b);
#endif