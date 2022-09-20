#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "myutils.h"

#include "hittable.h"
#include "material.h"
#include "texture.h"

class Constant_medium : public Hittable {
    public:
        Constant_medium(shared_ptr<Hittable> b, double d, shared_ptr<Texture> a)
                        : boundary(b), neg_inv_density(-1 / d), phase_function(make_shared<Isotropic>(a)) {}
        Constant_medium(shared_ptr<Hittable> b, double d, Color c)
                        : boundary(b), neg_inv_density(-1 / d), phase_function(make_shared<Isotropic>(c)) {}
        
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(double t0, double t1, AABB& output_box) const override {
            return boundary->bounding_box(t0, t1, output_box);
        }
    public:
        shared_ptr<Hittable> boundary;
        shared_ptr<Material> phase_function;
        double neg_inv_density;
};

#endif