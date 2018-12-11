//
// Subclass of Phong, basically the same but with perlin noise texture
//

#pragma once

#include "phong.h"
#include "perlin_noise.h"

class PhongTexture: public Phong {

public:
    PerlinNoise *perlin = new PerlinNoise();
    Vertex sphere_centre;

    void compute_base_colour(Colour &result, Hit &hit);
    void compute_light_colour(Vector &viewer, Vector &normal, Vector &ldir, Colour &result);

    long double bi_interpolation(long double ord);

    void bilinear_interpolation(long double u, long double d, Colour &i_result);
};