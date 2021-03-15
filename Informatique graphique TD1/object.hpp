//
//  object.hpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 10/02/2021.
//

#ifndef object_hpp
#define object_hpp

#include <stdio.h>
#include "ray_obj.hpp"

class Object
{
public:
    Object() {};
    virtual double intersect(Ray r, Vector &N, Vector &color) = 0;
    
    Vector rho;
    double transparancy;
    double reflexion;
    double n;
};

#endif /* object_hpp */
