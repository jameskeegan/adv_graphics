//
// Created by Keegan on 05/12/2018.
//

#include "physical_light.h"
#include "ray.h"
#include "hit.h"
#include "scene.h"

PhysicalLight::PhysicalLight() {
    Light();
}

PhysicalLight::PhysicalLight(Vector pos, Vector dir, Colour col) {

    Light();

    position = pos;
    direction = dir;
    direction.normalise();
    colour = col;

}

bool PhysicalLight::get_direction(Vertex &surface, Vector &dir) {

    dir = direction;

    // shadow ray is created
    Ray shadow_ray = Ray();

    // position is the point of intersection, plus a lil bit more so that the trace() doesn't just detect itself
    shadow_ray.position.x = surface.x - 0.001*direction.x;
    shadow_ray.position.y = surface.y - 0.001*direction.y;
    shadow_ray.position.z = surface.z - 0.001*direction.z;

    // going towards the light, so invert the light direction
    shadow_ray.direction.x = -direction.x;
    shadow_ray.direction.y = -direction.y;
    shadow_ray.direction.z = -direction.z;

    // trace shadow_ray, which will return if there was another object blocking it or not
    Hit shadow_hit;
    scene->trace(shadow_ray, scene->object_list, shadow_hit);

    // returns true if light is visible to object, false if object is blocked
    return !shadow_hit.flag;
}

void PhysicalLight::get_intensity(Vertex &surface, Colour &level) {
    level = colour;
}
