#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan_raii.hpp>

#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>


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

        void initWindow(){
            glfwInit();
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

            window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr); //4 parameter el monitor donde se ejecuta
        }
        void initVulkan(){
            createInstance();
        }
        void mainLoop(){
            while(!glfwWindowShouldClose(window)){
                glfwPollEvents();
                std::cout << "loop" << std::endl;
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