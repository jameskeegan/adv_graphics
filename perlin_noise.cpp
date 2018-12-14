//
// C++ implementation of Ken Perlin's code for Improved noise (COPYRIGHT 2002 KEN PERLIN)
// Perlin's code is wrapped in this class so that it can be accessed like an object in my code
//

#include <fstream>
#include <algorithm>
#include "perlin_noise.h"
#include "colour.h"

using namespace std;

PerlinNoise::PerlinNoise() {

    // initialisation of the p vector, containing values for random gradients
    // values are random, and straight from Perlin's improved noise implementation
    p = {
            151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
            8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
            35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,
            134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
            55,46,245,40,244,102,143,54, 65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
            18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
            250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
            189,28,42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167,
            43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,
            97,228,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,
            107,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
            138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };

    // duplicate the p to avoid overflow errors (size 256 -> 512)
    p.insert(p.end(), p.begin(), p.end());

    // create an image of the Perlin noise
    for(int i=0; i < noiseHeight; i++){
        for(int j=0; j<noiseWidth; j++){
            double x = (double)j/((double)noiseWidth);
            double y = (double)i/((double)noiseHeight);

            // Typical Perlin noise
            //double n = noise(10 * x, 10 * y, 0.8);

            double xyValue = x + y + 1.0 * turbulence(j, i, 128.0);
            double n = fabs(sin(xyValue * 2*M_PI));

            // assigns the values of n to the frambuffer, p_noise, for storage and eventual printing
            p_noise[i][j].r = n;
            p_noise[i][j].g = n;
            p_noise[i][j].b = n;
        }
    }

    // output the perlin noise as an image (for verification that the noise is correct, not printing the final output)
    write_framebuffer();

}

// gets colour (noise) for a specific point
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

    // the +1 and /2 helps scale this number to a value between 0 and 1
    return (res + 1.0)/2.0;
}

// function which smooths out the transition between gradients
// uses the equation from Perlin's improved noise function, 6t^5 - 15t^4 + 10t^3
double PerlinNoise::fade(double t) {
    //return t * t * t * (t * (t * 6 - 15) + 10);
    // TEST THIS
    return 6*pow(t,5) - 15*pow(t,4) + 10*pow(t,3);
}

// function for linear interpolation of the points
double PerlinNoise::lerp(double t, double a, double b) {
    return a + t * (b - a);
}

// calculates the dot product of the gradient vectors and the distance vectors
double PerlinNoise::grad(int hash, double x, double y, double z) {
    int h = hash & 15;
    // Convert lower 4 bits of hash into 12 gradient directions
    double u = h < 8 ? x : y,
            v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

// the write_framebuffer() method provided by Ken, reused here
void PerlinNoise::write_framebuffer() {
    int x, y;
    std::ofstream fout;

    fout.open("perlin_noise.ppm");

    fout << "P6\r" << dimensions << " " << dimensions << "\r255\r";

    for (y = 0; y < dimensions; y += 1) {
        for (x = 0; x < dimensions; x += 1) {
            long double red = 255.0 * p_noise[y][x].r;
            long double green = 255.0 * p_noise[y][x].g;
            long double blue = 255.0 * p_noise[y][x].b;

            // checks if they don't equal " ", which breaks the ppm file
            if (green > 6.0) {
                if (green < 12.0) {
                    green = 0.0;

                }
            }

            if (red > 6.0) {
                if (red < 12.0) {
                    red = 0.0;

                }
            }

            if (blue > 6.0) {
                if (blue < 12.0) {
                    blue = 0.0;

                }
            }

            if(green > 255.0){
                cerr << green << endl;
            }else if(green < 0.0){
                cerr << green << endl;
            }

            fout << (unsigned char) (red) << (unsigned char) (green) << (unsigned char) (blue);

            //cerr << green << endl;
        }
    }

    fout.close();
}

// gives the visual appearance of turbulent flow
double PerlinNoise::turbulence(double x, double y, double size) {
    double value = 0.0, initialSize = size;

    while (size >= 1) {
        value += fabs(noise(x / size, y / size, 0.8) * size);
        size /= 2.0;
    }

    return 0.5 * value / initialSize;
}