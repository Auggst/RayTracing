#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>

#include "hittable.h"
#include "aabb.h"

AABB surrounding_box(AABB box0, AABB box1);

using std::shared_ptr;
using std::make_shared;

class Scene {
    public:
        Scene() {}
        Scene(shared_ptr<Hittable> object) { addObj(object);}

        void clearObj() {objects.clear();}
        void addObj(shared_ptr<Hittable> object) {objects.push_back(object);}

        bool hit(
            const Ray&r, double t_min, double t_max, hit_record& rec) const;
        bool bounding_box(
            double t0, double t1, AABB& output_box) const;
    public:
        std::vector<shared_ptr<Hittable>> objects;
        AABB sceaabb;
        bool havingaabb = false;
};

#endif