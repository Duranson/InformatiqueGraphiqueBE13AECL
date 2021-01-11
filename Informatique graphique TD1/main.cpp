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

#include "objects.hpp"
 
int main() {
    int W = 512;
    int H = 512;
    
    // std::time_t sceneCreation = std::time(nullptr);
    
    // Creation of the camera
    Vector C(0,0,55);
    double fov = 60 * M_PI / 180; // field of view
    
    // Create the light of the scene
    double I = 1e7;
    Vector L(-10, 20, 40);
    Light mainLight(L ,I);
    
    Scene mainScene(mainLight);
    
    // Add elements to the scene
    Vector O_1(10,10,10);
    double R_1 = 10;
    Vector rho_1(1,0,1);
    Sphere S_1(O_1,R_1,rho_1);
    
    mainScene.addSphere(S_1);
    
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
    
    // std::string location = "/Users/fabienduranson/Desktop/Pougne:Pro/ECL/INFO/Info graphique/InformatiqueGraphiqueBE13AECL/Images/new_test_" + scT + "_" + rtT + ".png";
    std::string location = "/Users/fabienduranson/Desktop/Pougne:Pro/ECL/INFO/Info graphique/InformatiqueGraphiqueBE13AECL/Images/new_test.png";
    char* cname;
    cname = &location[0];
    stbi_write_png(cname, W, H, 3, &image[0], 0);
    
    return 0;
}
