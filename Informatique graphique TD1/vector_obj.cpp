//
//  vector_object.cpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 27/01/2021.
//

#include "vector_obj.hpp"
#include "math.h"

Vector::Vector()
{
    coord[0] = 0;
    coord[1] = 0;
    coord[2] = 0;
};

Vector::Vector(double x, double y, double z)
{
    coord[0] = x;
    coord[1] = y;
    coord[2] = z;
};

double Vector::sqrnorm()
{
    return coord[0] * coord[0] + coord[1] * coord[1] + coord[2] * coord[2];
};

Vector Vector::operator+(const Vector v)
{
    return Vector(coord[0] + v.coord[0], coord[1] + v.coord[1], coord[2] + v.coord[2]);
};

Vector Vector::operator-(const Vector v)
{
    return Vector(coord[0] - v.coord[0], coord[1] - v.coord[1], coord[2] - v.coord[2]);
};

Vector Vector::operator-()
{
    return Vector( - coord[0], - coord[1], - coord[2]);
};

Vector Vector::operator*(double a)
{
    return Vector(coord[0] * a, coord[1] * a, coord[2] * a);
};

Vector Vector::operator*(Vector v)
{
    return Vector(v[0] * coord[0], v[1] * coord[1], v[2] * coord[2]);
};

Vector Vector::operator/(double a)
{
    return Vector( coord[0] / a, coord[1] / a, coord[2] / a );
};

double Vector::dot(Vector b)
{
    return coord[0] * b[0] + coord[1] * b[1] + coord[2] * b[2];
};

void Vector::normalize()
{
    double norm = sqrt(sqrnorm());
    coord[0] = coord[0] / norm;
    coord[1] = coord[1] / norm;
    coord[2] = coord[2] / norm;
};

Vector Vector::cross(Vector v)
{
    return Vector(coord[1] * v[2] - coord[2] * v[1], coord[2] * v[0] - coord[0] * v[2], coord[0] * v[1] - coord[1] * v[0]);
}
