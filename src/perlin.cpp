#include "perlin.h"

Perlin::Perlin() {
    _ranvec = new Vec3[_point_count];
    for (int i = 0; i < _point_count; ++i) {
        _ranvec[i] = unit_vector(Vec3::random(-1, 1));
    }

    _perm_x = perlin_generate_perm();
    _perm_y = perlin_generate_perm();
    _perm_z = perlin_generate_perm();
}

Perlin::~Perlin() {
    delete[] _ranvec;
    delete[] _perm_x;
    delete[] _perm_y;
    delete[] _perm_z;
}

double Perlin::noise(const Point3& p) const {
    auto u = p.x() - floor(p.x());
    auto v = p.y() - floor(p.y());
    auto w = p.z() - floor(p.z());
    u = u * u * (3 - 2 * u);
    v = v * v * (3 - 2 * v);
    w = w * w * (3 - 2 * w);


    auto i = static_cast<int>(floor(p.x()));
    auto j = static_cast<int>(floor(p.y()));
    auto k = static_cast<int>(floor(p.z()));
    Vec3 c[2][2][2];

    //三线性插值
    for (int di = 0; di < 2; di++) {
        for (int dj = 0; dj < 2; dj++) {
            for (int dk = 0; dk < 2; dk++) {
                c[di][dj][dk] = _ranvec[
                                _perm_x[(i + di) & 255] ^ 
                                _perm_y[(j + dj) & 255] ^ 
                                _perm_z[(k + dk) & 255]
                                ];
            }
        }
    }
    return perlin_interp(c, u, v, w);
}

double Perlin::turb(const Point3& p, int depth) const {
    auto accum = 0.0;
    auto temp_p = p;
    auto weight = 1.0;

    for (int i = 0; i < depth; i++) {
        accum += weight * noise(temp_p);
        weight *= 0.5;
        temp_p *= 2;
    }
    return fabs(accum);
}
