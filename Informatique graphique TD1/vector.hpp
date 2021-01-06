//
//  vector.hpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 06/01/2021.
//

#ifndef vector_hpp
#define vector_hpp

#include <stdio.h>

class Vector
{
public:
    double coord[3];
    double sqrnorm();
    Vector(): coord() {}
    Vector operator+ (const Vector);
    double dot(const Vector, const Vector);
};

class Sphere
{
public:
    Vector O;
    double R;
};

class Ray
{
public:
    Vector C,u;
};

#endif /* vector_hpp */
