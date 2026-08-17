#include "meshObject.h"

void MeshObject::update(){
    return;
}

MeshObject::MeshObject(std::string model, std::string texture){
    mesh = new Mesh(model);
    material = new Material(texture);
}

MeshObject::~MeshObject(){
    delete mesh;
    delete material;    
}
    