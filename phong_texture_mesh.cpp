//
// Created by kegof on 09/12/2018.
//

#include "phong_texture_mesh.h"

void PhongTextureMesh::compute_base_colour(Colour &result, Hit &hit) {
    Phong::compute_base_colour(result, hit);
}

void PhongTextureMesh::compute_light_colour(Vector &viewer, Vector &normal, Vector &ldir, Colour &result) {
    Phong::compute_light_colour(viewer, normal, ldir, result);
}
