
#include "geometry/meshObject.h"
#include <iostream>
int main(){
    std::string model = "/home/aimar/Documentos/GitHub/TruffleEngine/builds/Model.obj";
    std::string texture = "/home/aimar/Documentos/GitHub/TruffleEngine/builds/Texture.png";
    MeshObject* mo = new MeshObject(model, texture);

    std::cout << "All loaded :)" << std::endl;
    delete mo;
    std::cout << "All Unloaded :)" << std::endl;

    return 0;
}