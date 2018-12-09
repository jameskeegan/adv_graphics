//
// Created by kegof on 09/12/2018.
//

#include <iostream>
#include "phong.h"
#include "perlin_noise.h"

#pragma once

using namespace std;

class PhongTextureMesh: public Phong {

public:
    PerlinNoise *perlin = new PerlinNoise();

    void compute_base_colour(Colour &result, Hit &hit);
    void compute_light_colour(Vector &viewer, Vector &normal, Vector &ldir, Colour &result);
};
