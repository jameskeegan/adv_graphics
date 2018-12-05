/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

// Material is the base class for materials.

#pragma once

#include "vector.h"
#include "colour.h"

class Material {
public:
	Colour reflection;
	Colour refraction;
	double refractive_index;

public:
	virtual void compute_base_colour(Colour &result)
	{
		result.r = 0.0f;
		result.g = 0.0f;
		result.b = 0.0f;
	}
	virtual void compute_light_colour(Vector &viewer, Vector &normal, Vector &ldir, Colour &result)
	{
		result.r = 0.0f;
		result.g = 0.0f;
		result.b = 0.0f;
	}

	long double get_reflection(){
		return reflection.r;
	}

	void set_reflection(long double ref){
		reflection.r = ref;
		reflection.g = ref;
		reflection.b = ref;
	}

	long double get_refraction(){
		return refraction.r;
	}

	long double get_refraction_index(){
		return refractive_index;
	}

	void set_refraction(long double refr){
		refraction.r = refr;
		refraction.g = refr;
		refraction.b = refr;
	}
};
