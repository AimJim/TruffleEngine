#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <string>

class VK_Window{

    VK_Window* instance = nullptr;
    uint32_t width = 800;
    uint32_t height = 600;

    SDL_Window *window = nullptr;

    public:
    VK_Window(uint32_t width, uint32_t height, std::string windowName);

    ~VK_Window();

};