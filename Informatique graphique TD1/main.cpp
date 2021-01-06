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
 
 
int main() {
    int W = 512;
    int H = 512;
    
    std::vector<unsigned char> image(W*H * 3, 0);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
 
            image[(i*W + j) * 3 + 0] = 255;
            image[(i*W + j) * 3 + 1] = 0;
            image[(i*W + j) * 3 + 2] = 0;
        }
    }
    stbi_write_png("/Users/fabienduranson/Desktop/Pougne:Pro/ECL/INFO/Info graphique/InformatiqueGraphiqueBE13AECL/Images/image.png", W, H, 3, &image[0], 0);
    std::string nomDuGenie;
    printf("Youpi ça compile\n");
    std::cout << "T'es content de toi ?? H vaut " << H << std::endl;
    std::cout << "Quel est ton nom indomptable génie ?" << std::endl;
    std::cin >> nomDuGenie;
    std::cout << std::endl << "Puisse tu vivre longtemps " << nomDuGenie << std::endl;
    
    return 0;
}
