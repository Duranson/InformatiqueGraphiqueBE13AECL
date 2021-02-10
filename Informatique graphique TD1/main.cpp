//
//  main.cpp
//  Informatique graphique TD1
//
//  Created by Fabien Duranson on 06/01/2021.
//

#define _CRT_SECURE_NO_WARNINGS 1
#include <vector>
 
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.hpp"
 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>

#include "math.h"

#include "scene.hpp"
#include "integral.hpp"

#include <random>

static std::default_random_engine engine(10) ; // random seed = 10
static std::uniform_real_distribution<double> uniform(0, 1);

int main() {
    // Integral::integral4Dcos2(100000);
    int W = 128; //512;
    int H = 128; //512;
    
    // std::time_t sceneCreation = std::time(nullptr);
    
    // Creation of the camera
    Vector C(0,0,55);
    double fov = 60 * M_PI / 180; // field of view
    
    double distance_plan_mise_au_point = 55;
    double ouverture = 1;
    
    // Create the light of the scene (list position, intensity)
    Light mainLight(0, 2e9);
    
    Sphere light( Vector(-10, 20, 40), 5, Vector(1,1,1), 0., 0., 1., 2e9);
    
    Scene mainScene(mainLight);
    
    // Add elements to the scene
    // Walls
    
    double horizontal_distance = 40; // All lights should be inside
    double floor_distance = 10; // All lights should be inside
    double back_distance = 100; // All lights should be inside
    double front_distance = 200; // All lights should be inside
    double roof_distance = 41; // All lights should be inside
    double ray = 1000; // Should be big for the walls to appear flat
    
    Sphere W_left(Vector( - (ray + horizontal_distance), 0, 0), ray, Vector(0,0,1), 0., 0., 1., 0.);
    Sphere W_right(Vector((ray + horizontal_distance), 0, 0), ray, Vector(1,0,1), 0., 0., 1., 0.);
    Sphere W_floor(Vector(0, - (ray + floor_distance), 0), ray, Vector(1,1,1), 0., 0., 1., 0.); // can put 0.3
    Sphere W_roof(Vector(0, (ray + roof_distance), 0), ray, Vector(0,1,1), 0., 0., 1., 0.);
    Sphere W_back(Vector(0, 0, - (ray + back_distance / 2)), ray, Vector(0,1,0), 0., 0., 1., 0.);
    Sphere W_front(Vector(0, 0, (ray + front_distance / 2)), ray, Vector(1,1,0), 0., 0., 1., 0.);
    
    // Spheres (position, radius, color, reflexion, transparancy, n index, emmissivity)
    // S_1 : miroir en bas à gauche
    Sphere S_1(Vector(7, -3, 2), 6, Vector(1, 1, 1), 1., 0., 1., 0.);
    // S_2 : miroir en bas à droite
    Sphere S_2(Vector(-7, -3, 2), 6, Vector(1, 1, 1), 1., 0., 1., 0.);
    // S_3 : grosse boule rouge du milieu
    Sphere S_3(Vector(0, 5, 0), 8, Vector(1, 0, 0), 0., 0., 1., 0.);
    // S_4 : boule transparante en avant scène
    Sphere S_4(Vector(2, 0, 20), 4, Vector(0.5, 0.5, 1), 1., 1., 1.4, 0.);
    // S_4 : miroir en haut à gauche et en avant scène
    Sphere S_5(Vector(10, 10, 25), 8, Vector(0.5, 0.5, 1), 0., 0., 1., 0.);
    
    mainScene.addSphere(&light);
    //mainScene.addSphere(&S_1);
    //mainScene.addSphere(&S_2);
    //mainScene.addSphere(&S_3);
    mainScene.addSphere(&S_4);
    //mainScene.addSphere(&S_5);
    //mainScene.addSphere(&W_left);
    //mainScene.addSphere(&W_right);
    mainScene.addSphere(&W_floor);
    //mainScene.addSphere(&W_roof);
    mainScene.addSphere(&W_back);
    //mainScene.addSphere(&W_front);
    
    // Read obj file
    TriangleMesh mesh(Vector(1,1,0));
    mesh.readOBJ("/Users/fabienduranson/Desktop/Pougne:Pro/ECL/INFO/Info graphique/InformatiqueGraphiqueBE13AECL/objects/Australian_Cattle_Dog_v1_L3.123c9c6a5764-399b-4e86-9897-6bcb08b5e8ed/13463_Australian_Cattle_Dog_v3.obj");
    
    mesh.rotate();
    mesh.move(0, -10, 5);
    
    mainScene.addSphere(&mesh);
    
    // std::string scT = std::asctime(std::localtime(&sceneCreation));
    // std::cout << scT << sceneCreation << " s to create the scene." << std::endl;
    
    // std::time_t raytracerTime = std::time(nullptr);
    // Raytracing
#pragma omp parallel for schedule(dynamic, 1)
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<unsigned char> image(W*H * 3, 0);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            
            double sigma = 0.25;
            
            Vector color(0,0,0);
            
            int N_iter = 20;
            
            for (int k = 0; k < N_iter; k++)
            {
                double u1 = uniform(engine);
                double u2 = uniform(engine);
                
                double x1 = sigma * cos(2*M_PI*u1) * sqrt(-2 * log(u2));
                double x2 = sigma * sin(2*M_PI*u1) * sqrt(-2 * log(u2));
                
                Vector u(j - W / 2 + x2, i - H / 2 + x1,  - W / (2. * tan(fov / 2)));
                
                u.normalize();
                
                Vector target_point = C + u * distance_plan_mise_au_point;
                
                Ray r(C,u); // rayon depuis le centre de l'ouverture
                
                /*Vector Cp = Integral::random_origin(ouverture, C[2]);
                
                Vector up = target_point - Cp;
                up.normalize();
                
                Ray r(Cp,up); // rayon depuis le centre de l'ouverture*/
                
                color = color + mainScene.intersects(r, 0, false);
            }
            
            color = color / N_iter;
            
            image[((H-i-1)*W + j) * 3 + 0] = std::min(255., std::pow(color[0],0.45));
            image[((H-i-1)*W + j) * 3 + 1] = std::min(255., std::pow(color[1],0.45));
            image[((H-i-1)*W + j) * 3 + 2] = std::min(255., std::pow(color[2],0.45));
            auto diff_seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start);
            std::cout << "Pixel " << i << "x" << j << " fait pour l'image " << H << "x" << W << ". Temps restant estimé : " << (H * W - i * H + j) * diff_seconds.count() / (i * H + j + 1) << " secondes." << std::endl;
        }
    }
        
    // Save the result as choosen
    // std::string rtT = std::asctime(std::localtime(&raytracerTime));
    // std::cout << rtT << raytracerTime << " s to calculate the Raytracer" << std::endl;
    // std::string imageName;
    // std::cout << "Nom de l'image : ";
    // std::cin >> imageName;
    auto end = std::chrono::high_resolution_clock::now();
    auto diff = end - start;
    auto diff_sec = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
    std::ostringstream out;
    //out << "/Users/fabienduranson/Desktop/Pougne:Pro/ECL/INFO/Info graphique/InformatiqueGraphiqueBE13AECL/Images/new_test-" << diff_sec.count() << ".png";
    out << "/Users/fabienduranson/Desktop/Pougne:Pro/ECL/INFO/Info graphique/InformatiqueGraphiqueBE13AECL/Images/new_image" << distance_plan_mise_au_point << "-" << diff_sec.count() << ".png";
    std::string location = out.str();
    char* cname;
    cname = &location[0];
    stbi_write_png(cname, W, H, 3, &image[0], 0);
    
    return 0;
}
