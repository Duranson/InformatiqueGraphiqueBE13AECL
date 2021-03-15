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
    objects = std::vector<Object*>();
    light = l;
}

void Scene::addSphere(Object* o)
{
    objects.push_back(o);
}

bool Scene::is_shadowed(Vector P, Vector L)
{
    Vector u = L - P;
    double dist = sqrt(u.sqrnorm());
    
    u.normalize();
    Ray r(P, u);
    
    Vector N;
    Vector intersectionColor;
    for (Object* s : objects)
    {
        double t = s->intersect(r,N,intersectionColor);
        double eps = 0.01;
        if (t > eps && t < (1 - eps) * dist)
            return true;
    }
    return false;
}

Vector Scene::intersects(Ray r, int bounds, bool last_diffuse)
{
    int max_bounds = 5;
    
    if (bounds > max_bounds)
        // Too much bounds, need to escape this loop
        return Vector(0,0,0);
    std::vector<double> distances = {};
    std::vector<Vector> normals = {};
    std::vector<Vector> colors = {};
    for (Object* s : objects)
    {
        Vector N;
        Vector intersectionColor;
        double t = s->intersect(r, N, intersectionColor);
        distances.push_back(t);
        normals.push_back(N);
        colors.push_back(intersectionColor);
    }
    int i = std::min_element(distances.begin(), distances.end()) - distances.begin();
    // Distance to the intersection
    double t = *std::min_element(distances.begin(),distances.end());
    if (t >= 1e5)
    {
        return Vector(0,0,0);
    }
    Vector color(0,0,0);
    // Sphere collided
    Object* s = objects[i];
    
    if (i == light.position)
    {
        if (!last_diffuse)
            return s->rho;
        return Vector(0,0,0);
    }
    
    Vector P = r.C + r.u * t;
    // Normal to the intersection point (On suppose qu'on l'a déjà par la fonction intersects)
    Vector N = normals[i];
    Vector intersectionColor = colors[i];
    N.normalize();
    bool inside_the_colliding_sphere = ( N.dot(r.u) > 0);
    
    // Calculate primary color
    Vector lightOrigin = (dynamic_cast<Sphere*>(objects[light.position])->O);
    double lightRay = (dynamic_cast<Sphere*>(objects[light.position])->R);
    Vector PL = lightOrigin - P;
    PL.normalize();
    Vector w = Integral::random_cos(-PL);
    Vector xprime = w * lightRay + lightOrigin;
    Vector Pxprime = xprime - P;
    double d = sqrt(Pxprime.sqrnorm());
    Pxprime.normalize();
    
    if (is_shadowed(P, xprime))
    {
        color = Vector(0,0,0);
    }
    else
    {
        double r2 = (dynamic_cast<Sphere*>(objects[light.position])->R);
        double fact = light.intensity / (4 * M_PI * M_PI * r2 * r2);
        double proba = std::max(- PL.dot(w), 0.) / (M_PI * r2 * r2);
        double Jacob = std::max(w.dot(- Pxprime), 0.) / (d * d);
        color = intersectionColor / M_PI * (std::max(N.dot(PL) , 0.) * (Jacob / proba)  * fact);
    }
    
    // Add secondary illumination with recursivity
    
    Vector up = Integral::random_cos(N);
    double eps = 1e-3;
    Ray wi(P + up * eps, up);
    
    color = color + intersectionColor * intersects(wi, bounds + 1, true);
    
    // Add reflexion to color
    if (s->reflexion > 0.01)
    {
        Vector up = r.u - N * r.u.dot(N) * 2.;
        up.normalize();
        double eps = 1e-4;
        Ray rp(P + up * eps, up);
        color = ( color * (1 - s->reflexion)  + intersects(rp, bounds + 1, false) * s->reflexion );
    }
    // Add transparancy to color
    if (s->transparancy > 0.01)
    {
        double n1 = 1;
        double n2 = s->n;
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
        color = ( color * (1 - s->transparancy)  + intersects(rp, bounds + 1, false) * s->transparancy );
    }
    return color;
}
