//
//  mesh_reader.cpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 10/02/2021.
//

#include "mesh_reader.hpp"
#include <list>

double TriangleMesh::intersect(Ray r, Vector& N)
{
    double t = 1E9;
    Vector P;
    
    if (!BVH->bBox.intersect(r))
        return t;
    std::list<Noeud*> l;
    l.push_back(BVH);
    
    while(!l.empty())
    {
        Noeud* courant = l.front();
        l.pop_front();
        
        if (courant->fg)
        {
            if (courant->fg->bBox.intersect(r)){
                l.push_back(courant->fg);
            }
            if (courant->fd->bBox.intersect(r)){
                l.push_back(courant->fd);
            }
        } else {
            // Feuille de l'arbre
            for (int i = courant->debut; i < courant->fin; i++)
            {
                Vector &A = vertices[indices[i].vtxi];
                Vector &B = vertices[indices[i].vtxj];
                Vector &C = vertices[indices[i].vtxk];
                
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
        }
        
    }
    return t;
}
