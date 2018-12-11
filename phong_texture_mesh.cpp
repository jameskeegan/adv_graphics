//
// Created by kegof on 09/12/2018.
//

#include "phong_texture_mesh.h"
#include "object.h"

void PhongTextureMesh::compute_base_colour(Colour &result, Hit &hit) {

    Vector scaled_intersection;
    scaled_intersection.x = hit.position.x + abs(hit.what->smallest_x);
    scaled_intersection.y = hit.position.y + abs(hit.what->smallest_y);

    // gets the coordinates as a scale [0->1] in pixel space
    long double ratio_x = scaled_intersection.x / (abs(hit.what->smallest_x) + abs(hit.what->largest_x));
    long double ratio_y = scaled_intersection.y / (abs(hit.what->smallest_y) + abs(hit.what->largest_y));

    // convert (u,v) into the pixel area space
    ratio_x *= perlin->dimensions;
    ratio_y *= perlin->dimensions;

    //TODO: different interpolation

    // get the corresponding integer vertices for the pixel area space
    int u = (int) ratio_x;
    int v = (int) ratio_y;

    // apply this to the result, scaling with the ambient terms
    result.r = perlin->p_noise[u][v].r * ambient.r;
    result.g = perlin->p_noise[u][v].g * ambient.g;
    result.b = perlin->p_noise[u][v].b * ambient.b;

}

void PhongTextureMesh::compute_light_colour(Vector &viewer, Vector &normal, Vector &ldir, Colour &result) {
    Phong::compute_light_colour(viewer, normal, ldir, result);
}
