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
#include "phong_texture.h"
#include "phong_texture_mesh.h"
#include "directional_light.h"
#include "physical_light.h"
#include "sphere.h"
#include "vertex.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <thread>
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
	
	fout.open("image_1024.ppm");

	fout << "P6\r" << XSIZE << " " << YSIZE << "\r255\r";

	for (y = 0; y < YSIZE; y += 1)
	{
		for (x = 0; x < XSIZE; x += 1)
		{
			long double red = 255.0*framebuffer[y][x].r;
			long double green = 255.0*framebuffer[y][x].g;
			long double blue = 255.0*framebuffer[y][x].b;

			// checks if they don't equal " ", which breaks the ppm file
			if(green > 6.0){
			    if(green < 12.0){
			        green = 5.0;

			    }
			}

            if(red > 6.0){
                if(red < 12.0){
                    red = 5.0;

                }
            }

            if(blue > 6.0){
                if(blue < 12.0){
                    blue = 5.0;

                }
            }

            fout << (unsigned char)(red) << (unsigned char)(green) << (unsigned char)(blue);
		}
	}

	fout.close();
}

int main(int argc, char *argv[])
{

    clock_t start = clock();
    long double time;

	Scene scene;
	int x, y;

	clear_framebuffer();

	// loop over the pixels

	PhongTextureMesh bp;

	bp.ambient.r = 0.4f;
	bp.ambient.g = 0.4f;
	bp.ambient.b = 0.4f;
	bp.diffuse.r = 0.4f;
	bp.diffuse.g = 0.4f;
	bp.diffuse.b = 0.4f;
	bp.specular.r = 0.4f;
	bp.specular.g = 0.4f;
	bp.specular.b = 0.4f;
	bp.power = 40.0f;

	bp.set_reflection(0.0);

	Phong sp2;

	sp2.ambient.r = 0.0f;
	sp2.ambient.g = 0.2f;
	sp2.ambient.b = 0.0f;
	sp2.diffuse.r = 0.0f;
	sp2.diffuse.g = 0.4f;
	sp2.diffuse.b = 0.0f;
	sp2.specular.r = 0.0f;
	sp2.specular.g = 0.4f;
	sp2.specular.b = 0.0f;
	sp2.power = 40.0f;

	sp2.set_reflection(0.2);

	// creates phong with texture
	PhongTexture tex;

	tex.ambient.r = 0.5f;
	tex.ambient.g = 0.5f;
	tex.ambient.b = 0.5f;
	tex.diffuse.r = 0.4f;
	tex.diffuse.g = 0.4f;
	tex.diffuse.b = 0.4f;
	tex.specular.r = 0.4f;
	tex.specular.g = 0.4f;
	tex.specular.b = 0.4f;
	tex.power = 40.0f;

	tex.set_reflection(0.2);

	DirectionalLight *dl;

	dl = new DirectionalLight(Vector(0.0f, 0.0f, 1.0f),Colour(0.5, 0.5, 0.5, 0.0));

	dl->scene = &scene;

	// point light
	PhysicalLight *pl = new PhysicalLight(Vector(0.0,0.0,0.0),Vector(0.0, 0.0, 1.0), Colour(0.5,0.5,0.5, 0.0));
	pl->scene = &scene;

	scene.light_list = dl;

	Transform transform(10.0f,  0.0f,  0.0f, 0.0f,
			     0.0f, 10.0f,  0.0f,-1.0f,
			     0.0f,  0.0f,-10.0f, 2.0f,
			     0.0f,  0.0f,  0.0f, 1.0f);

	const char* mesh_name = "..//bunny_zipper.kcply";

	PolyMesh *bunny = new PolyMesh((char *) mesh_name, transform);

	Sphere *sphere = new Sphere(Vertex(0,-1.5,3), 1.0);
	Sphere *sphere2 = new Sphere(Vertex(0, 1.5, 3), 1.0);
	//Sphere *sphere2 = new Sphere(Vertex(-0.168404, 0.101542, 2.01537), 0.778495*1.25);

	tex.sphere_centre.x = sphere->center.x;
	tex.sphere_centre.y = sphere->center.y;
	tex.sphere_centre.z = sphere->center.z;

	sphere->material = &tex;
	sphere2->material = &sp2;
	bunny->material = &bp;

	sphere->next = sphere2;
	scene.object_list = sphere;
	//scene.object_list = bunny;
	//scene.object_list = bunny;



	//bunny->make_bounding_sphere();

	// number of reflection levels to go down
	int levels = 4;

	// turns anti-aliasing on or off
	bool aa = false;

	// rate of AA - should be between 2 and 10 depending on AA level desired
	int aa_rate = 4;

	Ray root(Vertex(0.0f, 0.0f, 0.0f),Vector(0.0f, 0.0f, 0.0f));

	root.position.x = 0.0;
	root.position.y = 0.0;
	root.position.z = 0.0;
	root.position.w = 1.0;

	for(y = 0; y < YSIZE; y += 1)
	{
		cerr << "Line " << y+1 << " of " << (int)YSIZE << endl;
		long double py = (((long double)y / (long double)YSIZE) - 0.5)*-1.0; // 0.5 to -0.5, flipped y axis

		for (x = 0; x < XSIZE; x += 1)
		{
			//cerr << "Line " << y+1 << ", pixel " << x+1 << " of " << (int)YSIZE << endl;

		    // aa is a bool which indicates whether or not to use anti-aliasing
		    if(aa){

                Colour aa_accumulator;

                // iterate through the grid of rays for the pixel
                for(int i = 0; i < aa_rate; i += 1){
                	for(int j=0; j < aa_rate; j += 1){
                	    // get direction, which is moved slightly using a displacement - (i/aa_rate)/XSIZE
						long double px = (((long double)x / (long double)XSIZE) - 0.5); // -0.5 to 0.5
						root.direction.x = px + ((long double)i/aa_rate)/(long double)XSIZE;
						root.direction.y = py + ((long double)j/aa_rate)/(long double)YSIZE;
						root.direction.z = 0.5;

						double offset_x = ((double)i/aa_rate)/XSIZE;
						double offset_y = ((double)j/aa_rate)/YSIZE;

						root.direction.normalise();

						// runs raytracer again, noting the colour returned
						Colour return_col;
						scene.raytrace(root, levels, scene.object_list, scene.light_list, return_col);

						// adds returned colour to accumulator
						aa_accumulator.r += return_col.r;
						aa_accumulator.g += return_col.g;
						aa_accumulator.b += return_col.b;


						if(i==j==(int)aa_rate/2){
						    if(aa_accumulator.r+aa_accumulator.g+aa_accumulator.r == 0){
						        break;
						    }
						}


                	}
                }

                // averages out results using aa_rate^2
                framebuffer[y][x].r = aa_accumulator.r/(aa_rate*aa_rate);
                framebuffer[y][x].g = aa_accumulator.g/(aa_rate*aa_rate);
                framebuffer[y][x].b = aa_accumulator.b/(aa_rate*aa_rate);

			}else {
				long double px = (((long double) x / (long double) XSIZE) - 0.5f); // -0.5 to 0.5

				root.direction.x = px;
				root.direction.y = py;
				root.direction.z = 0.5f;

				root.direction.normalise();

				scene.raytrace(root, levels, scene.object_list, scene.light_list, framebuffer[y][x]);
			}

		}

   }

	time = (clock()-start)/ (long double) CLOCKS_PER_SEC;

	cerr << "Done in " << time << "seconds" << endl;

	write_framebuffer();

}
