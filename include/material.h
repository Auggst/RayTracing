#ifndef MATERIAL_H
#define MATERIAL_H

#include "myutils.h"
#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "texture.h"

struct hit_record;

class Material {
    public:
        virtual Color emitted(double u, double v, Point3& p) const {return Color(0, 0, 0);}
        virtual bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
    public:
        Lambertian(const Color& a) : albedo(make_shared<Solid_color>(a)) {}
        Lambertian(shared_ptr<Texture> a) : albedo(a){}

        // virtual Color emitted(double u, double v, Point3& p) const {return albedo->value(u, v, p);}
        virtual bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override;
    public:
        shared_ptr<Texture> albedo;
};

class Metal : public Material {
    public:
        Metal(const Color& a, double f) : albedo(a), fuzz(clamp(f, 0.0, 1.0)) {}

        // virtual Color emitted(double u, double v, Point3& p) const {return albedo;}
        virtual bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override;
    public:
        Color albedo;
        double fuzz;
};

class Dielectric : public Material {
    public:
        Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override;
    private:
        static double reflectance(double cosine, double ref_idx);
    public:
        double ir;  //折射系数
};

class Diffuse_light : public Material {
    public:
        Diffuse_light(shared_ptr<Texture> a) : emit(a){}
        Diffuse_light(Color c) : emit(make_shared<Solid_color>(c)){}

        virtual bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override {return false;}
        virtual Color emitted(double u, double v, const Point3& p) const  {return emit->value(u, v, p);}
    public:
        shared_ptr<Texture> emit;
};

class Isotropic : public Material {
    public:
        Isotropic(Color c) : albedo(make_shared<Solid_color>(c)) {}
        Isotropic(shared_ptr<Texture> a) : albedo(a) {}

        virtual bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override {
            scattered = Ray(rec.p, random_in_unit_sphere(), r_in.time());
            attenuation = albedo->value(rec.u, rec.v, rec.p);
            return true;
        }
    public:
        shared_ptr<Texture> albedo;
};

#endif