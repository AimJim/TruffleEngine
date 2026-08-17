#pragma once
#include "object.h"
#include "mesh.h"
#include "material.h"
#include <string>

class MeshObject : public Object{
  
    Mesh* mesh;
    Material* material; 

    public:
    
    void update() override;
    MeshObject(std::string model, std::string texture);
    ~MeshObject();

    
};