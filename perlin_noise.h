//
// Created by kegof on 06/12/2018.
//

#pragma once

#include "vector.h"
#include "colour.h"
#include <iostream>
#include <vector>

using namespace std;

class PerlinNoise{

public:
    vector<int> p;
    unsigned int seed = 313;
    Colour p_noise[256][256];

    PerlinNoise();
    double noise(double x, double y, double z);

private:
    double fade(double t);
    double lerp(double t, double a, double b);
    double grad(int hash, double x, double y, double z);
};
