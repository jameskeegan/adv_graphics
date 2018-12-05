/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#include "scene.h"

using namespace std;

Scene::Scene()
{
	object_list = nullptr;
	light_list = nullptr;
}

void Scene::trace(Ray ray, Object *objects, Hit &hit)
{
	Hit current_hit;

	hit.flag = false;
	hit.t = 0.0f;
	hit.what = 0;

	while (objects != nullptr)
	{
		Hit hit_current;

		objects->intersection(ray, hit_current);

		if (hit_current.flag)
		{
			if (!hit.flag)
			{
				hit = hit_current;

			} else if (hit_current.t < hit.t)
			{
				hit = hit_current;
			}
		}

		objects = objects->next;
	}
}

void Scene::raytrace(Ray ray, int level, Object *objects, Light *lights, Colour &colour)
{

	// check we've not recursed too far.
	level = level - 1;
	if (level < 0)
	{
		return;
	}

	// first step, find the closest primitive

	Hit best_hit;
	trace(ray, objects, best_hit);

	// if we found a primitive then compute the colour we should see
	if (best_hit.flag)
	{

		best_hit.what->material->compute_base_colour(colour);

		Light *light = lights;

		while (light != (Light *)0)
		{
			Vector viewer;
			Vector ldir;

			viewer.x = -best_hit.position.x;
			viewer.y = -best_hit.position.y;
			viewer.z = -best_hit.position.z;
			viewer.normalise();

			bool lit;

			lit = light->get_direction(best_hit.position, ldir);

			if (lit)
			{
				Colour intensity;
				Colour scaling;

				light->get_intensity(best_hit.position, scaling);

				best_hit.what->material->compute_light_colour(viewer, best_hit.normal, ldir, intensity);

				intensity.scale(scaling);

				colour.add(intensity);
			}

			light = light->next;
		}

		// gets reflection factor of the material
		long double ref_co = best_hit.what->material->get_reflection();

		// if true this means item material can reflect
		if(ref_co > 0){

			// get colour of reflections
			Colour reflection_colour = reflective_rays(ray, best_hit, level, objects, lights);

            // add reflection colour onto main colour
            colour.r += ref_co * reflection_colour.r;
            colour.g += ref_co * reflection_colour.g;
            colour.b += ref_co * reflection_colour.b;
		}

		//long double refraction_co = best_hit.what->material->get_refraction();
		long double refr_co = best_hit.what->material->refraction.r;

		// TODO: compute refraction ray if material supports it.
		if(refr_co>0){

			// get colour of refractions
			Colour refraction_colour = refraction_rays(ray, best_hit, level, objects, lights);

			// add refraction colour onto main colour
			colour.r += refr_co * refraction_colour.r;
			colour.g += refr_co * refraction_colour.g;
			colour.b += refr_co * refraction_colour.b;
		}
		
	}
	else
	{
		colour.r = 0.0f;
		colour.g = 0.0f;
		colour.b = 0.0f;
	}
}

/**
 * Does raytracing for the reflected rays from a reflective surface
 * @param ray : original ray
 * @param best_hit : intersection point of ray
 * @param level : levels of recursion to do
 * @param objects : list of objects in scene
 * @param lights : list of lights in scene
 * @return colour of accumulation of reflection rays
 */
Colour Scene::reflective_rays(Ray ray, Hit best_hit, int level, Object *objects, Light *lights) {
	Ray reflection;
    Colour reflection_colour = Colour();

	// create reflection ray direction
	// R = E - 2*incident*N, where incident = N.E
	long double incident = ray.direction.dot(best_hit.normal);
	reflection.direction.x = ray.direction.x - 2.0f * incident * best_hit.normal.x;
	reflection.direction.y = ray.direction.y - 2.0f * incident * best_hit.normal.y;
	reflection.direction.z = ray.direction.z - 2.0f * incident * best_hit.normal.z;

	// create reflection ray's position
	reflection.position.x = best_hit.position.x + 0.001f * reflection.direction.x;
	reflection.position.y = best_hit.position.y + 0.001f * reflection.direction.y;
	reflection.position.z = best_hit.position.z + 0.001f * reflection.direction.z;

	// recursive call, going down as many layers as said to reflect
	raytrace(reflection, level, objects, lights, reflection_colour);

    return reflection_colour;
}

Colour Scene::refraction_rays(Ray ray, Hit best_hit, int level, Object *objects, Light *lights) {

	Vector og_ray_dir = ray.direction;
	og_ray_dir.normalise();

	// calculates angle of ___
	Vector normal = best_hit.normal;
	normal.normalise();

	long double cos = normal.dot(ray.direction);
	long double ir_ray = 1.0;
	long double ir_medium = objects->material->refractive_index;

	long double k_refr = objects->material->refraction.r;

	Ray refraction_ray;



	if(cos < 0) {
	    cos *= -1;
	}else{
        normal.x *= -1;
        normal.y *= -1;
        normal.z *= -1;
        swap(ir_ray, ir_medium);
	}

	// value equals n1/n2
	long double n1n2 = ir_ray/ir_medium;

	long double k = 1 - n1n2*n1n2*(1-cos*cos);
	long double cost = sqrt(k);

	// if true, then total internal reflection, so don't compute
	if(k<0){
	    k_refr = 1.0;
	    refraction_ray.direction.x = 0.0;
	    refraction_ray.direction.y = 0.0;
	    refraction_ray.direction.z = 0.0;
	}else{
	    long double rpar = (ir_ray * cos - cost) / (ir_ray * cos + cost);
	    long double rper = (cos - ir_ray * cost) / (cos + ir_ray*cost);
		k_refr = 0.5 * (rpar*rpar + rper*rper);
		refraction_ray.direction.x = n1n2 * ray.direction.x + (n1n2*cos - cost)*normal.x;
		refraction_ray.direction.y = n1n2 * ray.direction.y + (n1n2*cos - cost)*normal.y;
		refraction_ray.direction.z = n1n2 * ray.direction.z + (n1n2*cos - cost)*normal.z;
		refraction_ray.direction.normalise();
	}

	refraction_ray.position.x = ray.position.x + 0.001*refraction_ray.direction.x;
	refraction_ray.position.y = ray.position.y + 0.001*refraction_ray.direction.y;
	refraction_ray.position.z = ray.position.z + 0.001*refraction_ray.direction.z;


	Colour refracted_colour;
	raytrace(refraction_ray, level -1, objects, lights, refracted_colour);

	return refracted_colour;
}
