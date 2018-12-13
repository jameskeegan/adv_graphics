//
// Created by kegof on 06/12/2018.
//

#pragma once

#include "vector.h"
#include "colour.h"
#include <iostream>
#include <vector>

#define noiseWidth 1024
#define noiseHeight 1024

using namespace std;

class PerlinNoise{

public:
    vector<int> p;
    unsigned int seed = 313;
    int dimensions = noiseWidth;
    Colour p_noise[noiseWidth][noiseHeight];

    PerlinNoise();
    double noise(double x, double y, double z);

private:
    void write_framebuffer();
    double fade(double t);
    double lerp(double t, double a, double b);
    double grad(int hash, double x, double y, double z);
    double turbulence(double x, double y, double size);
};
