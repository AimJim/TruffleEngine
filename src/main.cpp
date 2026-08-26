
#include "geometry/meshObject.h"
#include "VK_renderer/VK_window.h"
#include <iostream>
int main(){
    std::string model = "/home/aimar/Documentos/GitHub/TruffleEngine/builds/Model.obj";
    std::string texture = "/home/aimar/Documentos/GitHub/TruffleEngine/builds/Texture.png";
    MeshObject* mo = new MeshObject(model, texture);

    std::cout << "All loaded :)" << std::endl;
    delete mo;
    std::cout << "All Unloaded :)" << std::endl;

    VK_Window *win = new VK_Window(800, 600, "Truffle Engine 0.0.1");

    SDL_Event e;
    bool quit = false;
    //TODO main loop
    while(!quit){
        while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            
            case SDL_EVENT_QUIT:
                quit = true;
                break;
        }
        
    }

    
    }
		
    delete win;

    return 0;
}