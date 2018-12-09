//
// Created by kegof on 09/12/2018.
//

#include "phong_texture_mesh.h"
#include "object.h"

void PhongTextureMesh::compute_base_colour(Colour &result, Hit &hit) {
    /*

    Vector bary;

    Vector a, b, c;

    a.x = hit.triangle[0].x;
    a.y = hit.triangle[0].y;
    a.z = hit.triangle[0].z;

    b.x = hit.triangle[1].x;
    b.y = hit.triangle[1].y;
    b.z = hit.triangle[1].z;

    c.x = hit.triangle[2].x;
    c.y = hit.triangle[2].y;
    c.z = hit.triangle[2].z;

    Vector ab, ac;

    ab.x = b.x - a.x;
    ab.y = b.y - a.y;
    ab.z = b.z - a.z;

    ac.x = c.x - a.x;
    ac.y = c.y - a.y;
    ac.z = c.z - a.z;

    Vector pa, pb, pc;

    pa.x = a.x - hit.position.x;
    pa.y = a.y - hit.position.y;
    pa.z = a.z - hit.position.z;

    pb.x = b.x - hit.position.x;
    pb.y = b.y - hit.position.y;
    pb.z = b.z - hit.position.z;

    pc.x = c.x - hit.position.x;
    pc.y = c.y - hit.position.y;
    pc.z = c.z - hit.position.z;

    Vector abac, pbpc, papc;

    ab.cross(ac, abac);
    pb.cross(pc, pbpc);
    pc.cross(pa, papc);

    long double areaABC = hit.normal.dot(abac);
    long double areaPBC = hit.normal.dot(pbpc);
    long double areaPCA = hit.normal.dot(papc);

    bary.x = areaPBC / areaABC;
    bary.y = areaPCA / areaABC;
    bary.z = 1.0 - bary.x - bary.y;

    bary.x *= perlin->dimensions;
    bary.y *= perlin->dimensions;

    int u_int = (int) bary.x;
    int v_int = (int) bary.y;

    result.r = perlin->p_noise[v_int][u_int].r * ambient.r;
    result.g = perlin->p_noise[v_int][u_int].g * ambient.g;
    result.b = perlin->p_noise[v_int][u_int].b * ambient.b;

    */

    Vector scaled_intersection;

    scaled_intersection.x = hit.position.x + abs(hit.what->smallest_x);
    scaled_intersection.y = hit.position.y + abs(hit.what->smallest_y);

    long double ratio_x = scaled_intersection.x / (abs(hit.what->smallest_x) + abs(hit.what->largest_x));
    long double ratio_y = scaled_intersection.y / (abs(hit.what->smallest_y) + abs(hit.what->largest_y));

    ratio_x *= perlin->dimensions;
    ratio_y *= perlin->dimensions;

    int u = (int) ratio_x;
    int v = (int) ratio_y;

    result.r = perlin->p_noise[u][v].r * ambient.r;
    result.g = perlin->p_noise[u][v].g * ambient.g;
    result.b = perlin->p_noise[u][v].b * ambient.b;

}

void PhongTextureMesh::compute_light_colour(Vector &viewer, Vector &normal, Vector &ldir, Colour &result) {
    Phong::compute_light_colour(viewer, normal, ldir, result);
}
