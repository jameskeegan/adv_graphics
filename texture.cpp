//
// Created by kegof on 06/12/2018.
//
#define _USE_MATH_DEFINES

#include "texture.h"
#include <cmath>

void Texture::compute_base_colour(Colour &result) {
    //Material::compute_base_colour(result);

    result.r = 0.0;
    result.g = 0.0;
    result.b = 0.0;

}

void Texture::compute_light_colour(Vector &viewer, Vector &normal, Vector &ldir, Vertex &is, Colour &result) {

    long double r = 1.0;

    Vertex centre(0.0, 0.0, 1.5);

    long double x = is.x - centre.x;
    long double y = is.y - centre.y;
    long double z = is.z - centre.z;

    long double u = atan2(z,x) / (2*M_PI) + 0.5;
    //long double v = y * 0.5 + 0.5;
    long double v = 0.5 - asin(y)/M_PI;

    u *= 255;
    v *= 255;

    int u_int = (int) u;
    int v_int = (int) v;

    long double inside = (is.z-1.5)/r;
    long double acos_inside = acos(inside);


    //cerr << "acos_inside = " << acos_inside << endl;


    long double s = acos_inside/M_PI;

    long double t_inside = x/(r*sin(s*M_PI));

    if ((-1 >= t_inside) || (1 <= t_inside)){
        //cerr << "t_inside = " << t_inside << endl;
    }

    if(t_inside <= -1){
        t_inside = -0.99;
    }else if(t_inside >= 1){
        t_inside = 0.99;
    }

    long double t = acos(t_inside)/(2*M_PI);

    if(t < 0){
        cerr << t_inside << endl;
    }

    //cout << is.z << endl;
    //cout << r << endl;

    //long double s = acos(0.5)/M_PI;
    //long double t = acos(0.5)/M_PI;


    // scale to size of image
    s = ceil(s*255);
    t = ceil(t*255);

    int s_int = (int) s;
    int t_int = (int) t;


    if(s_int > 255){
        cerr << "too big" << endl;
    }

    if(s_int < 0){
        cerr << "too big" << endl;
    }

    if(t_int > 255){
        cerr << "t_int too big" << endl;
        cerr << t_int << endl;
        t_int = 255;
    }

    if(t_int < 0){
        cerr << "t_int too small" << endl;
        cerr << t_int << endl;
        t_int = 0;
    }



    //cerr << "s_int = " << s_int << ", t_int  = " << t_int << endl;
    //result.r = perlin->p_noise[v_int][u_int].r;
    result.r = 0.0;
    result.g = perlin->p_noise[v_int][u_int].g;
    //result.g = 0.0;
    //result.b = perlin->p_noise[v_int][u_int].b;
    result.b = 0.0;

    if(result.r > 255){
        cerr << "result.r = " << result.r << endl;
    }

}
