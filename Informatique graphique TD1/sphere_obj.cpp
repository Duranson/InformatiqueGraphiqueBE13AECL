//
//  sphere.cpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 27/01/2021.
//

#include "sphere_obj.hpp"
#include "math.h"

#include "vector_obj.hpp"
#include "ray_obj.hpp"

Sphere::Sphere()
{
    O = Vector();
    R = 0.;
    rho = Vector();
    reflexion = 0;
};

Sphere::Sphere(Vector o, double r, Vector _rho, double refl, double tr, double _n)
{
    O = o;
    R = r;
    rho = _rho;
    reflexion = refl;
    transparancy = tr;
    n = _n;
};

bool Sphere::intersect(Ray r)
{
    double b = 2 * r.u.dot(r.C - O);
    double c = (r.C - O).sqrnorm() - R * R;
    double det = b * b - 4 * c;          // a = 1 because r.u is normalized
    return det >= 0;
}

bool Sphere::intersect(Ray r, Vector& P, Vector& N)
{
    double b = 2 * r.u.dot(r.C - O);
    double c = (r.C - O).sqrnorm() - R * R;
    double det = b * b - 4 * c;          // a = 1 because r.u is normalized
    if (det < 0) return false;
    
    double t;
    
    double sqrDelta = sqrt(det);
    double t2 = ( - b + sqrDelta) / 2;
    if (t2 < 0) return false;
    double t1 = ( - b - sqrDelta) / 2;
    if (t1 > 0)
    {
        t = t1;
    } else {
        t = t2;
    };
    P = r.C + r.u * t;
    N = (P - O);
    N.normalize();
    return true;
}
