//
//  vector.hpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 06/01/2021.
//

#ifndef objects_hpp
#define objects_hpp

#include <stdio.h>

class Vector
{
public:
    double coord[3];
    double sqrnorm();
    Vector(): coord() {}
    Vector(double,double,double);
    Vector operator+ (const Vector);
    Vector operator- (const Vector);
    double dot(const Vector);
    Vector operator* (double);
    Vector operator* (Vector);
    void normalize();
    double operator[](int i) const {return coord[i];} ;
    double &operator[](int i) {return coord[i];} ;
};

class Ray
{
public:
    Vector C,u;
    Ray(): C(), u() {}
    Ray(Vector, Vector);
};

class Sphere
{
public:
    Vector O;
    double R;
    Sphere(): O(), R() {}
    Sphere(Vector, double);
    bool intersect(Ray);
};

#endif /* vector_hpp */
