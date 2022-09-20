#ifndef TEXTURE_H
#define TEXTURE_H

#include "myutils.h"
#include "vec3.h"
#include "perlin.h"
#include "stb_image.h"

class Texture {
    public:
        virtual Color value(double u, double v, const Point3& p) const = 0;
};

class Solid_color : public Texture {
    public:
        Solid_color() {color_val = Color(0, 0, 0);}
        Solid_color(Color c) : color_val(c) { 
            color_val.val[0] = clamp(color_val.val[0], 0.0, 1.0); 
            color_val.val[1] = clamp(color_val.val[1], 0.0, 1.0); 
            color_val.val[2] = clamp(color_val.val[2], 0.0, 1.0); 
        }

        Solid_color(double red, double green, double blue) : Solid_color(Color(red, blue, green)) {}
        virtual Color value(double u, double v, const Point3& p) const override { return color_val;}
    
    private:
        Color color_val;
};

class Checker_texture : public Texture {
    public:
        Checker_texture() {}
        Checker_texture(shared_ptr<Texture> _even, shared_ptr<Texture> _odd) : even(_even), odd(_odd) {}
        Checker_texture(Color c1, Color c2) : even(make_shared<Solid_color>(c1)), odd(make_shared<Solid_color>(c2)) {}

        virtual Color value(double u, double v, const Point3& p) const override {
            auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
            if (sines < 0)
                return odd->value(u, v, p);
            else
                return even->value(u, v, p);
        }

    public:
        shared_ptr<Texture> odd;
        shared_ptr<Texture> even;
};

class Noise_texture : public Texture {
    public:
        Noise_texture() {Noise_texture(1.0);}
        Noise_texture(double sc) : scale(sc) {}
        virtual Color value(double u, double v, const Point3& p) const override {
            //return Color(1, 1, 1) * 0.5 * (1.0 + noise.noise(scale * p));
            return Color(1, 1, 1) * noise.turb(scale * p);
            //return Color(1, 1, 1) * 0.5 * (1 + sin(scale * p.z() + 10 * nosie.turb(p)));
        }
    public:
        Perlin noise;
        double scale;     

};

class Image_texture : public Texture {
    public:
        const static int bytes_per_pixel = 3;

        Image_texture() : _data(nullptr), _width(0), _height(0), _bytes_per_scanline(0) {std::cout << _width << std::endl;}
        Image_texture(const char* filename);
        ~Image_texture() {delete _data;}

        virtual Color value(double u, double v, const Vec3& p) const override {
            //如果没有纹理数据，就返回固定颜色
            if (_data == nullptr)   return Color(0, 1, 1);

            //输入坐标变换到[0, 1] × [1, 0]
            u = clamp(u, 0.0, 1.0);
            v = 1.0 - clamp(v, 0.0, 1.0);   //对于图片坐标要反转v

            auto i = static_cast<int> (u * _width);
            auto j = static_cast<int> (v * _height);

            //剪切int映射，因为确切坐标应当小于1.0
            if (i >= _width) i = _width - 1;
            if (j >= _height) j = _height - 1;

            const auto color_scale = 1.0 / 255.0;
            auto pixel = _data + j * _bytes_per_scanline + i * bytes_per_pixel;

            return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
        }

    private:
        unsigned char *_data;
        int _width, _height;
        int _bytes_per_scanline;
};

#endif