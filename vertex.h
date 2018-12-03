/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

// Vertex is a class to store and maniplulate 3D vertices.

#pragma once

class Vertex {
public:
	double x;
	double y;
	double z;
	double w;

	Vertex()
	{
		x = 0.0;
		y = 0.0;
		z = 0.0;
		w = 1.0;
	}

	Vertex(double px, double py, double pz, double pw)
	{
		x = px;
		y = py;
		z = pz;
		w = pw;
	}

	Vertex(double px, double py, double pz)
	{
		x = px;
		y = py;
		z = pz;
		w = 1.0;
	}
};
