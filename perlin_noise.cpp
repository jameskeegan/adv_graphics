//
// C++ implementation of Ken Perlin's code for Improved noise (COPYRIGHT 2002 KEN PERLIN)
// Perlin's code is wrapped in this class so that it can be accessed like an object in my code
// Idea for a PerlinNoise class inspired from https://solarianprogrammer.com/2012/07/18/perlin-noise-cpp-11/
// Code is similar, but that is because they're both close translations of Perlin's code
//

#include <fstream>
#include "perlin_noise.h"
#include "colour.h"

using namespace std;

PerlinNoise::PerlinNoise() {

    // duplicate the permutations to avoid overflow errors (size 256 -> 512)
    for(int i=0; i < 256; i++){
        p[i] = permutations[i%256];
    }

    // create an image of the Perlin noise
    for(int i=0; i < noiseHeight; i++){
        for(int j=0; j<noiseWidth; j++){
            // scale x and y to the noise image dimensions
            double x = (double)j/((double)noiseWidth);
            double y = (double)i/((double)noiseHeight);

            // code which produces the marble look of the texture
            double xyValue = 3 * x + y + 1.0 * turbulence(j, i, 128.0);
            double n = fabs(sin(xyValue * M_PI));

            // assigns the values of n to the framebuffer (p_noise) for storage and eventual printing
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
    // finds the cube that contains this point
    int X = (int) floor(x) & 255;
    int Y = (int) floor(y) & 255;
    int Z = (int) floor(z) & 255;

    // Find relative x, y, z of point in cube (i.e. turn them into value between 0 and 1)
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    // Compute fade curves for each of the values, which will lead to smoother transition of gradients
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
    double res = lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), grad(p[BA], x-1, y, z)),
            lerp(u, grad(p[AB], x, y-1, z), grad(p[BB], x-1, y-1, z))),
                    lerp(v, lerp(u, grad(p[AA+1], x, y, z-1), grad(p[BA+1], x-1, y, z-1)),
                            lerp(u, grad(p[AB+1], x, y-1, z-1),	grad(p[BB+1], x-1, y-1, z-1))));

    // the +1 and /2 scales this number to a value between 0 and 1
    return (res + 1.0)/2.0;
}

// function which smooths out the transition between gradients
// uses the equation from Perlin's improved noise function, 6t^5 - 15t^4 + 10t^3
double PerlinNoise::fade(double t) {
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

    fout << "P6\r" << noiseWidth << " " << noiseWidth << "\r255\r";

    for (y = 0; y < noiseWidth; y += 1) {
        for (x = 0; x < noiseWidth; x += 1) {
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
        }
    }

    fout.close();
}

// gives the visual appearance of turbulent flow
// function based off code from https://lodev.org/cgtutor/randomnoise.html
double PerlinNoise::turbulence(double x, double y, double size) {

    // stores the original size of the scale for scaling later
    double value = 0.0, og_size = size;

    // keep zooming out until limit is reached
    while (size >= 1) {
        value += fabs(noise(x / size, y / size, 0.8) * size);
        size /= 2.0;
    }

    // scaled so that total value doesn't exceed 1 (or 255 when scaled to RGB)s
    return 0.5 * value / og_size;
}