#include <fstream>
#include <iostream>
#include <vector>

#include "myutils.h"
#include "sphere.h"
#include "camera.h"
#include "scene.h"
#include "material.h"
#include "texture.h"
#include "aarect.h"
#include "box.h"
#include "bvh.h"
#include "constant_medium.h"


//将颜色写入图片中，基础版
void write_color(std::ofstream &out, std::vector<std::vector<Vec3>> img) {
    out.open("RayTracing.ppm", std::ios::out);
    out << "P3\n" << static_cast<int>(img[0].size()) << " " << static_cast<int>(img.size()) << "\n255\n";
    for (int j = img.size() - 1; j >= 0; j--) {
        for (int i = 0; i < static_cast<int>(img[0].size()); i++) {
            out << static_cast<int>(255.999 * clamp(img[j][i].x(), 0.0, 0.999)) << " "
                << static_cast<int>(255.999 * clamp(img[j][i].y(), 0.0, 0.999)) << " "
                << static_cast<int>(255.999 * clamp(img[j][i].z(), 0.0, 0.999)) << "\n";
            }
    }
    out.close();
}

//光线颜色
Color ray_color(const Ray& r, const Color& background, const Scene& world, int depth) {
    hit_record rec;

    //结束条件
    if (depth <= 0) return Color(0, 0, 0);

    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    // Point3 target = rec.p + rec.normal + random_in_unit_sphere();   //圆内拒绝-接收采样
    // Point3 target = rec.p + rec.normal + random_on_unit_sphere();   //圆上重要性采样
    // Point3 target = rec.p + random_in_hemisphere(rec.normal);   //半球采样
    Ray scattered;
    Color attenuation;
    Color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;
    
    return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
}

