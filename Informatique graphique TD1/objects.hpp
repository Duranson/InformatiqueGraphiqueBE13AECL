//
//  vector.hpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 06/01/2021.
//

#ifndef objects_hpp
#define objects_hpp

//#define _CRT_SECURE_NO_WARNINGS 1
#include <vector>

#include <stdio.h>

#include <iostream>

class Vector
{
public:
    double coord[3];
    double sqrnorm();
    Vector();
    Vector(double,double,double);
    Vector operator+ (const Vector);
    Vector operator- (const Vector);
    Vector operator- ();
    double dot(const Vector);
    Vector operator* (double);
    Vector operator* (Vector);
    Vector operator/ (double);
    Vector cross(Vector);
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
    Vector rho;
    double reflexion;
    double transparancy;
    double n;
    Sphere();
    Sphere(Vector o, double r, Vector _rho, double refl, double tr, double _n);
    bool intersect(Ray);
    bool intersect(Ray, Vector&, Vector&);
};

class Light
{
public:
    Vector position;
    int intensity;
    Light();
    Light(Vector position, int intensity);
};

class Scene
{
public:
    std::vector<Sphere> objects;
    Light light;
    Vector intersects(Ray, int bounds);    // returns the color
    bool is_shadowed(Vector, Vector);
    void addSphere(Sphere S);
    Scene();
    Scene(Light);
    Vector getColor(Ray,int);
};

#endif /* vector_hpp */
