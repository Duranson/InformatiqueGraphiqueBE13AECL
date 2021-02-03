//
//  vector.hpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 06/01/2021.
//

#ifndef scene_hpp
#define scene_hpp

//#define _CRT_SECURE_NO_WARNINGS 1
#include <vector>

#include <stdio.h>

#include <iostream>
#include <cmath>

#include "vector_obj.hpp"
#include "ray_obj.hpp"
#include "sphere_obj.hpp"
#include "light_obj.hpp"

class Scene
{
public:
    std::vector<Sphere> objects;
    Light light;
    Vector intersects(Ray, int bounds, bool diffus);    // returns the color
    bool is_shadowed(Vector, Vector);
    void addSphere(Sphere S);
    Scene();
    Scene(Light);
    Vector getColor(Ray,int);
};

#endif /* vector_hpp */
