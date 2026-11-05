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
    Mesh* getMesh();
    Material* getMaterial();
    MeshObject();
    MeshObject(std::string model, std::string texture);
    ~MeshObject();

    
};