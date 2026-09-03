#pragma once
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_profiles.hpp>
#include <algorithm>

#ifndef VP_KHR_ROADMAP_2022_NAME
#	define VP_KHR_ROADMAP_2022_NAME "VP_KHR_roadmap_2022"
#endif

#ifndef VP_KHR_ROADMAP_2022_SPEC_VERSION
#	define VP_KHR_ROADMAP_2022_SPEC_VERSION 1
#endif

#include "../utils/glm_imports.h"


class VK_Renderer{
    const int MAX_FRAMES_IN_FLIGHT = 2;
    const int MAX_OBJECTS = 3;
    const bool enableValidationLayers = true;
    const std::vector<char const *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

    struct UniformBufferObject{
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

    struct AppInfo{
        bool profileSupported = false;
        VpProfileProperties profile;
    };

    AppInfo appInfo = {};
    vk::raii::Context context;
    vk::raii::Instance instance = nullptr;
    vk::raii::SurfaceKHR surface = nullptr;
    std::vector<const char *> requiredDeviceExtension = {
	    vk::KHRSwapchainExtensionName,
	    vk::KHRCreateRenderpass2ExtensionName};

    vk::raii::PhysicalDevice physicalDevice = nullptr;
    uint32_t queueIndex = ~0;
    vk::raii::Device device = nullptr;
    vk::raii::Queue queue = nullptr;

    vk::raii::SwapchainKHR swapChain = nullptr;
    std::vector<vk::Image> swapChainImages;
    vk::SurfaceFormatKHR swapChainSurfaceFormat;
    vk::Extent2D swapChainExtent;
    std::vector<vk::raii::ImageView> swapChainImageViews;

    //utils
    std::vector<const char *> getRequiredInstanceExtensions() const;
    bool isDeviceSuitable(vk::raii::PhysicalDevice const &physicalDevice);
    //Vulkan intialazing
    void createInstance();
    //void setupDebugMessenger();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapChain();
    void createImageViews();
    void createDescriptorSetLayout();
    void createGraphicsPipeline();
    void createCommandPool();

    //Model Loading -> Funcion generica que llame a todo esto por cada modelo ?¿
    //Rentaria, que al generar el modelo, se ejecute todo esto ya el solo
    void createDepthResources();
    void createTextureImage();
    void createTextureImageView();
    void createTextureSampler();
    void loadModel(); //?
    void createVertexBuffer(); //?
    void createIndexBuffer();
    void setupGameObjects();
    void createUniformBuffers();

    //Fin de la carga
    void createDescriptorPool();
    void createDescriptorSets();
    void createCommandBuffers();
    void createSyncObjects();

    //
    
    public:

    void init();

    void drawFrame();
    void recreateSwapchain(); //que reciba los datos necesarios

    VK_Renderer();
    ~VK_Renderer();

};