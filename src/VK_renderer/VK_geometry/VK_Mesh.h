#pragma once


#include "../../geometry/vertex.h"

class VK_Mesh{

    std::vector<Vertex>* vertices;
    std::vector<uint32_t>* indices;
    bool sharedMesh = true;
    public:

    VK_Mesh(std::vector<Vertex>* vertices, std::vector<uint32_t>* indices, bool sharedMesh);
    ~VK_Mesh();
};