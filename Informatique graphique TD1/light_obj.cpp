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
    position = Vector();
    intensity = 0;
}

Light::Light( Vector pos, int intens)
{
    position = pos;
    intensity = intens;
}
