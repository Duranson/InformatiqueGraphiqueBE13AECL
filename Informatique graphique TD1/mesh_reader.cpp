//
//  mesh_reader.cpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 10/02/2021.
//

#include "mesh_reader.hpp"
#include "stb_image.hpp"
#include <list>

double TriangleMesh::intersect(Ray r, Vector& N, Vector &color)
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
                double alpha = 1 - beta - gamma;
                double t_intersect = - AO.dot(normal) / UvN;
                
                if (beta >= 0 && beta + gamma < 1 && gamma >= 0 && t > 0)
                {
                    if (t_intersect < t)
                    {
                        t = t_intersect;
                        P = r.C + r.u * t;
                        if (normals.size() > 0)
                            N = normals[indices[i].ni] * alpha + normals[indices[i].nj] * beta + normals[indices[i].nk] * gamma;
                        else
                            N = - normal; // Etrange
                        N.normalize();
                        if (textures.size() > 0)
                        {
                            Vector UV = uvs[indices[i].uvi] * alpha + uvs[indices[i].uvj] * beta + uvs[indices[i].uvk] * gamma;
                            int W = Wtex[indices[i].group];
                            int H = Htex[indices[i].group];
                            UV = UV * Vector(W, H ,0);
                            int uvx = UV[0] + 0.5;
                            int uvy = H - UV[1] + 0.5;
                            uvx = uvx % W;
                            uvy = uvy % H;
                            if (uvx < 0) uvx += W;
                            if (uvy < 0) uvy += H;
                            int x0 = uvy * W + uvx;
                            color = Vector(pow(textures[indices[i].group][x0 * 3 + 0] / 255.,2.2), pow(textures[indices[i].group][x0 * 3 + 1] / 255.,2.2),
                                pow(textures[indices[i].group][x0 * 3 + 2] / 255.,2.2));
                        }
                        else
                        {
                            color = Vector(1,1,1);
                        }
                    }
                }
            }
        }
        
    }
    return t;
}

void TriangleMesh::readOBJ(const char* obj) {
    
    // char matfile[255];
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
    BVH = new Noeud;
    buildBVH(BVH, 0, (int) indices.size());
    fclose(f);

}

void TriangleMesh::loadTexture(const char* filename)
{
    int W, H, C;
    unsigned char* texture = stbi_load(filename, &W, &H, &C, 3);
    Wtex.push_back(W);
    Htex.push_back(H);
    textures.push_back(texture);
}
