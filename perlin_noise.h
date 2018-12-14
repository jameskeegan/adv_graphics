//
// C++ implementation of Ken Perlin's code for Improved noise (COPYRIGHT 2002 KEN PERLIN)
// Perlin's code is wrapped in this class so that it can be accessed like an object in my code
//

#pragma once

#include "vector.h"
#include "colour.h"
#include <iostream>
#include <vector>

// width and height of final Perlin noise image
#define noiseWidth 1024
#define noiseHeight 1024

using namespace std;

class PerlinNoise{

public:
    // contains the permutation values for the pseudorandom gradient selection
    vector<int> p;

    // dimensions of the image, used for scaling later
    int dimensions = noiseWidth;

    // creates framebuffer for perlin noise so that it can be stored and printed
    Colour p_noise[noiseWidth][noiseHeight];

    // creates an instance of Perlin noise, creating an image
    PerlinNoise();
    double noise(double x, double y, double z);

private:
    void write_framebuffer();
    double fade(double t);
    double lerp(double t, double a, double b);
    double grad(int hash, double x, double y, double z);
    double turbulence(double x, double y, double size);
};
