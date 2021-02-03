//
//  light_obj.cpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 27/01/2021.
//

#include "light_obj.hpp"

#include "vector_obj.hpp"

Light::Light()
{
    position = 0;
    intensity = 0;
}

Light::Light( int pos, int intens)
{
    position = pos;
    intensity = intens;
}
