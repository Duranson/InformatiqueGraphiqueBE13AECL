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
    object = Sphere(Vector(), 15, Vector(1,1,1), 0., 0., 1., 0);
}

Light::Light( Vector pos, double ray, int intens)
{
    object = Sphere(pos, ray, Vector(1,1,1), 0., 0., 1., intens);
}
