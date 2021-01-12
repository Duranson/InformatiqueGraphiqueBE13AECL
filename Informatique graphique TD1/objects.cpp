//
//  vector.cpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 06/01/2021.
//

#include "objects.hpp"
#include "math.h"

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

double dot(Vector a, Vector b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
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

Sphere::Sphere()
{
    O = Vector();
    R = 0.;
    rho = Vector();
};

Sphere::Sphere(Vector o, double r, Vector _rho)
{
    O = o;
    R = r;
    rho = _rho;
};

Ray::Ray(Vector _C, Vector _u)
{
    C = _C;
    u = _u;
};

bool Sphere::intersect(Ray r)
{
    double b = 2 * dot(r.u, r.C - O);
    double c = (r.C - O).sqrnorm() - R * R;
    double det = b * b - 4 * c;          // a = 1 because r.u is normalized
    return det >= 0;
}

bool Sphere::intersect(Ray r, Vector& P, Vector& N)
{
    double b = 2 * dot(r.u, r.C - O);
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

Scene::Scene(Light l)
{
    objects = std::vector<Sphere>();
    light = l;
}

void Scene::addSphere(Sphere S)
{
    objects.push_back(S);
}

Vector Scene::intersects(Ray r)
{
    // Adapter le code suivant à la structure de Scene -> itérer sur les sphères et prendre les P et N constants (source de lumiere)
    std::vector<double> distances = {};
    for (Sphere& s : objects)
    {
        double b = 2 * dot(r.u, r.C - s.O);
        double c = (r.C - s.O).sqrnorm() - s.R * s.R;
        double det = b * b - 4 * c;          // a = 1 because r.u is normalized
        double t;
        if (det >= 0)
        {
            double sqrDelta = sqrt(det);
            double t2 = ( - b + sqrDelta) / 2;
            if (t2 < 0) t = 1e10;
            else
            {
                double t1 = ( - b - sqrDelta) / 2;
                if (t1 > 0)
                {
                    t = t1;
                } else {
                    t = t2;
                };
            }
        } else {
            t = 1e10;
        }
        distances.push_back(t);
    }
    int i = std::min_element(distances.begin(), distances.end()) - distances.begin();
    double t = *std::min_element(distances.begin(),distances.end());
    if (t >= 1e4)
    {
        return Vector(0,0,0);
    }
    Sphere s = objects[i];
    Vector P = r.C + r.u * t;
    Vector N = (P - s.O);
    N.normalize();
    Vector color(0,0,0);
    Vector PL = light.position - P;
    double d = PL.sqrnorm();
    PL.normalize();
    double fact = light.intensity / (4 * M_PI * d);
    color = s.rho / M_PI * (std::max(N.dot(PL) , 0.) * fact);
    return color;
}

Light::Light()
{
    position = Vector();
    intensity = 0;
}

Light::Light( Vector pos, int intens)
{
    position = pos;
    intensity = intens;
}

Vector::Vector()
{
    coord[0] = 0;
    coord[1] = 0;
    coord[2] = 0;
}
