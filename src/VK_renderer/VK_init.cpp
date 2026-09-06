#include "VK_renderer.h"
#include "VK_window.h"
#include "../utils/logging.h"
#include "../utils/readfile.h"
#include <iostream>
#include <assert.h>
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
    
    //If compared to VK_TRUE/FALSE, bad comparison
    if(!SDL_Vulkan_CreateSurface(VK_Window::getInstance()->getWindow(), *instance, nullptr, &_surface)){
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

uint64_t checkVRAM(vk::raii::PhysicalDevice pDevice){
    auto memoryProps = VkPhysicalDeviceMemoryProperties{};
    vkGetPhysicalDeviceMemoryProperties(*pDevice, &memoryProps);

    auto heapsPointer = memoryProps.memoryHeaps;
    auto heaps = std::vector<VkMemoryHeap>(heapsPointer, heapsPointer+memoryProps.memoryHeapCount);
    uint64_t totalvram = 0;
    for (const auto& heap : heaps){
        if (heap.flags & VkMemoryHeapFlagBits::VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
        {
            totalvram += heap.size;
        }
    }

    return totalvram;
}

void VK_Renderer::pickPhysicalDevice(){
    std::vector<vk::raii::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

  
    uint64_t vram = 0;
    uint8_t selectedDevice = 0xFF;
    VkPhysicalDeviceType type;

    for(int i = 0; i< physicalDevices.size(); i++){
        auto props = VkPhysicalDeviceProperties{};
        vkGetPhysicalDeviceProperties(*(physicalDevices[i]), &props);
        //Discrete GPU & most VRAM
        if(props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
            if(checkVRAM(physicalDevices[i]) > vram){
                selectedDevice = i;
            }
        }//Integrated GPU, and NO discrete selected
        else if(props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU && type != VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
            if(checkVRAM(physicalDevices[i]) > vram){
                selectedDevice = i;
            }
        }
    }
    physicalDevice = physicalDevices[selectedDevice];
    if(physicalDevice == nullptr){
        throw std::runtime_error("Failed to find suitable GPU");
        LOGE("Failed to find suitable GPU!");
    }
    
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
        LOGI("Selected GPU: %s", static_cast<const char*>(physicalDevice.getProperties().deviceName));

        
        
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

//Swapchain helper functions
vk::Extent2D chooseSwapExtent(vk::SurfaceCapabilitiesKHR const &capabilities){
    if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()){
        return capabilities.currentExtent;
    }

    int width, height;
    SDL_GetWindowSizeInPixels(VK_Window::getInstance()->getWindow(), &width, &height); //No tengo muy claro que esta valga
    return {
        std::clamp<uint32_t>(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
        std::clamp<uint32_t>(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
    };
}

uint32_t chooseSwapMinImageCount(vk::SurfaceCapabilitiesKHR const &surfaceCapabilities){
    auto minImageCount = std::max(3u, surfaceCapabilities.minImageCount);
    if((0 < surfaceCapabilities.maxImageCount) && (surfaceCapabilities.maxImageCount < minImageCount)){
        minImageCount = surfaceCapabilities.maxImageCount;
    }
    return minImageCount;
}

vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats){
    assert(!availableFormats.empty());
    const auto formatIt = std::ranges::find_if(
        availableFormats,
        [](const auto &format){
            return format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
        }
    );
    return formatIt != availableFormats.end() ? *formatIt : availableFormats[0];
}

vk::PresentModeKHR chooseSwapPresentMode(std::vector<vk::PresentModeKHR> const &availablePresentModes){
    assert(std::ranges::any_of(availablePresentModes, [](auto presentMode){return presentMode ==vk::PresentModeKHR::eFifo;}));
    return std::ranges::any_of(availablePresentModes,
        [](const vk::PresentModeKHR value){ return vk::PresentModeKHR::eMailbox == value; }
    ) ? vk::PresentModeKHR::eMailbox : vk::PresentModeKHR::eFifo;
}

void VK_Renderer::createSwapChain(){

    vk::SurfaceCapabilitiesKHR surfaceCababilities = physicalDevice.getSurfaceCapabilitiesKHR(*surface);
    swapChainExtent = chooseSwapExtent(surfaceCababilities);
    uint32_t minImageCount  = chooseSwapMinImageCount(surfaceCababilities);

    std::vector<vk::SurfaceFormatKHR> availableFormats = physicalDevice.getSurfaceFormatsKHR(*surface);
    swapChainSurfaceFormat = chooseSwapSurfaceFormat(availableFormats);

    std::vector<vk::PresentModeKHR> availablePresentModes = physicalDevice.getSurfacePresentModesKHR(*surface);
    vk::PresentModeKHR presentMode = chooseSwapPresentMode(availablePresentModes);

    vk::SwapchainCreateInfoKHR swapChainCreateInfo{
        .surface = *surface,
        .minImageCount = minImageCount,
        .imageFormat = swapChainSurfaceFormat.format,
        .imageColorSpace = swapChainSurfaceFormat.colorSpace,
        .imageExtent = swapChainExtent,
        .imageArrayLayers = 1,
        .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
        .imageSharingMode = vk::SharingMode::eExclusive,
        .preTransform = surfaceCababilities.currentTransform,
        .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque, // Posiblemente haya que cambiar esto para computar transparencias
        .presentMode = presentMode,
        .clipped = true
    };

    swapChain = vk::raii::SwapchainKHR(device, swapChainCreateInfo);
    swapChainImages = swapChain.getImages();
}

void VK_Renderer::createImageViews(){

    assert(swapChainImageViews.empty());

    vk::ImageViewCreateInfo imageViewCreateInfo{.viewType = vk::ImageViewType::e2D,
        .format = swapChainSurfaceFormat.format,
        .subresourceRange = { vk::ImageAspectFlagBits::eColor, 0,1, 0,1}};
    
    for (auto &image : swapChainImages){
        imageViewCreateInfo.image = image;
        swapChainImageViews.emplace_back(device, imageViewCreateInfo);
    }
}

void VK_Renderer::createDescriptorSetLayout(){
    std::array bindings = {
        vk::DescriptorSetLayoutBinding(0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex, nullptr),
        vk::DescriptorSetLayoutBinding(1, vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment, nullptr)
    };
    vk::DescriptorSetLayoutCreateInfo layoutInfo{.bindingCount = static_cast<uint32_t>(bindings.size()), .pBindings = bindings.data()};
    descriptorSetLayout = vk::raii::DescriptorSetLayout(device, layoutInfo);
}

[[nodiscard]] vk::raii::ShaderModule VK_Renderer::createShaderModule(const std::vector<char> &code){
    vk::ShaderModuleCreateInfo createInfo{.codeSize = code.size(), .pCode = reinterpret_cast<const uint32_t *>(code.data())};
    vk::raii::ShaderModule shaderModule(device, createInfo);
    return shaderModule;
}

void VK_Renderer::createGraphicsPipeline(){ //Aqui se carga el shader, posiblemente moverlo a el correspondiente

    vk::raii::ShaderModule shaderModule = createShaderModule(readFile("shaders/slang.spv")); //TODO, cambiar por el shader que se cargue
}

void VK_Renderer::createCommandPool(){

}

