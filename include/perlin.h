#ifndef PERLIN_H
#define PERLIN_H

#include "myutils.h"
#include "vec3.h"

class Perlin {
    public:
        Perlin();
        ~Perlin();

        double noise(const Point3& p) const;
        double turb(const Point3& p, int depth = 7) const;
    private:
        static int* perlin_generate_perm() {
            auto p = new int[_point_count];
            for (int i = 0; i < Perlin::_point_count; ++i) {
                p[i] = i;
            }

            permute(p, _point_count);

            return p;
        }
        static void permute(int* p, int n) {
            for (int i = n - 1; i > 0; --i) {
                int target = RandomInt(0, i);
                int tmp = p[i];
                p[i] = p[target];
                p[target] = tmp;
            }
        }
        static double trilinear_interp(double c[2][2][2], double u, double v, double w) {
            auto accum = 0.0;
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    for (int k = 0; k < 2; k++)
                        accum += (i * u + (1 - i) * (1 - u)) * 
                                 (j * v + (1 - j) * (1 - v)) *
                                 (k * w + (1 - k) * (1 - w)) * c[i][j][k];
                }
            }
            return accum;
        }
        static double perlin_interp(Vec3 c[2][2][2], double u, double v, double w) {
            auto uu = u * u * (3 - 2 * u);
            auto vv = v * v * (3 - 2 * v);
            auto ww = w * w * (3 - 2 * w);
            auto accum = 0.0;

            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    for (int k = 0; k < 2; k++) {
                        Vec3 weight_v(u - i, v - j, w - k);
                        accum += (i * uu + (1 - i) * (1 - uu))
                               * (j * vv + (1 - j) * (1 - vv))
                               * (k * ww + (1 - k) * (1 - ww))
                               * dot(c[i][j][k], weight_v);                        
                    }
                }
            }
            return accum;
        }

        static const int _point_count = 256;
        Vec3* _ranvec;
        int* _perm_x;
        int* _perm_y;
        int* _perm_z;
};

#endif