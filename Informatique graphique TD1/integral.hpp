//
//  integral.hpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 27/01/2021.
//

#ifndef integral_hpp
#define integral_hpp

#include <stdio.h>

#include "scene.hpp"

class Integral
{
private:
    static double p_normal(double, double, double);
public:
    static Vector random_cos(Vector);
    static double integral4Dcos2(int);
    static Vector integralHemisphereScene(Scene& scene, Sphere sphere, Ray r, Vector N, Vector P, Vector color, int bounds);
};

#endif /* integral_hpp */
