#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan_raii.hpp>

#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <map>


constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;


//Validation layers
const std::vector<char const*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};
#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

class HelloTriangleApplication{
    public:
        void run(){
            initWindow();
            initVulkan();
            mainLoop();
            cleanup();
        }

    private:
        GLFWwindow* window;
        vk::raii::Context context;
        vk::raii::Instance instance = nullptr;
        vk::raii::PhysicalDevice physicalDevice = nullptr;

        void initWindow(){
            glfwInit();
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

            window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr); //4 parameter el monitor donde se ejecuta
        }
        void initVulkan(){
            createInstance();
            pickPhysicalDevice();
        }

        std::vector<const char*> deviceExtensions = {
        vk::KHRSwapchainExtensionName,
        vk::KHRSpirv14ExtensionName,
        vk::KHRSynchronization2ExtensionName,
        vk::KHRCreateRenderpass2ExtensionName
        };

        //Tutorial code to check for needed extensions
        void pickPhysicalDevice() {
            std::vector<vk::raii::PhysicalDevice> devices = instance.enumeratePhysicalDevices();
            const auto devIter = std::ranges::find_if(devices,
            [&](auto const & device) {
                    auto queueFamilies = device.getQueueFamilyProperties();
                    bool isSuitable = device.getProperties().apiVersion >= VK_API_VERSION_1_3;
                    const auto qfpIter = std::ranges::find_if(queueFamilies,
                    []( vk::QueueFamilyProperties const & qfp )
                            {
                                return (qfp.queueFlags & vk::QueueFlagBits::eGraphics) != static_cast<vk::QueueFlags>(0);
                            } );
                    isSuitable = isSuitable && ( qfpIter != queueFamilies.end() );
                    auto extensions = device.enumerateDeviceExtensionProperties( );
                    bool found = true;
                    for (auto const & extension : deviceExtensions) {
                        auto extensionIter = std::ranges::find_if(extensions, [extension](auto const & ext) {return strcmp(ext.extensionName, extension) == 0;});
                        found = found &&  extensionIter != extensions.end();
                    }
                    isSuitable = isSuitable && found;
                    if (isSuitable) {
                        physicalDevice = device;
                        std::cout << "Selected device: " << physicalDevice.getProperties().deviceName << std::endl;
                    }
                    return isSuitable;
            });
            if (devIter == devices.end()) {
                throw std::runtime_error("failed to find a suitable GPU!");
            }
        }
        
        // void pickPhysicalDevice(){
        //     auto devices = instance.enumeratePhysicalDevices();
        //     if(devices.empty()){
        //         throw std::runtime_error("No GPUs supporting Vulkan found!");
        //     }

        //     //Map to check for best GPU
        //     std::multimap<int, vk::raii::PhysicalDevice> candidates;
        //     //Check if devices meet requirements
        //     for(const auto& device :  devices){
        //         auto deviceProperties = device.getProperties();
        //         auto deviceFeatures = device.getFeatures();
        //         uint32_t score = 0;

        //         //Dedicated GPUS are best
        //         if(deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu){
        //             score += 1000;
        //         }

        //         //Max texture size
        //         score += deviceProperties.limits.maxImageDimension2D;

        //         //Cannot geometry shader
        //         if(!deviceFeatures.geometryShader){
        //             continue;
        //         }
        //         candidates.insert(std::make_pair(score, device));

        //     }

        //     if(candidates.rbegin()->first > 0){
        //         physicalDevice = candidates.rbegin()->second;
        //         std::cout<< "Chosen GPU: " << physicalDevice.getProperties().deviceName << std::endl;
        //     }else{
        //         throw std::runtime_error("failed to find sutiable GPU!");
        //     }
        // }


        uint32_t findQueueFamilies(vk::raii::PhysicalDevice physicalDevice){
            std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

            // get the first index into queueFamilyProperties which supports graphics
            auto graphicsQueueFamilyProperty =
            std::find_if( queueFamilyProperties.begin(),
                            queueFamilyProperties.end(),
                            []( vk::QueueFamilyProperties const & qfp ) { return qfp.queueFlags & vk::QueueFlagBits::eGraphics; } );

            return static_cast<uint32_t>( std::distance( queueFamilyProperties.begin(), graphicsQueueFamilyProperty ) );
        }
        //Support geometry shaders
        bool isDeviceSuitable(vk::raii::PhysicalDevice physicalDevice){
            //Device properties
            auto deviceProperties = physicalDevice.getProperties();

            //Features
            auto deviceFeatures = physicalDevice.getFeatures();

            if(deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu && deviceFeatures.geometryShader){
                return true;
            }
            return false;
        }

        void mainLoop(){
            while(!glfwWindowShouldClose(window)){
                glfwPollEvents();
                
            }
        }
        void cleanup(){
            glfwDestroyWindow(window);
            glfwTerminate();
        }

        //vulkan required extensions
        std::vector<const char*> getRequiredExtensions(){
            uint32_t glfwExtensionCount = 0;
            auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
            if(enableValidationLayers){
                extensions.push_back(vk::EXTDebugUtilsExtensionName);
            }

            return extensions;
        }

        //Create instance
        void createInstance(){
            constexpr vk::ApplicationInfo appInfo{
                .pApplicationName = "Hello Triangle",
                .applicationVersion = VK_MAKE_VERSION(1,0,0),
                .pEngineName = "No Engine",
                .engineVersion = VK_MAKE_VERSION(1,0,0),
                .apiVersion = vk::ApiVersion14
            };


            //Get the layres
            std::vector<char const*> requiredLayers;
            if(enableValidationLayers){
                requiredLayers.assign(validationLayers.begin(), validationLayers.end());
            }
            //Check layer suport
            auto layerProperties = context.enumerateInstanceLayerProperties();
            if (std::ranges::any_of(requiredLayers, [&layerProperties](auto const& requiredLayer) {
                return std::ranges::none_of(layerProperties,
                                        [requiredLayer](auto const& layerProperty)
                                        { return strcmp(layerProperty.layerName, requiredLayer) == 0; });}))
                {
                    throw std::runtime_error("One or more required layers are not supported!");
                }

            
            //Get extensions
            auto requiredExtensions = getRequiredExtensions();
            

            // //Check if extensions are supported
            auto extensionProperties = context.enumerateInstanceExtensionProperties();
            for (auto const & requiredExtension : requiredExtensions)
    {
            if (std::ranges::none_of(extensionProperties,
                                    [requiredExtension](auto const& extensionProperty)
                                    { return strcmp(extensionProperty.extensionName, requiredExtension) == 0; }))
            {
                throw std::runtime_error("Required extension not supported: " + std::string(requiredExtension));
            }
    }

            vk::InstanceCreateInfo createInfo{
                .pApplicationInfo = &appInfo,
                .enabledLayerCount = static_cast<uint32_t>(requiredLayers.size()),
                .ppEnabledLayerNames = requiredLayers.data(),
                .enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()),
                .ppEnabledExtensionNames = requiredExtensions.data()
            };

            instance = vk::raii::Instance(context, createInfo);
        }
};

int main(){
    HelloTriangleApplication app;
    try{
        app.run();
    } catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}