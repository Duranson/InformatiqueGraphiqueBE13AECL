//
//  light_obj.hpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 27/01/2021.
//

#ifndef light_obj_hpp
#define light_obj_hpp

#include <stdio.h>

#include "vector_obj.hpp"
#include "sphere_obj.hpp"

class Light
{
public:
    int position;
    double intensity;
    Light();
    Light(int list_position, int intensity);
};

#endif /* light_obj_hpp */
