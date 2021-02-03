//
//  sphere.hpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 27/01/2021.
//

#ifndef sphere_obj_hpp
#define sphere_obj_hpp

#include <stdio.h>

#include "vector_obj.hpp"
#include "ray_obj.hpp"

class Sphere
{
public:
    Vector O;
    double R;
    Vector rho;
    double reflexion;
    double transparancy;
    double n;
    double emmissivity;
    Sphere();
    Sphere(Vector o, double r, Vector _rho, double refl, double tr, double _n, double emmiss);
    bool intersect(Ray);
    bool intersect(Ray, Vector&, Vector&);
};

#endif /* sphere_hpp */
