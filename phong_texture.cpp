//
// Created by kegof on 08/12/2018.
//

#include "phong_texture.h"

void PhongTexture::compute_base_colour(Colour &result, Hit &hit) {

    // normalise the centre
    long double x = hit.position.x - sphere_centre.x;
    long double y = hit.position.y - sphere_centre.y;
    long double z = hit.position.z - sphere_centre.z;

    // calculate the approximate
    long double u = atan2(z,x) / (2*M_PI) + 0.5;
    long double v = 0.5 - asin(y)/M_PI;

    // scale to dimensions of the perlin noise image
    u *= perlin->dimensions;
    v *= perlin->dimensions;

    // get the ordinates for the image
    int u_int = (int) u;
    int v_int = (int) v;

    // assign colour of pixel from perlin noise image to the pixel colour
    result.r = perlin->p_noise[v_int][u_int].r * ambient.r;
    result.g = perlin->p_noise[v_int][u_int].g * ambient.g;
    result.b = perlin->p_noise[v_int][u_int].b * ambient.b;
}

void PhongTexture::compute_light_colour(Vector &viewer, Vector &normal, Vector &ldir, Colour &result) {
    // just call the phong model as that does it fine
    Phong::compute_light_colour(viewer, normal, ldir, result);
}
