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
    
    Vector C(0,0,55);
    Vector O(0,0,0);
    double R = 10;
    Sphere S(O,R);
    double fov = 60 * M_PI / 180;
    
    
    std::vector<unsigned char> image(W*H * 3, 0);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            Vector u(j - W / 2, i - H / 2,  - W / (2. * tan(fov / 2)));
            
            u.normalize();
            
            Ray r(C,u);
            
            if ( S.intersect(r) )
            {
                image[(i*W + j) * 3 + 0] = 255;
                image[(i*W + j) * 3 + 1] = 255;
                image[(i*W + j) * 3 + 2] = 255;
            }
            else
            {
                image[(i*W + j) * 3 + 0] = 0;
                image[(i*W + j) * 3 + 1] = 0;
                image[(i*W + j) * 3 + 2] = 0;
            }
        }
    }
    std::string imageName;
    std::cout << "Nom de l'image : ";
    std::cin >> imageName;
    std::string location = "/Users/fabienduranson/Desktop/Pougne:Pro/ECL/INFO/Info graphique/InformatiqueGraphiqueBE13AECL/Images/" + imageName + ".png";
    char* cname;
    cname = &location[0];
    stbi_write_png(cname, W, H, 3, &image[0], 0);
    
    return 0;
}
