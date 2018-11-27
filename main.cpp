/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

// This file contains the mail entry point for the raytracer, that
// creates the scene and then traces it. It also contains the code to
// write out the framebuffer to a PPM file.

#include "scene.h"
#include "sphere.h"
#include "polymesh.h"
#include "phong.h"
#include "directional_light.h"
#include "sphere.h"
#include "vertex.h"

#include <iostream>
#include <fstream>
using namespace std;

#define XSIZE 512
#define YSIZE 512

Colour framebuffer[YSIZE][XSIZE];

void clear_framebuffer()
{
	int x, y;

	for (y = 0; y < YSIZE; y += 1)
	{
		for (x = 0; x < XSIZE; x += 1)
		{
			framebuffer[y][x].r = 0.0f;
			framebuffer[y][x].g = 0.0f;
			framebuffer[y][x].b = 0.0f;
		}
	}
}

void write_framebuffer()
{
	int x, y;
	ofstream fout;
	
	fout.open("image_64.ppm");

	fout << "P6\r" << XSIZE << " " << YSIZE << "\r255\r";

	for (y = 0; y < YSIZE; y += 1)
	{
		for (x = 0; x < XSIZE; x += 1)
		{
			fout << (unsigned char)(255.0*framebuffer[y][x].r);
			fout << (unsigned char)(255.0*framebuffer[y][x].g);
			fout << (unsigned char)(255.0*framebuffer[y][x].b);
		}
	}

	fout.close();
}

int main(int argc, char *argv[])
{
	Scene scene;
	int x, y;

	clear_framebuffer();

	// loop over the pixels

	Phong bp;

	bp.ambient.r = 0.2f;
	bp.ambient.g = 0.0f;
	bp.ambient.b = 0.0f;
	bp.diffuse.r = 0.4f;
	bp.diffuse.g = 0.0f;
	bp.diffuse.b = 0.0f;
	bp.specular.r = 0.4f;
	bp.specular.g = 0.0f;
	bp.specular.b = 0.0f;
	bp.power = 40.0f;

	DirectionalLight *dl;
	
	dl = new DirectionalLight(Vector(1.0f, -1.0f, 1.0f),Colour(0.5f, 0.5f, 0.5f, 0.0f));

	dl->scene = &scene;

	scene.light_list = dl;

	Transform transform(10.0f,  0.0f,  0.0f, 0.0f,
			     0.0f, 10.0f,  0.0f,-1.0f,
			     0.0f,  0.0f,-10.0f, 2.0f,
			     0.0f,  0.0f,  0.0f, 1.0f);

	const char* mesh_name = "..//bunny_zipper.kcply";

	//PolyMesh *bunny = new PolyMesh((char *) mesh_name, transform);

	Sphere *sphere = new Sphere(Vertex(-1,-1,5), 1);
	Sphere *sphere2 = new Sphere(Vertex(1,1,5), 1);
	Sphere *sphere3 = new Sphere(Vertex(-1,1,5), 1);
	Sphere *sphere4 = new Sphere(Vertex(1,-1,5), 1);

	sphere->material = &bp;
	sphere2->material = &bp;
	sphere3->material = &bp;
	sphere4->material = &bp;
	//bunny->material = &bp;
	//scene.object_list = bunny;
	sphere->next = sphere2;
	sphere2->next = sphere3;
	sphere3->next = sphere4;
	scene.object_list = sphere;

	for(y = 0; y < YSIZE; y += 1)
	{
		//cerr << "Line " << y+1 << " of " << (int)YSIZE << endl;
		float py = (((float)y / (float)YSIZE) - 0.5f)*-1.0f; // 0.5 to -0.5, flipped y axis

		for (x = 0; x < XSIZE; x += 1)
		{
			float px = (((float)x / (float)XSIZE) - 0.5f); // -0.5 to 0.5

			Ray root(Vertex(0.0f, 0.0f, 0.0f),Vector(0.0f, 0.0f, 0.0f));

			root.position.x = 0.0f;
			root.position.y = 0.0f;
			root.position.z = 0.0f;
			root.position.w = 1.0f;

			root.direction.x = px;
			root.direction.y = py;
			root.direction.z = 0.5f;

			root.direction.normalise();

			scene.raytrace(root, 1, scene.object_list, scene.light_list, framebuffer[y][x]);
		}
   }

   cerr << "Done" << endl;

	write_framebuffer();
}
