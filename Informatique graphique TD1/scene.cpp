//
//  vector.cpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 06/01/2021.
//

#include "scene.hpp"
#include "math.h"

#include "integral.hpp"

#include "vector_obj.hpp"
#include "ray_obj.hpp"
#include "sphere_obj.hpp"
#include "light_obj.hpp"

#include <random>
static std::default_random_engine engine(10) ; // random seed = 10
static std::uniform_real_distribution<double> uniform(0, 1);

double dot(Vector a, Vector b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
};

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
    int max_bounds = 5;
    
    if (bounds > max_bounds)
        // Too much bounds, need to escape this loop
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
    // Distance to the intersection
    double t = *std::min_element(distances.begin(),distances.end());
    if (t >= 1e4)
    {
        return Vector(0,0,0);
    }
    Vector color(0,0,0);
    // Sphere collided
    Sphere s = objects[i];
    Vector P = r.C + r.u * t;
    // Normal to the intersection point
    Vector N = (P - s.O);
    N.normalize();
    bool inside_the_colliding_sphere = ( N.dot(r.u) > 0); // Hope not
    // Calculate primary color
    /*if (is_shadowed(P, light.position))
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
    }*/
    
    if (s.emmissivity > 0)
    {
        return s.rho * s.emmissivity / (4 * M_PI * M_PI * s.R * s.R);;
    }
    
    // Add secondary illumination with recursivity
    
    double r1 = uniform(engine);
    double r2 = uniform(engine);
    
    double x = cos(2*M_PI*r1) * sqrt(1 - r2);
    double y = sin(2*M_PI*r1) * sqrt(1 - r2);
    double z = sqrt(r2);
    
    Vector T1;
    
    if ( N[0] < N[1] && N[0] < N[2] )
    {
        T1 = Vector(0, - N[2], N[1]);
    }
    else
    {
        if ( N[1] < N[0] && N[1] < N[2] )
        {
            T1 = Vector( N[2], 0, - N[0]);
        }
        else
        {
            T1 = Vector( - N[1], N[0], 0);
        }
    }
    
    Vector T2 = N.cross(T1);
    
    Vector up = N * z - T1 * x - T2 * y;
    
    double eps = 1e-3;
    Ray wi(P + up * eps, up);
    
    color = color + s.rho * intersects(wi, bounds + 1);
    
    // Add reflexion to color
    if (s.reflexion > 0.01)
    {
        Vector up = r.u - N * r.u.dot(N) * 2.;
        up.normalize();
        double eps = 1e-4;
        Ray rp(P + up * eps, up);
        color = ( color * (1 - s.reflexion)  + intersects(rp, bounds + 1) * s.reflexion );
    }
    // Add transparancy to color
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
