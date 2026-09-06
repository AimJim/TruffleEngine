#include "VK_Mesh.h"

/*The indices array will always be shared*/
VK_Mesh::VK_Mesh(std::vector<Vertex>* vertices, std::vector<uint32_t>* indices, bool sharedMesh){

    this->indices = indices;
    if(sharedMesh){
        this->vertices = vertices;
    }else{
        this->vertices = new std::vector<Vertex>(*vertices);
        this->sharedMesh = sharedMesh;
    }
}
    

VK_Mesh::~VK_Mesh(){
    if(sharedMesh){
        delete vertices;
    
    }
}