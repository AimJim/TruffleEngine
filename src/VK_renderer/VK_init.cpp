#include "VK_renderer.h"
#include "VK_window.h"
#include "../utils/logging.h"

VK_Renderer::VK_Renderer(){

}
VK_Renderer::~VK_Renderer(){
    
}

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

bool VK_Renderer::isDeviceSuitable(vk::raii::PhysicalDevice const &physicalDevice){
    bool supportsVulkan1_3 = physicalDevice.getProperties().apiVersion >= VK_API_VERSION_1_3;

    //queue families
    auto queueFamilies =physicalDevice.getQueueFamilyProperties();
    bool supportsGraphics = std::ranges::any_of(queueFamilies, [](auto const &qfp) {return !!(qfp.queueFlags & vk::QueueFlagBits::eGraphics);});

    //Extensions
    auto availableDeviceExtensions = physicalDevice.enumerateDeviceExtensionProperties();
    bool supportsAllRequiredExtensions = 
        std::ranges::all_of(requiredDeviceExtension, 
        [&availableDeviceExtensions](auto const &requiredDeviceExtension){
            return std::ranges::any_of(availableDeviceExtensions,
                [requiredDeviceExtension](auto const &availableDeviceExtension) { return strcmp(availableDeviceExtension.extensionName, requiredDeviceExtension) == 0; });
        });
    
    //Features

    auto features =
        physicalDevice
            .template getFeatures2<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();
    bool supportsRequiredFeatures = features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
                                    features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState;

    return supportsVulkan1_3 && supportsGraphics && supportsAllRequiredExtensions && supportsRequiredFeatures;
}

void VK_Renderer::pickPhysicalDevice(){
    std::vector<vk::raii::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
    auto const devIter = std::ranges::find_if(physicalDevices, [&](auto const &physicalDevice) { return isDeviceSuitable(physicalDevice);});

    if (devIter == physicalDevices.end()){
        throw std::runtime_error("Failed to find suitable GPU");
        LOGE("Failed to find suitable GPU!");
    }

    physicalDevice = *devIter;

    //Vulkan profile support
    VpProfileProperties profileProperties;
    strcpy(profileProperties.profileName, VP_KHR_ROADMAP_2022_NAME);
    profileProperties.specVersion = VP_KHR_ROADMAP_2022_SPEC_VERSION;

    VkBool32 supported = VK_FALSE;
    bool result = false;

    VkResult vk_result = vpGetPhysicalDeviceProfileSupport(*instance, *physicalDevice, &profileProperties, &supported);
    result = vk_result == static_cast<int>(vk::Result::eSuccess);

    const char* name = nullptr;

    name = profileProperties.profileName;

    if (result && supported == VK_TRUE)
    {
        appInfo.profileSupported = true;
        appInfo.profile          = profileProperties;
        LOGI("Device supports Vulkan profile: %s", name);
        LOGI("Selected GPU: %s", physicalDevice.getProperties().deviceName);
        
    }
    else
    {
        LOGW("Device does not support Vulkan profile: %s", name);
    }
}

void VK_Renderer::createLogicalDevice(){
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

    for(uint32_t qfpIndex = 0; qfpIndex < queueFamilyProperties.size(); qfpIndex++){
        if((queueFamilyProperties[qfpIndex].queueFlags & vk::QueueFlagBits::eGraphics) &&
            physicalDevice.getSurfaceSupportKHR(qfpIndex, *surface)){
                queueIndex = qfpIndex;
                break;
            }
    }
    if(queueIndex == ~0){
        throw std::runtime_error("Could not find a queue for graphics and present -> terminating");
    }

    //Queries for vk1.3
    auto features = physicalDevice.getFeatures2();
    vk::PhysicalDeviceVulkan13Features vulkan13Features;
    vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT extendedDynamicStateFeatures;
    vulkan13Features.dynamicRendering = vk::True;
    vulkan13Features.synchronization2 = vk::True;
    extendedDynamicStateFeatures.extendedDynamicState = vk::True;
    vulkan13Features.pNext = &extendedDynamicStateFeatures;
    features.pNext = &vulkan13Features;

    //Device Creation
    float queuePriority = 0.5f;
    vk::DeviceQueueCreateInfo deviceQueueCreateInfo{.queueFamilyIndex = queueIndex, .queueCount = 1, .pQueuePriorities = &queuePriority};
    vk::DeviceCreateInfo deviceCreateInfo{
        .pNext = &features,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &deviceQueueCreateInfo,
        .enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtension.size()),
        .ppEnabledExtensionNames = requiredDeviceExtension.data()
    };

    device = vk::raii::Device(physicalDevice, deviceCreateInfo);
    queue = vk::raii::Queue(device, queueIndex, 0);
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

