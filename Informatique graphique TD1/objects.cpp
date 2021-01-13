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

bool Scene::is_shadowed(Vector P, Vector L)
{
    Vector u = L - P;
    double dist = sqrt(u.sqrnorm());
    
    u.normalize();
    Ray r(P, u);
    
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
        double eps = 0.01;
        if (t > eps && t < (1 - eps) * dist)
            return true;
    }
    return false;
}

Vector Scene::intersects(Ray r, int bounds)
{
    // Adapter le code suivant à la structure de Scene -> itérer sur les sphères et prendre les P et N constants (source de lumiere)
    if (bounds > 5)
        return Vector(0,0,0);
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
    Vector color(0,0,0);
    Sphere s = objects[i];
    Vector P = r.C + r.u * t;
    Vector N = (P - s.O);
    N.normalize();
    bool inside_the_colliding_sphere = ( N.dot(r.u) > 0);
    if (is_shadowed(P, light.position))
    {
        color = Vector(0,0,0);
    }
    else
    {
        Vector PL = light.position - P;
        double d = PL.sqrnorm();
        PL.normalize();
        double fact = light.intensity / (4 * M_PI * d);
        color = s.rho / M_PI * (std::max(N.dot(PL) , 0.) * fact);
    }
    if (s.reflexion > 0.01)
    {
        Vector up = r.u - N * r.u.dot(N) * 2.;
        up.normalize();
        double eps = 1e-4;
        Ray rp(P + up * eps, up);
        color = ( color * (1 - s.reflexion)  + intersects(rp, bounds + 1) * s.reflexion );
    }
    if (s.transparancy > 0.01)
    {
        double n1 = 1;
        double n2 = s.n;
        if (inside_the_colliding_sphere)
        {
            N = - N;
            std::swap(n1,n2);
        }
        Vector tt = (r.u -  N * r.u.dot(N)) * (n1 / n2)  ;
        Vector tn = - N * sqrt(std::max(0., 1 - (n1 / n2) * (n1 / n2) * (1 - r.u.dot(N) * r.u.dot(N))));
        Vector refracted = tt + tn;
        
        double eps = 1e-4;
        Ray rp(P + refracted * eps, refracted);
        color = ( color * (1 - s.transparancy)  + intersects(rp, bounds + 1) * s.transparancy );
    }
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
