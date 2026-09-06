#pragma once

#include <string>


#include <vector>

#include"../VK_renderer/VK_geometry/VK_Mesh.h"

class Mesh{

    std::vector<Vertex>* vertices;
    std::vector<uint32_t>* indices;
    VK_Mesh* renderingMesh = nullptr;

    bool isRendering = true;//TODO posible cambio cuando se desee no renderizar (Physics o lo q sea)
    bool sharedMesh = true;
    void buildMesh(std::string model_path);
    public:
    Mesh(std::string model_path);
    Mesh(std::string model_path ,bool shareMesh);
    Mesh();
    ~Mesh();

};