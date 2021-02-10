//
//  mesh_reader.cpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 10/02/2021.
//

#include "mesh_reader.hpp"

double TriangleMesh::intersect(Ray r, Vector& N)
{
    double t = 1E9;
    Vector P;
    
    if (!bBox.intersect(r))
        return t;
    
    for (TriangleIndices& indice : indices)
    {
        Vector &A = vertices[indice.vtxi];
        Vector &B = vertices[indice.vtxj];
        Vector &C = vertices[indice.vtxk];
        
        Vector e1 = B - A;
        Vector e2 = C - A;
        Vector normal = e1.cross(e2);
        
        Vector AO = r.C - A;
        Vector AOvu = AO.cross(r.u);
        
        double UvN = r.u.dot(normal);
        double beta = - e2.dot(AOvu) / UvN;
        double gamma = e1.dot(AOvu) / UvN;
        double t_intersect = - AO.dot(normal) / UvN;
        
        if (beta >= 0 && beta + gamma < 1 && gamma >= 0 && t > 0)
        {
            if (t_intersect < t)
            {
                t = t_intersect;
                P = r.C + r.u * t;
                N = normal;
            }
        }
    }
    
    return t;
}
