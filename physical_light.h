//
// Created by Keegan on 05/12/2018.
//

#pragma once

#include "light.h"

class PhysicalLight: public Light{

    Vector position;
    Vector direction;
    Colour colour;

public:
    PhysicalLight();
    PhysicalLight(Vector pos, Vector dir, Colour col);

    bool get_direction(Vertex &surface, Vector &dir);
    void get_intensity(Vertex &surface, Colour &intensity);

};
