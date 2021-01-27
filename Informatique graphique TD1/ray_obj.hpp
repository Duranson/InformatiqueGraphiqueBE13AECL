//
//  ray_obj.hpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 27/01/2021.
//

#ifndef ray_obj_hpp
#define ray_obj_hpp

#include <stdio.h>

#include "vector_obj.hpp"

class Ray
{
public:
    Vector C,u;
    Ray(): C(), u() {}
    Ray(Vector, Vector);
};

#endif /* ray_obj_hpp */
