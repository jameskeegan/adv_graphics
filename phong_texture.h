//
// Subclass of Phong, basically the same but with perlin noise texture
//

#pragma once

#include "phong.h"
#include "perlin_noise.h"

class PhongTexture: public Phong {
    PerlinNoise *perlin = new PerlinNoise();

    void compute_base_colour(Colour &result, Vertex &is);
    void compute_light_colour(Vector &viewer, Vector &normal, Vector &ldir, Colour &result);
};