//场景构建
Scene random_scene() {
    Scene world;

    auto checker_tex = make_shared<Checker_texture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    world.addObj(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(checker_tex)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = Random(0.0, 1.0);
            Point3 center(a + 0.9 * Random(0.0, 1.0), 0.2, b + 0.9 * Random(0.0, 1.0));

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    //漫反射材质
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    auto center2 = center + Vec3(0, Random(0.0, 1.0), 0);
                    world.addObj(make_shared<Moving_Sphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    //镜面材质
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = Random(0.0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.addObj(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    //玻璃材质
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.addObj(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.addObj(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.addObj(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.addObj(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}

Scene two_spheres() {
    Scene scene;
    
    auto earth_texture = make_shared<Image_texture> ("/home/wzs/codes/RayTracing/images/texture_sphere/earthmap.jpg");
    auto earth_surface = make_shared<Lambertian>(earth_texture);

    scene.addObj(make_shared<Sphere>(Point3(0, 0, 0), 2, earth_surface));

    std::cerr << "Scene build done!" << std::endl;

    return scene;
}

Scene simple_light() {
    Scene world;

    auto pertext = make_shared<Noise_texture>(4);
    world.addObj(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
    world.addObj(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(pertext)));
    
    auto difflight = make_shared<Diffuse_light>(Color(1, 1, 1));
    world.addObj(make_shared<XY_rect>(3, 5, 1, 3, -2, difflight));

    return world;
}

Scene Cornell_box() {
    Scene world;
    auto red = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<Diffuse_light>(Color(15, 15, 15));

    world.addObj(make_shared<YZ_rect>(0, 555, 0, 555, 555, green));
    world.addObj(make_shared<YZ_rect>(0, 555, 0, 555, 0, red));
    world.addObj(make_shared<XZ_rect>(213, 343, 227, 332, 554, light));
    world.addObj(make_shared<XZ_rect>(0, 555, 0, 555, 0, white));
    world.addObj(make_shared<XZ_rect>(0, 555, 0, 555, 555, white));
    world.addObj(make_shared<XY_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<Hittable> box1 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = make_shared<Rotate>(box1, 15, 1);
    box1 = make_shared<Translate>(box1, Vec3(265, 0, 295));
    world.addObj(box1);

    shared_ptr<Hittable> box2 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box2 = make_shared<Rotate>(box2, -18, 1);
    box2 = make_shared<Translate>(box2, Vec3(130, 0, 65));
    world.addObj(box2);

    return world;
}

Scene Cornell_smoke() {
    Scene world;

    auto red = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<Diffuse_light>(Color(1.0, 1.0, 1.0));

    world.addObj(make_shared<YZ_rect>(0, 555, 0, 555, 555, green));
    world.addObj(make_shared<YZ_rect>(0, 555, 0, 555, 0, red));
    world.addObj(make_shared<XZ_rect>(113, 443, 127, 432, 554, light));
    world.addObj(make_shared<XZ_rect>(0, 555, 0, 555, 555, white));
    world.addObj(make_shared<XZ_rect>(0, 555, 0, 555, 0, white));
    world.addObj(make_shared<XY_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<Hittable> box1 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = make_shared<Rotate>(box1, 15, 1);
    box1 = make_shared<Translate>(box1, Vec3(265, 0, 295));
    
    shared_ptr<Hittable> box2 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box2 = make_shared<Rotate>(box2, -18, 1);
    box2 = make_shared<Translate>(box2, Vec3(130, 0, 65));

    world.addObj(make_shared<Constant_medium>(box1, 0.01, Color(0, 0, 0)));
    world.addObj(make_shared<Constant_medium>(box2, 0.01, Color(1, 1, 1)));

    return world;
}

Scene Final_scene() {
    //box1为地面
    Scene box1;
    auto ground = make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = Random(1.0, 101.0);
            auto z1 = z0 + w;

            box1.addObj(make_shared<Box>(Point3(x0, y0, z0), Point3(x1, y1, z1), ground));
        }
    }

    //整个场景，地面+各种材质球+泡沫球
    Scene world;

    world.addObj(make_shared<BVH_Node>(box1, 0, 1));

    //区域光
    auto light = make_shared<Diffuse_light>(Color(1.0, 1.0, 1.0));
    world.addObj(make_shared<XZ_rect>(123, 423, 147, 412, 554, light));

    //运动球
    auto center1 = Point3(400, 400, 200);
    auto center2 = center1 + Vec3(30, 0, 0);
    auto moving_sphere_material = make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
    world.addObj(make_shared<Moving_Sphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    //材质球
    world.addObj(make_shared<Sphere>(Point3(260, 150, 45), 50, make_shared<Dielectric>(1.5)));
    world.addObj(make_shared<Sphere>(Point3(0, 150, 145), 50, make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)));

    //体积云雾
    auto boundary = make_shared<Sphere>(Point3(360, 150, 145), 70, make_shared<Dielectric>(1.5));
    world.addObj(boundary);
    world.addObj(make_shared<Constant_medium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
    boundary = make_shared<Sphere>(Point3(0, 0, 0), 5000, make_shared<Dielectric>(1.5));
    world.addObj(make_shared<Constant_medium>(boundary, .0001, Color(1, 1, 1)));

    //地球贴图球+噪声球
    auto earthmat = make_shared<Lambertian>(make_shared<Image_texture>("/home/wzs/codes/RayTracing/images/texture_sphere/earthmap.jpg"));
    world.addObj(make_shared<Sphere>(Point3(400, 200, 400), 100, earthmat));
    auto pertext = make_shared<Noise_texture>(0.1);
    world.addObj(make_shared<Sphere>(Point3(220, 280, 300), 80, make_shared<Lambertian>(pertext)));


    //box2为泡沫球
    Scene box2;
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        box2.addObj(make_shared<Sphere>(Point3::random(0, 165), 10, white));
    }

    world.addObj(make_shared<Translate>(
                    make_shared<Rotate>(
                        make_shared<BVH_Node>(box2, 0.0, 1.0), 15, 1),
                         Vec3(-100, 270, 395)
                    )
                );
    return world;
}

std::string time_count(const clock_t& start, const clock_t& end) {
    std::string result = "Total time: ";
    double sec_total = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    int rec_total = static_cast<int>(sec_total);
    result += std::to_string(sec_total) +"ms; ";
    int h_total = rec_total / 3600;
    result += std::to_string(h_total) + "h, ";
    rec_total = rec_total % 3600;
    int min_total = rec_total / 60;
    result += std::to_string(min_total) + "min, ";
    rec_total = rec_total % 60;
    result += std::to_string(rec_total) + "s.\n";
    return result;
}

int main(){
    //时间记录
    clock_t start_time = clock();

    //image 大小
    auto aspect_ratio = 16.0 / 9.0;
    int img_width = 400;
    int samples = 100;
    int max_depth = 50;
    double gamma = 2.2;

    //场景设置
    Color background(0, 0, 0);
    Scene world;
    Point3 lookfrom;
    Point3 lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;

    switch (0) {
        case 1:
            background = Color(0.70, 0.80, 1.00);
            world = random_scene();
            // world.havingaabb = world.bounding_box(0.0, 0.0, world.sceaabb);
            lookfrom = Point3(13, 2, 3);
            lookat = Point3(0, 0, 0);
            vfov = 20.0;
            aperture = 0.1;
            break;
        case 2:
            background = Color(0.70, 0.80, 1.00);
            world = two_spheres();
            lookfrom = Point3(13, 2, 3);
            lookat = Point3(0, 0, 0);
            vfov = 20.0;
            break;
        case 3:
            background = Color(0.50, 0.40, 1.00);
            world = simple_light();
            lookfrom = Point3(26, 3, 6);
            lookat = Point3(0, 2, 0);
            vfov = 20.0;
            break;
        case 4:
            world = Cornell_box();
            aspect_ratio = 1.0;
            img_width = 600;
            samples = 10;
            background = Color(0.5, 0.4, 0.7);
            lookfrom = Color(278, 278, -800);
            lookat = Point3(278, 278, 0);
            vfov = 40.0;
            break;   
        default:
        case 5:
            world = Cornell_smoke();
            aspect_ratio = 1.0;
            img_width = 600;
            samples = 100;
            background = Color(0.5, 0.4, 0.7);
            lookfrom = Point3(278, 278, -800);
            lookat = Point3(278, 278, 0);
            vfov = 40.0;
            break;
        case 6:
            world = Final_scene();
            aspect_ratio = 1.0;
            img_width = 800;
            samples = 10000;
            background = Color(0, 0, 0);
            lookfrom = Point3(478, 278, -600);
            lookat = Point3(278, 278, 0);
            vfov = 40.0;
            break;
    }

    //image 大小
    const auto ASPECT_RATIO = aspect_ratio;
    const int IMG_WIDTH = img_width;
    const int IMG_HEIGHT = static_cast<int>(IMG_WIDTH / ASPECT_RATIO);
    const int SAMPLERS = samples;
    const int MAX_DEPTH = max_depth;
    const double GAMMA = gamma;

    //相机设置
    Vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    Camera cam(lookfrom, lookat, vup, vfov, ASPECT_RATIO, aperture, dist_to_focus, 0.0, 1.0);

    //渲染到图像
    std::cout << "P3\n" << IMG_WIDTH << " " << IMG_HEIGHT << "\n255\n";
    std::vector<std::vector<Vec3>> img(IMG_HEIGHT, std::vector(IMG_WIDTH, Vec3()));

    for (int j = IMG_HEIGHT - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
        for (int i = 0; i < IMG_WIDTH; ++i) {
            for (int s = 0; s < SAMPLERS; ++s) {
                auto u = (i + Random((double)0.0,  (double)1.0)) / (IMG_WIDTH - 1);
                auto v = (j + Random((double)0.0,  (double)1.0)) / (IMG_HEIGHT - 1);
                Ray r = cam.get_ray(u, v);
                img[j][i] += ray_color(r, background, world, MAX_DEPTH);
            }
            img[j][i] /= SAMPLERS;
            //gamma校正
            img[j][i].val[0] = pow(img[j][i].x(), 1/GAMMA);
            img[j][i].val[1] = pow(img[j][i].y(), 1/GAMMA);
            img[j][i].val[2] = pow(img[j][i].z(), 1/GAMMA);
        }
    }
    std::ofstream outfile;
    write_color(outfile, img);
    std::cerr << "\n Rendering Done.\n";

    clock_t end_time = clock();
    std::string time_rec = time_count(start_time, end_time);
    std::cerr << time_rec; 
}