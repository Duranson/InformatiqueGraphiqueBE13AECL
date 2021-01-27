//
//  ray_obj.cpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 27/01/2021.
//

#include "ray_obj.hpp"

#include "vector_obj.hpp"

Ray::Ray(Vector _C, Vector _u)
{
    C = _C;
    u = _u;
};
