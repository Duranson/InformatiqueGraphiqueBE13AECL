//
//  integral.cpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 27/01/2021.
//

#include "integral.hpp"

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>

#include "math.h"

#include "scene.hpp"

#include <random>
static std::default_random_engine engine(10) ; // random seed = 10
static std::uniform_real_distribution<double> uniform(0, 1);

double Integral::p_normal(double x, double sigma, double center)
{
    return exp(- (x - center) * (x - center) / (2 * sigma * sigma)) / (sigma * sqrt(2 * M_PI));
}

Vector Integral::random_cos(Vector N)
{
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
            T1 = Vector( - N[2], 0, N[0]);
        }
        else
        {
            T1 = Vector( - N[1], N[0], 0);
        }
    }
    T1.normalize();
    
    Vector T2 = N.cross(T1);
    
    Vector up = N * z - T1 * x - T2 * y;
    
    return up;
}

Vector Integral::random_origin(double R, double z)
{
    double r = uniform(engine) * R;
    double theta = uniform(engine) * 2 * M_PI;
    
    double x = r * cos(theta);
    double y = r * sin(theta);
    
    return Vector(x,y,z);
}

double Integral::integral4Dcos2(int N_iter)
{
    double center = 0.;
    double sigma = 1.;
    double sum = 0.;
    for (int i = 0; i < N_iter; i++)
    {
        double u1 = uniform(engine);
        double u2 = uniform(engine);
        double u3 = uniform(engine);
        double u4 = uniform(engine);
        
        double xi = sigma * cos(2*M_PI*u1) * sqrt(-2 * log(u2));
        double pxi = Integral::p_normal(xi,sigma,center);
        
        double yi = sigma * sin(2*M_PI*u1) * sqrt(-2 * log(u2));
        double pyi = Integral::p_normal(yi,sigma,center);
        
        double zi = sigma * cos(2*M_PI*u3) * sqrt(-2 * log(u4));
        double pzi = Integral::p_normal(zi,sigma,center);
        
        double wi = sigma * sin(2*M_PI*u3) * sqrt(-2 * log(u4));
        double pwi = Integral::p_normal(wi,sigma,center);
        
        if (xi < M_PI/2 && xi > - M_PI / 2 && yi < M_PI/2 && yi > - M_PI / 2 && zi < M_PI/2 && zi > - M_PI / 2 && wi < M_PI/2 && wi > - M_PI / 2)
            sum += cos(xi + yi + zi + wi) * cos(xi + yi + zi + wi) / ( pxi * pyi * pzi * pwi);
    }
    std::cout << "Résultat de l'intégrale avec N = " << N_iter << " : " << sum / N_iter << ". Vraie valeur en 1D : " << 63 * M_PI / 256 << std::endl;
    return sum / N_iter;
}

Vector Integral::integralHemisphereScene(Scene& scene, Sphere sphere, Ray r, Vector N, Vector P, Vector color, int bounds)
{
    // Not used yet
    int N_iter = 1000;
    if (bounds > 0)
    {
        // Same as mirror with rho factor
        Vector up = r.u - N * r.u.dot(N) * 2.;
        up.normalize();
        double eps = 1e-4;
        Ray rp(P + up * eps, up);
        color = color + sphere.rho * scene.intersects(rp, bounds + 1, false);
        return color;
    }
    for (int i = 0; i < N_iter; i++)
    {
        Vector up = random_cos(N);
        
        double eps = 1e-3;
        Ray wi(up, P + up * eps);
        
        color = color + sphere.rho * scene.intersects(wi, bounds + 1, false);
    }
    return color;
}
