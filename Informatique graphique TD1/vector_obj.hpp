//
//  vector_object.hpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 27/01/2021.
//

#ifndef vector_obj_hpp
#define vector_obj_hpp

#include <stdio.h>

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

#endif /* vector_object_hpp */
