//
// Created by kegof on 06/12/2018.
//

#include <random>
#include <algorithm>
#include "perlin_noise.h"
#include "colour.h"

using namespace std;

PerlinNoise::PerlinNoise() {
    p.resize(256);

    int height = 256;
    int width = 256;

    iota(p.begin(), p.end(), 0);

    default_random_engine generator;

    uniform_int_distribution<int> distribution(1,500);

    int s = distribution(generator);

    default_random_engine engine(s);

    shuffle(p.begin(), p.end(), engine);

    p.insert(p.end(), p.begin(), p.end());

    for(int i=0; i < height; i++){
        for(int j=0; j<width; j++){
            double x = (double)j/((double)width);
            double y = (double)i/((double)height);

            // Typical Perlin noise
            double n = noise(10 * x, 10 * y, 0.8);

            p_noise[i][j].r = n;
            p_noise[i][j].g = n;
            p_noise[i][j].b = n;
        }
    }



}

double PerlinNoise::noise(double x, double y, double z) {
    // Find the unit cube that contains the point
    int X = (int) floor(x) & 255;
    int Y = (int) floor(y) & 255;
    int Z = (int) floor(z) & 255;

    // Find relative x, y,z of point in cube
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    // Compute fade curves for each of x, y, z
    double u = fade(x);
    double v = fade(y);
    double w = fade(z);

    // Hash coordinates of the 8 cube corners
    int A = p[X] + Y;
    int AA = p[A] + Z;
    int AB = p[A + 1] + Z;
    int B = p[X + 1] + Y;
    int BA = p[B] + Z;
    int BB = p[B + 1] + Z;

    // Add blended results from 8 corners of cube
    double res = lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), grad(p[BA], x-1, y, z)), lerp(u, grad(p[AB], x, y-1, z), grad(p[BB], x-1, y-1, z))),	lerp(v, lerp(u, grad(p[AA+1], x, y, z-1), grad(p[BA+1], x-1, y, z-1)), lerp(u, grad(p[AB+1], x, y-1, z-1),	grad(p[BB+1], x-1, y-1, z-1))));
    return (res + 1.0)/2.0;
}

double PerlinNoise::fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double t, double a, double b) {
    return a + t * (b - a);
}

double PerlinNoise::grad(int hash, double x, double y, double z) {
    int h = hash & 15;
    // Convert lower 4 bits of hash into 12 gradient directions
    double u = h < 8 ? x : y,
            v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
