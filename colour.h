/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

// Colour is a class to store and maniplulate an rgba colour.

#pragma once

class Colour {
public:
	double r;
    double g;
    double b;
    double a;

	Colour()
	{
		r = 0.0;
		g = 0.0;
		b = 0.0;
		a = 1.0;
	}

	Colour(double pr, double pg, double pb, double pa)
	{
		r = pr;
		g = pg;
		b = pb;
		a = pa;
	}

	void scale(Colour &scaling)
	{
		r *= scaling.r;
		g *= scaling.g;
		b *= scaling.b;
		a *= scaling.a;
	}

	void add(Colour &adjust)
	{
		r += adjust.r;
		g += adjust.g;
		b += adjust.b;
	}
};
