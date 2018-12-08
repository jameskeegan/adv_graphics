//
// Created by kegof on 08/12/2018.
//

#include "phong_texture.h"

void PhongTexture::compute_base_colour(Colour &result, Vertex &is) {
    // radius of the sphere
    long double r = 1.0;

    // centre of the sphere
    Vertex centre(0.0, 0.0, 1.5);

    // normalise the centre
    long double x = is.x - centre.x;
    long double y = is.y - centre.y;
    long double z = is.z - centre.z;

    // calculate the approximate
    long double u = atan2(z,x) / (2*M_PI) + 0.5;
    long double v = 0.5 - asin(y)/M_PI;

    // scale to dimensions of the perlin noise image
    u *= 255;
    v *= 255;

    // get the ordinates for the image
    int u_int = (int) u;
    int v_int = (int) v;

    // assign colour of pixel from perlin noise image to the pixel colour
    result.r = perlin->p_noise[v_int][u_int].r;
    result.g = perlin->p_noise[v_int][u_int].g;
    result.b = perlin->p_noise[v_int][u_int].b;
}

void PhongTexture::compute_light_colour(Vector &viewer, Vector &normal, Vector &ldir, Colour &result) {
    // just call the phong model as that does it fine
    Phong::compute_light_colour(viewer, normal, ldir, result);
}
