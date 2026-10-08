#pragma once




#include "utils/stb_image.h"

#include "../utils/glm_imports.h"
#include <string>


class Material{

    int width, height, textureChannels;
    glm::vec3 color;
    stbi_uc *pixels = nullptr;

    public:
    //Use when texture is desired
    Material(std::string texture_path);

    //Use when solid color is desired
    Material(glm::vec3 color); 
    
    
    ~Material();

    //Getters
    uint32_t getWidth();
    uint32_t getHeight();
    uint32_t getTextureChannels();
    glm::vec3 getColor();
    stbi_uc* getPixels();


};