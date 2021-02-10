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
    
    void readOBJ(const char* obj) {
 
        char matfile[255];
        char grp[255];
 
        FILE* f;
        f = fopen(obj, "r");
        int curGroup = -1;
        while (!feof(f)) {
            char line[255];
            if (!fgets(line, 255, f)) break;
 
            std::string linetrim(line);
            linetrim.erase(linetrim.find_last_not_of(" \r\t") + 1);
            strcpy(line, linetrim.c_str());
 
            if (line[0] == 'u' && line[1] == 's') {
                sscanf(line, "usemtl %[^\n]\n", grp);
                curGroup++;
            }
 
            if (line[0] == 'v' && line[1] == ' ') {
                Vector vec;
 
                Vector col;
                if (sscanf(line, "v %lf %lf %lf %lf %lf %lf\n", &vec[0], &vec[1], &vec[2], &col[0], &col[1], &col[2]) == 6) {
                    col[0] = std::min(1., std::max(0., col[0]));
                    col[1] = std::min(1., std::max(0., col[1]));
                    col[2] = std::min(1., std::max(0., col[2]));
 
                    vertices.push_back(vec);
                    vertexcolors.push_back(col);
 
                } else {
                    sscanf(line, "v %lf %lf %lf\n", &vec[0], &vec[1], &vec[2]);
                    vertices.push_back(vec);
                }
            }
            if (line[0] == 'v' && line[1] == 'n') {
                Vector vec;
                sscanf(line, "vn %lf %lf %lf\n", &vec[0], &vec[1], &vec[2]);
                normals.push_back(vec);
            }
            if (line[0] == 'v' && line[1] == 't') {
                Vector vec;
                sscanf(line, "vt %lf %lf\n", &vec[0], &vec[1]);
                uvs.push_back(vec);
            }
            if (line[0] == 'f') {
                TriangleIndices t;
                int i0, i1, i2, i3;
                int j0, j1, j2, j3;
                int k0, k1, k2, k3;
                int nn;
                t.group = curGroup;
 
                char* consumedline = line + 1;
                int offset;
 
                nn = sscanf(consumedline, "%u/%u/%u %u/%u/%u %u/%u/%u%n", &i0, &j0, &k0, &i1, &j1, &k1, &i2, &j2, &k2, &offset);
                if (nn == 9) {
                    if (i0 < 0) t.vtxi = (int) vertices.size() + i0; else t.vtxi = (int) i0 - 1;
                    if (i1 < 0) t.vtxj = (int) vertices.size() + i1; else t.vtxj = (int) i1 - 1;
                    if (i2 < 0) t.vtxk = (int) vertices.size() + i2; else t.vtxk = (int) i2 - 1;
                    if (j0 < 0) t.uvi = (int) uvs.size() + j0; else   t.uvi = (int) j0 - 1;
                    if (j1 < 0) t.uvj = (int) uvs.size() + j1; else   t.uvj = (int) j1 - 1;
                    if (j2 < 0) t.uvk = (int) uvs.size() + j2; else   t.uvk = (int) j2 - 1;
                    if (k0 < 0) t.ni = (int) normals.size() + k0; else    t.ni = (int) k0 - 1;
                    if (k1 < 0) t.nj = (int) normals.size() + k1; else    t.nj = (int) k1 - 1;
                    if (k2 < 0) t.nk = (int) normals.size() + k2; else    t.nk = (int) k2 - 1;
                    indices.push_back(t);
                } else {
                    nn = sscanf(consumedline, "%u/%u %u/%u %u/%u%n", &i0, &j0, &i1, &j1, &i2, &j2, &offset);
                    if (nn == 6) {
                        if (i0 < 0) t.vtxi = (int) vertices.size() + i0; else t.vtxi = (int) i0 - 1;
                        if (i1 < 0) t.vtxj = (int) vertices.size() + i1; else t.vtxj = (int) i1 - 1;
                        if (i2 < 0) t.vtxk = (int) vertices.size() + i2; else t.vtxk = (int) i2 - 1;
                        if (j0 < 0) t.uvi = (int) uvs.size() + j0; else   t.uvi = (int) j0 - 1;
                        if (j1 < 0) t.uvj = (int) uvs.size() + j1; else   t.uvj = (int) j1 - 1;
                        if (j2 < 0) t.uvk = (int) uvs.size() + j2; else   t.uvk = (int) j2 - 1;
                        indices.push_back(t);
                    } else {
                        nn = sscanf(consumedline, "%u %u %u%n", &i0, &i1, &i2, &offset);
                        if (nn == 3) {
                            if (i0 < 0) t.vtxi = (int) vertices.size() + i0; else t.vtxi = (int) i0 - 1;
                            if (i1 < 0) t.vtxj = (int) vertices.size() + i1; else t.vtxj = (int) i1 - 1;
                            if (i2 < 0) t.vtxk = (int) vertices.size() + i2; else t.vtxk = (int) i2 - 1;
                            indices.push_back(t);
                        } else {
                            nn = sscanf(consumedline, "%u//%u %u//%u %u//%u%n", &i0, &k0, &i1, &k1, &i2, &k2, &offset);
                            if (i0 < 0) t.vtxi = (int) vertices.size() + i0; else t.vtxi = (int) i0 - 1;
                            if (i1 < 0) t.vtxj = (int) vertices.size() + i1; else t.vtxj = (int) i1 - 1;
                            if (i2 < 0) t.vtxk = (int) vertices.size() + i2; else t.vtxk = (int) i2 - 1;
                            if (k0 < 0) t.ni = (int) normals.size() + k0; else    t.ni = (int) k0 - 1;
                            if (k1 < 0) t.nj = (int) normals.size() + k1; else    t.nj = (int) k1 - 1;
                            if (k2 < 0) t.nk = (int) normals.size() + k2; else    t.nk = (int) k2 - 1;
                            indices.push_back(t);
                        }
                    }
                }
 
                consumedline = consumedline + offset;
 
                while (true) {
                    if (consumedline[0] == '\n') break;
                    if (consumedline[0] == '\0') break;
                    nn = sscanf(consumedline, "%u/%u/%u%n", &i3, &j3, &k3, &offset);
                    TriangleIndices t2;
                    t2.group = curGroup;
                    if (nn == 3) {
                        if (i0 < 0) t2.vtxi = (int) vertices.size() + i0; else    t2.vtxi = (int) i0 - 1;
                        if (i2 < 0) t2.vtxj = (int) vertices.size() + i2; else    t2.vtxj = (int) i2 - 1;
                        if (i3 < 0) t2.vtxk = (int) vertices.size() + i3; else    t2.vtxk = (int) i3 - 1;
                        if (j0 < 0) t2.uvi = (int) uvs.size() + j0; else  t2.uvi = (int) j0 - 1;
                        if (j2 < 0) t2.uvj = (int) uvs.size() + j2; else  t2.uvj = (int) j2 - 1;
                        if (j3 < 0) t2.uvk = (int) uvs.size() + j3; else  t2.uvk = (int) j3 - 1;
                        if (k0 < 0) t2.ni = (int) normals.size() + k0; else   t2.ni = (int) k0 - 1;
                        if (k2 < 0) t2.nj = (int) normals.size() + k2; else   t2.nj = (int) k2 - 1;
                        if (k3 < 0) t2.nk = (int) normals.size() + k3; else   t2.nk = (int) k3 - 1;
                        indices.push_back(t2);
                        consumedline = consumedline + offset;
                        i2 = i3;
                        j2 = j3;
                        k2 = k3;
                    } else {
                        nn = sscanf(consumedline, "%u/%u%n", &i3, &j3, &offset);
                        if (nn == 2) {
                            if (i0 < 0) t2.vtxi = (int) vertices.size() + i0; else    t2.vtxi = (int) i0 - 1;
                            if (i2 < 0) t2.vtxj = (int) vertices.size() + i2; else    t2.vtxj = (int) i2 - 1;
                            if (i3 < 0) t2.vtxk = (int) vertices.size() + i3; else    t2.vtxk = (int) i3 - 1;
                            if (j0 < 0) t2.uvi = (int) uvs.size() + j0; else  t2.uvi = (int) j0 - 1;
                            if (j2 < 0) t2.uvj = (int) uvs.size() + j2; else  t2.uvj = (int) j2 - 1;
                            if (j3 < 0) t2.uvk = (int) uvs.size() + j3; else  t2.uvk = (int) j3 - 1;
                            consumedline = consumedline + offset;
                            i2 = i3;
                            j2 = j3;
                            indices.push_back(t2);
                        } else {
                            nn = sscanf(consumedline, "%u//%u%n", &i3, &k3, &offset);
                            if (nn == 2) {
                                if (i0 < 0) t2.vtxi = (int) vertices.size() + i0; else    t2.vtxi = (int) i0 - 1;
                                if (i2 < 0) t2.vtxj = (int) vertices.size() + i2; else    t2.vtxj = (int) i2 - 1;
                                if (i3 < 0) t2.vtxk = (int) vertices.size() + i3; else    t2.vtxk = (int) i3 - 1;
                                if (k0 < 0) t2.ni = (int) normals.size() + k0; else   t2.ni = (int) k0 - 1;
                                if (k2 < 0) t2.nj = (int) normals.size() + k2; else   t2.nj = (int) k2 - 1;
                                if (k3 < 0) t2.nk = (int) normals.size() + k3; else   t2.nk = (int) k3 - 1;
                                consumedline = consumedline + offset;
                                i2 = i3;
                                k2 = k3;
                                indices.push_back(t2);
                            } else {
                                nn = sscanf(consumedline, "%u%n", &i3, &offset);
                                if (nn == 1) {
                                    if (i0 < 0) t2.vtxi = (int) vertices.size() + i0; else    t2.vtxi = (int) i0 - 1;
                                    if (i2 < 0) t2.vtxj = (int) vertices.size() + i2; else    t2.vtxj = (int) i2 - 1;
                                    if (i3 < 0) t2.vtxk = (int) vertices.size() + i3; else    t2.vtxk = (int) i3 - 1;
                                    consumedline = consumedline + offset;
                                    i2 = i3;
                                    indices.push_back(t2);
                                } else {
                                    consumedline = consumedline + 1;
                                }
                            }
                        }
                    }
                }
 
            }
 
        }
        // calcul de la moyenne
        Vector mean;
        for (Vector& v : vertices)
            mean = (mean + v) / vertices.size();
        for (Vector& v : vertices)
            v = v - mean;
        buildBBox();
        fclose(f);
 
    }
    
    virtual double intersect(Ray r, Vector& N);
    
    std::vector<TriangleIndices> indices;
    std::vector<Vector> vertices;
    std::vector<Vector> normals;
    std::vector<Vector> uvs;
    std::vector<Vector> vertexcolors;
    
    BoundingBox bBox;
    
    void buildBBox()
    {
        for (Vector& v : vertices)
        {
            for (int i = 0; i < 3; i++)
            {
                // Itération sur chaque composante
                bBox.mini[i] = std::min(bBox.mini[i], v[i]);
                bBox.maxi[i] = std::max(bBox.maxi[i], v[i]);
            }
        }
    }
    
    void move(int x, int y, int z)
    {
        for (Vector& v : vertices)
            v = v + Vector(x, y, z);
        buildBBox();
    }
    
    void rotate()
    {
        for (Vector& v : vertices)
        {
            std::swap(v[2], v[1]);
            v[2] = -v[2];
        }
        buildBBox();
    }
};

#endif /* mesh_reader_hpp */
