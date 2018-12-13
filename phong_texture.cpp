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


    //TODO interpolation

    long double uv[2] = {0.0,0.0};

    Colour inter_result;

    //bilinear_interpolation(u, v, inter_result);

    // get the ordinates for the image
    int u_int = (int) u;
    int v_int = (int) v;

    // assign colour of pixel from perlin noise image to the pixel colour

    /*
    result.r = inter_result.r * ambient.r;
    result.g = inter_result.g * ambient.g;
    result.b = inter_result.b * ambient.b;
    */


    result.r = perlin->p_noise[v_int][u_int].r * ambient.r;
    result.g = perlin->p_noise[v_int][u_int].g * ambient.g;
    result.b = perlin->p_noise[v_int][u_int].b * ambient.b;



}

void PhongTexture::compute_light_colour(Vector &viewer, Vector &normal, Vector &ldir, Colour &result) {
    // just call the phong model as that does it fine
    Phong::compute_light_colour(viewer, normal, ldir, result);
}

long double PhongTexture::bi_interpolation(long double ord) {

    long double ord_max = ceil(ord);
    long double ord_min = floor(ord);

    long double diff = ord_max - ord;

    return 0;
}

void PhongTexture::bilinear_interpolation(long double u, long double v, Colour &i_result) {
    //cerr << u << ", " << v << endl;

    int top_x = (int) ceil(u);
    int bottom_x = (int) floor(u);

    int top_y = (int) ceil(v);
    int bottom_y = (int) floor(v);

    long double diff_x = top_x - u;
    long double diff_y = top_y - v;

    long double top_top = perlin->p_noise[top_x][top_y].r;
    long double top_bot = perlin->p_noise[top_x][bottom_y].r;
    long double bot_top = perlin->p_noise[bottom_x][top_y].r;
    long double bot_bot = perlin->p_noise[bottom_x][bottom_y].r;

    i_result.r = perlin->p_noise[top_x][top_y].r * diff_x * diff_y + perlin->p_noise[top_x][bottom_y].r * diff_x * (1-diff_y) + perlin->p_noise[bottom_x][top_y].r * (1-diff_x) * diff_y + perlin->p_noise[bottom_x][bottom_y].r * (1-diff_x) * (1-diff_y);

    i_result.g = perlin->p_noise[top_x][top_y].g * diff_x * diff_y + perlin->p_noise[top_x][bottom_y].g * diff_x * (1-diff_y) + perlin->p_noise[bottom_x][top_y].g * (1-diff_x) * diff_y + perlin->p_noise[bottom_x][bottom_y].g * (1-diff_x) * (1-diff_y);

    i_result.b = perlin->p_noise[top_x][top_y].b * diff_x * diff_y + perlin->p_noise[top_x][bottom_y].b * diff_x * (1-diff_y) + perlin->p_noise[bottom_x][top_y].b * (1-diff_x) * diff_y + perlin->p_noise[bottom_x][bottom_y].b * (1-diff_x) * (1-diff_y);
}
