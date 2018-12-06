/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#include "sphere.h"
#include <math.h>


Sphere::Sphere(Vertex c, long double r)
{
	center = c;
	radius = r;
}

void Sphere::intersection(Ray ray, Hit &hit)
{
	Vector ro;

	hit.flag = false;

	// offset ray by sphere position
	// equivalent to transforming ray into local sphere space

	ro.x = ray.position.x - center.x;
	ro.y = ray.position.y - center.y;
	ro.z = ray.position.z - center.z;

	long double a = (long double)ray.direction.dot(ray.direction);
	long double b = (long double)(2.0 * ray.direction.dot(ro));
	long double c = (long double)ro.dot(ro) - radius*radius;

	long double disc = b*b - 4 * a*c;

	if (disc < 0.0)
	{
		return; // a negative value indicates no intersection.
	}

	long double ds = sqrtf(disc);
	

	long double t0 = (-b - ds) / 2.0;
	long double t1 = (-b + ds) / 2.0;

	if (t1 < 0.0)
	{
		return;
	}

	// if an intersection has been found, record details in hit object

	hit.what = this;

	if (t0 > 0.0) //smallest root is positive.
	{
		hit.t = t0;

		hit.position.x = ray.position.x + t0 * ray.direction.x;
		hit.position.y = ray.position.y + t0 * ray.direction.y;
		hit.position.z = ray.position.z + t0 * ray.direction.z;
		hit.normal.x = hit.position.x - center.x;
		hit.normal.y = hit.position.y - center.y;
		hit.normal.z = hit.position.z - center.z;
		hit.normal.normalise();
		hit.flag = true;

		return;
	}

	hit.t = t1;

	hit.position.x = ray.position.x + t1 * ray.direction.x;
	hit.position.y = ray.position.y + t1 * ray.direction.y;
	hit.position.z = ray.position.z + t1 * ray.direction.z;
	hit.normal.x = hit.position.x - center.x;
	hit.normal.y = hit.position.y - center.y;
	hit.normal.z = hit.position.z - center.z;
	hit.normal.normalise();
	hit.flag = true;

	return;
}