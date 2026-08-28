#include "VK_renderer.h"
#include "VK_window.h"
#include "../utils/logging.h"

void VK_Renderer::init(){
    createInstance();
    //setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createDescriptorSetLayout();
    createGraphicsPipeline();
    createCommandPool();
}

[[nodiscard]] std::vector<const char *> VK_Renderer::getRequiredInstanceExtensions() const{
    std::vector <const char *> extensions;
    uint32_t sdlExtensionCount = 0;
    auto sdlExtensions = SDL_Vulkan_GetInstanceExtensions(&sdlExtensionCount);
    extensions.assign(sdlExtensions, sdlExtensions + sdlExtensionCount);

    if(enableValidationLayers){
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return extensions;
    
}

void VK_Renderer::createInstance(){ //Posiblemente que reciba el nombre de aplicacion por llamada
    constexpr vk::ApplicationInfo appInfo{
        .pApplicationName = "Truffle Engine",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "Truffle Engine",
        .engineVersion = VK_MAKE_VERSION(1,0,0),
        .apiVersion = VK_API_VERSION_1_3
    };

    auto extensions = getRequiredInstanceExtensions();

    vk::InstanceCreateInfo createInfo{
        .pApplicationInfo = &appInfo,
        .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
        .ppEnabledExtensionNames = extensions.data()
    };

    instance = vk::raii::Instance(context, createInfo);
    LOGI("Vulkan instance created");
}


void VK_Renderer::createSurface(){
    VkSurfaceKHR _surface;
    if(SDL_Vulkan_CreateSurface(VK_Window::getInstance()->getWindow(), *instance, nullptr, &_surface) != VK_SUCCESS){
        LOGE("SDL Surface not loaded");
        throw std::runtime_error("failed to create window surface!");

    }
    surface = vk::raii::SurfaceKHR(instance, _surface);
    LOGI("SDL Surface loaded");
}

void VK_Renderer::pickPhysicalDevice(){

}

void VK_Renderer::createLogicalDevice(){

}

void VK_Renderer::createSwapChain(){

}

void VK_Renderer::createImageViews(){

}

void VK_Renderer::createDescriptorSetLayout(){

}

void VK_Renderer::createGraphicsPipeline(){

}

void VK_Renderer::createCommandPool(){

}

