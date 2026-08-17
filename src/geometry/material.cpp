#include "material.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"

Material::Material(std::string texture_path){
    
    pixels = stbi_load(texture_path.c_str(), &width, &height, &textureChannels, STBI_rgb_alpha);
    color = glm::vec3(1.0f,1.0f,1.0f);
    std::cout << "Image size" << width << " " << height << std::endl;
    if(!pixels){
        throw std::runtime_error("Failed to load texture image");
    }
}

Material::Material(glm::vec3 color){
    width = 0;
    height = 0;
    pixels = nullptr;
}

Material::~Material(){
    stbi_image_free(pixels);
    
    
}