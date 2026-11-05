
#include "geometry/meshObject.h"
#include "VK_renderer/VK_window.h"
#include "VK_renderer/VK_renderer.h"
#include <iostream>
int main(){
    std::string model = "/home/aimar/Documentos/GitHub/TruffleEngine/builds/Model.obj";
    std::string texture = "/home/aimar/Documentos/GitHub/TruffleEngine/builds/Texture.png";
    // MeshObject mo = MeshObject(model, texture);

    // std::cout << "All loaded :)" << std::endl;


    VK_Window *win = new VK_Window(800, 600, "Truffle Engine 0.0.1"); //Se genera como instancia?¿
    std::cout << "Window" << std::endl;
    VK_Renderer renderer = VK_Renderer(model, texture);
    std::cout << "renderer" << std::endl;
    renderer.init();
    std::cout<< "init" << std::endl;
    renderer.mainLoop();
		
    delete win;

    return 0;
}