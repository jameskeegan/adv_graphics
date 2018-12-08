//
// Created by kegof on 06/12/2018.
// Texture for perlin noise
//
#pragma once

#include "material.h"
#include "perlin_noise.h"

class Texture: public Material {

    PerlinNoise *perlin = new PerlinNoise();

    void compute_base_colour(Colour &result);

    void compute_light_colour(Vector &viewer, Vector &normal, Vector &ldir, Vertex &is, Colour &result);
};
