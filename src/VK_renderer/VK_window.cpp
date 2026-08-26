#include "VK_window.h"


VK_Window::VK_Window(uint32_t width, uint32_t height, std::string windowName){
    if(instance != nullptr){
        return;
    }
    this->width = width;
    this->height = height;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN);

    window = SDL_CreateWindow(windowName.c_str(), width, height, window_flags);

    instance = this;

}

VK_Window::~VK_Window(){
    instance = nullptr;
    SDL_DestroyWindow(window);
}