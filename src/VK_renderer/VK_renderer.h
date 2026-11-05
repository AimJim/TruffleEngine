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

//Image imports
#include "../geometry/material.h"
//Model imports
#include "../geometry/meshObject.h"

//Import for windows
#include "VK_window.h"


class VK_Renderer{
    const int MAX_FRAMES_IN_FLIGHT = 2;
    const int MAX_OBJECTS = 3;
    const bool enableValidationLayers = true;
    const std::vector<char const *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

    bool framebufferResized = false;
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

    vk::raii::DescriptorSetLayout descriptorSetLayout = nullptr;
    vk::raii::PipelineLayout pipelineLayout = nullptr;
    vk::raii::Pipeline graphicsPipeline = nullptr;

    vk::raii::Image depthImage = nullptr;
    vk::raii::DeviceMemory depthImageMemory = nullptr;
    vk::raii::ImageView depthImageView = nullptr;

    //Image loading
    //Is only creating 1 image, TODO
    //These are for a single Image probably. Format may be shared
    vk::Format textureImageFormat = vk::Format::eUndefined;
    vk::raii::Image textureImage = nullptr;
    vk::raii::DeviceMemory textureImageMemory = nullptr;
    vk::raii::ImageView textureImageView = nullptr;
    vk::raii::Sampler textureSampler = nullptr;

    //Model loading
    //Is only loading 1 model, TODO
    vk::raii::Buffer vertexBuffer = nullptr;
    vk::raii::DeviceMemory vertexBufferMemory = nullptr;
    vk::raii::Buffer indexBuffer = nullptr;
    vk::raii::DeviceMemory indexBufferMemory = nullptr;

    //GameObject List
    //std::array<MeshObject, 3> gameObjects; //hardcoded to 3 objects
    //Just one because tutorial, this is part of the GameObject
    MeshObject gameObject;
    std::vector<vk::raii::Buffer> uniformBuffers;
    std::vector<vk::raii::DeviceMemory> uniformBuffersMemory;
    std::vector<void *> uniformBuffersMaped;

    std::vector<vk::raii::DescriptorSet> descriptorSets;

    //Struct for UBO
    struct UnirofmBufferObject{
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

    //Class members again
    vk::raii::DescriptorPool descriptorPool = nullptr;

    vk::raii::CommandPool commandPool = nullptr;
    std::vector<vk::raii::CommandBuffer> commandBuffers;

    //Sync Objects
    std::vector<vk::raii::Semaphore> presentCompleteSemaphores;
    std::vector<vk::raii::Semaphore> renderFinishedSemaphores;
    std::vector<vk::raii::Fence> inFlightFences;
    uint32_t frameIndex = 0;

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
    vk::Format findSupportedFormat(const std::vector<vk::Format> &candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) const;
    vk::Format findDepthFormat() const;
    
    //Model Loading -> Funcion generica que llame a todo esto por cada modelo ?¿
    //Rentaria, que al generar el modelo, se ejecute todo esto ya el solo
    void createDepthResources();
    void createTextureImage(Material* material); //La imagen (pixeles) ya estan cargadas en memoria
    void createTextureImageView();
    void createTextureSampler();
    
    void createVertexBuffer(Mesh* mesh); 
    void createIndexBuffer(Mesh* mesh);
    
    void createUniformBuffers();

    //Utils, these Utils do not write on class members
    uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
    void createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::raii::Buffer &buffer, vk::raii::DeviceMemory &bufferMemory);
    void createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::raii::Image &image, vk::raii::DeviceMemory &imageMemory);
    void transitionImageLayout(const vk::raii::Image &image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);
    void copyBufferToImage(const vk::raii::Buffer &buffer, vk::raii::Image &image, uint32_t width, uint32_t height);
    vk::raii::ImageView createImageView(vk::raii::Image &image, vk::Format format, vk::ImageAspectFlags aspectFlags);
    //Model utils
    void copyBuffer(vk::raii::Buffer &srcBuffer, vk::raii::Buffer &dstBuffer, vk::DeviceSize size);
    
    //Fin de la carga
    void createDescriptorPool();
    void createDescriptorSets();
    void createCommandBuffers();
    void createSyncObjects();

    //utils
    vk::raii::ShaderModule createShaderModule(const std::vector<char> &code);
    std::unique_ptr<vk::raii::CommandBuffer> beginSingleTimeCommands();
    void endSingleTimeCommands(const vk::raii::CommandBuffer &commandBuffer) const;


    //Main loop
    
    void drawFrame();
    void updateUniformBuffers();
    void recordCommandBuffer(uint32_t imageIndex);

    //utils for record
    //Why distinct??
    void transition_image_layout(
	    vk::Image               image,
	    vk::ImageLayout         old_layout,
	    vk::ImageLayout         new_layout,
	    vk::AccessFlags2        src_access_mask,
	    vk::AccessFlags2        dst_access_mask,
	    vk::PipelineStageFlags2 src_stage_mask,
	    vk::PipelineStageFlags2 dst_stage_mask,
	    vk::ImageAspectFlags    image_aspect_flags);
	
    public:

    void init();
    void mainLoop();
    
    void recreateSwapchain(); //Deberia, tomar los tamaños de la ventana, limpiar la swapchain, y recrear swapchian, imageviews y depthresources
    
    void cleanUp();
    VK_Renderer(std::string model, std::string texture);
    ~VK_Renderer();

};