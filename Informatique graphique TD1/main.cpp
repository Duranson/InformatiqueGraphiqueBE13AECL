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

#include "objects.hpp"
 
int main() {
    auto start = std::chrono::high_resolution_clock::now();
    int W = 512;
    int H = 512;
    
    // std::time_t sceneCreation = std::time(nullptr);
    
    // Creation of the camera
    Vector C(0,0,55);
    double fov = 60 * M_PI / 180; // field of view
    
    // Create the light of the scene
    //Light mainLight(Vector(-10, 20, 40) ,1e7);
    Light mainLight(Vector(-10, 20, 40) ,1e7);
    
    Scene mainScene(mainLight);
    
    // Add elements to the scene
    
    // Walls
    
    double horizontal_distance = 30; // All lights should be inside
    double floor_distance = 10; // All lights should be inside
    double back_distance = 100; // All lights should be inside
    double roof_distance = 41; // All lights should be inside
    double ray = 1000; // Should be big for the walls to appear flat
    
    Sphere W_left(Vector( - (ray + horizontal_distance), 0, 0), ray, Vector(0,0,1));
    Sphere W_right(Vector((ray + horizontal_distance), 0, 0), ray, Vector(1,0,1));
    Sphere W_floor(Vector(0, - (ray + floor_distance), 0), ray, Vector(1,0,0));
    Sphere W_roof(Vector(0, (ray + roof_distance), 0), ray, Vector(0,1,1));
    Sphere W_back(Vector(0, 0, - (ray + back_distance / 2)), ray, Vector(0,1,0));
    
    // Spheres
    Sphere S_1(Vector(7, -3, 2), 6, Vector(1, 1, 1));
    Sphere S_2(Vector(-7, -3, 2), 6, Vector(1, 1, 1));
    Sphere S_3(Vector(0, 5, 0), 8, Vector(1, 0, 0));
    
    mainScene.addSphere(S_1);
    mainScene.addSphere(S_2);
    mainScene.addSphere(S_3);
    mainScene.addSphere(W_left);
    mainScene.addSphere(W_right);
    mainScene.addSphere(W_floor);
    mainScene.addSphere(W_roof);
    mainScene.addSphere(W_back);
    
    // std::string scT = std::asctime(std::localtime(&sceneCreation));
    // std::cout << scT << sceneCreation << " s to create the scene." << std::endl;
    
    // std::time_t raytracerTime = std::time(nullptr);
    // Raytracing
    std::vector<unsigned char> image(W*H * 3, 0);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            Vector u(j - W / 2, i - H / 2,  - W / (2. * tan(fov / 2)));
            
            u.normalize();
            
            Ray r(C,u);
            
            Vector color = mainScene.intersects(r);
            
            image[((H-i-1)*W + j) * 3 + 0] = std::min(255.,color[0]);
            image[((H-i-1)*W + j) * 3 + 1] = std::min(255.,color[1]);
            image[((H-i-1)*W + j) * 3 + 2] = std::min(255.,color[2]);
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
    // std::string location = "/Users/fabienduranson/Desktop/Pougne:Pro/ECL/INFO/Info graphique/InformatiqueGraphiqueBE13AECL/Images/new_test_" + scT + "_" + rtT + ".png";
    std::ostringstream out;
    out << "/Users/fabienduranson/Desktop/Pougne:Pro/ECL/INFO/Info graphique/InformatiqueGraphiqueBE13AECL/Images/new_test-" << diff_sec.count() << ".png";
    std::string location = out.str();
    char* cname;
    cname = &location[0];
    stbi_write_png(cname, W, H, 3, &image[0], 0);
    
    return 0;
}
