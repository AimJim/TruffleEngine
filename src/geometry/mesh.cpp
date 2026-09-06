#include "mesh.h"
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "utils/tiny_obj_loader.h"

void Mesh::buildMesh(std::string model_path){
    //Declare vectors
    vertices = new std::vector<Vertex>();
    indices = new std::vector<uint32_t>();

   tinyobj::attrib_t attrib;
   std::vector<tinyobj::shape_t> shapes;
   std::vector<tinyobj::material_t> materials; //For now ignored, but change in future
   std::string warn, err;

   if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, model_path.c_str())){
    std::cout << "Path " << model_path << std::endl;
    std::cout << warn << std::endl;
    std::cout << err << std::endl; 
    throw std::runtime_error(warn + err);
   }
   
   std::unordered_map<Vertex, uint32_t> uniqueVertex{};
    for (const auto &shape : shapes){
        for (const auto &index : shape.mesh.indices){
            
            Vertex vertex;
            vertex.pos = {
                attrib.vertices[3* index.vertex_index +0],
                attrib.vertices[3* index.vertex_index +1],
                attrib.vertices[3* index.vertex_index +2]};

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index +0],
                attrib.texcoords[2 * index.texcoord_index +1]
            };
            vertex.color = {1.0f, 1.0f, 1.0f};

            auto [it, inserted] = uniqueVertex.insert({vertex, static_cast<uint32_t>(vertices->size())});
            if(inserted){
                vertices->push_back(vertex);
            }
            indices->push_back(it->second);
        }
   }
   std::cout << "Loaded vertices: " << vertices->size() << std::endl;
   renderingMesh = new VK_Mesh(vertices,indices,sharedMesh);
}

/*If not specified, the generated VK_Mesh will share the vertices with the mesh, the indices array will always be shared*/
Mesh::Mesh(std::string model_path){
    buildMesh(model_path);
    
}
Mesh::Mesh(std::string model_path ,bool shareMesh){
    sharedMesh = sharedMesh;
    buildMesh(model_path);
    
}
Mesh::Mesh(){
    vertices = new std::vector<Vertex>();
    indices = new std::vector<u_int32_t>();
}

Mesh::~Mesh(){
    vertices->clear();
    indices->clear();
    delete renderingMesh;
    delete vertices;
    delete indices;
    
}