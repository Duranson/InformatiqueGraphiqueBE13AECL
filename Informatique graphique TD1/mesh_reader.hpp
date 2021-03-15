//
//  mesh_reader.hpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 10/02/2021.
//

#ifndef mesh_reader_hpp
#define mesh_reader_hpp

#include <string>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <math.h>

#include "ray_obj.hpp"
#include "vector_obj.hpp"
#include "object.hpp"

class BoundingBox {
public:
    bool intersect(Ray r)
    {
        double t1x = (mini[0] - r.C[0]) / r.u[0];
        double t2x = (maxi[0] - r.C[0]) / r.u[0];
        double t1Min = std::min(t1x,t2x);
        double t1Max = std::max(t1x,t2x);
        
        double t1y = (mini[1] - r.C[1]) / r.u[1];
        double t2y = (maxi[1] - r.C[1]) / r.u[1];
        double t2Min = std::min(t1y,t2y);
        double t2Max = std::max(t1y,t2y);
        
        double t1z = (mini[2] - r.C[2]) / r.u[2];
        double t2z = (maxi[2] - r.C[2]) / r.u[2];
        double t3Min = std::min(t1z,t2z);
        double t3Max = std::max(t1z,t2z);
        
        //return true;
        return std::max(t1Min, std::max(t2Min, t3Min)) < std::min(t1Max,std::min(t2Max,t3Max));
    }
    Vector mini;
    Vector maxi;
};

class Noeud {
public:
    Noeud *fg, *fd;
    BoundingBox bBox;
    int debut, fin;
};

class TriangleIndices {
public:
    TriangleIndices(int vtxi = -1, int vtxj = -1, int vtxk = -1, int ni = -1, int nj = -1, int nk = -1, int uvi = -1, int uvj = -1, int uvk = -1, int group = -1, bool added = false) : vtxi(vtxi), vtxj(vtxj), vtxk(vtxk), uvi(uvi), uvj(uvj), uvk(uvk), ni(ni), nj(nj), nk(nk), group(group) {
    };
    int vtxi, vtxj, vtxk; // indices within the vertex coordinates array
    int uvi, uvj, uvk;  // indices within the uv coordinates array
    int ni, nj, nk;  // indices within the normals array
    int group;       // face group
};
 
 
class TriangleMesh : public Object {
public:
  ~TriangleMesh() {}
    TriangleMesh(Vector _rho) {
        this->rho = _rho;
        this->transparancy = 0.;
        this->reflexion = 0.;
        this->n = 1.;
    };
    
    void readOBJ(const char* obj);
    
    void loadTexture(const char* filename);
    
    virtual double intersect(Ray r, Vector& N, Vector &color);
    
    std::vector<TriangleIndices> indices;
    std::vector<Vector> vertices;
    std::vector<Vector> normals;
    std::vector<Vector> uvs;
    std::vector<Vector> vertexcolors;
    
    std::vector<unsigned char *> textures;
    std::vector<int> Wtex;
    std::vector<int> Htex;
    
    Noeud* BVH = new Noeud;
    
    // BoundingBox bBox;
    
    void buildBVH(Noeud* n, int debut, int fin)
    {
        n->bBox = buildBBox(debut, fin);
        n->debut = debut;
        n->fin = fin;
        Vector diag = n->bBox.maxi - n->bBox.mini;
        int dim;
        if (diag[0] >= diag[1] && diag [0] >= diag[2])
            dim = 0;
        else{
            if (diag[1] >= diag[0] && diag [1] >= diag[2])
                dim = 1;
            else
                dim = 2;
        }
        double milieu = (n->bBox.mini[dim] + n->bBox.maxi[dim]) / 2;
        int indice_pivot = n->debut;
        for (int i = n->debut; i < n->fin; i++){
            double milieu_triangle = ( vertices[indices[i].vtxi][dim] + vertices[indices[i].vtxj][dim] + vertices[indices[i].vtxk][dim] ) / 3;
            if (milieu_triangle < milieu)
            {
                std::swap(indices[i], indices[indice_pivot]);
                indice_pivot++;
            }
        }
        
        n->fg = nullptr;
        n->fd = nullptr;
        
        if (indice_pivot == debut || indice_pivot == fin || ( fin - debut < 5))
            return;
        
        n->fg = new Noeud;
        n->fd = new Noeud;
        
        buildBVH(n->fg, n->debut, indice_pivot);
        buildBVH(n->fd, indice_pivot, n->fin);
    }
    
    BoundingBox buildBBox(int debut, int fin) // indices de triangles
    {
        BoundingBox b;
        b.mini = {1E6,1E6,1E6};
        b.maxi = {-1E6,-1E6,-1E6};
        for (int j = debut; j < fin; j++)
        {
            for (int i = 0; i < 3; i++)
            {
                // Itération sur chaque composante
                b.mini[i] = std::min(b.mini[i], vertices[indices[j].vtxi][i]);
                b.maxi[i] = std::max(b.maxi[i], vertices[indices[j].vtxi][i]);
                b.mini[i] = std::min(b.mini[i], vertices[indices[j].vtxj][i]);
                b.maxi[i] = std::max(b.maxi[i], vertices[indices[j].vtxj][i]);
                b.mini[i] = std::min(b.mini[i], vertices[indices[j].vtxk][i]);
                b.maxi[i] = std::max(b.maxi[i], vertices[indices[j].vtxk][i]);
            }
        }
        return b;
    }
    
    void move(int x, int y, int z)
    {
        for (Vector& v : vertices)
            v = v + Vector(x, y, z);
        BVH = new Noeud;
        buildBVH(BVH, 0, (int) indices.size());
    }
    
    void scale(double s)
    {
        for (Vector& v : vertices)
            v = v * s;
        BVH = new Noeud;
        buildBVH(BVH, 0, (int) indices.size());
    }
    
    void rotate(int type)
    {
        switch (type)
        {
        case 0:
            for (Vector& v : vertices)
            {
                std::swap(v[2], v[1]);
                v[2] = -v[2];
            }
            for (Vector& n : normals)
            {
                std::swap(n[2], n[1]);
                n[2] = -n[2];
            }
            BVH = new Noeud;
            buildBVH(BVH, 0, (int) indices.size());
            break;
        case 1:
            for (Vector& v : vertices)
            {
                std::swap(v[2], v[0]);
                //v[2] = -v[2];
            }
            /*for (Vector& n : normals)
            {
                std::swap(n[3], n[1]);
                n[3] = -n[3];
            }*/
            BVH = new Noeud;
            buildBVH(BVH, 0, (int) indices.size());
            break;
        }
        
    }
};

#endif /* mesh_reader_hpp */
