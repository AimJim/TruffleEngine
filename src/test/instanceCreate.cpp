#include <iostream>
#include <vulkan/vulkan.hpp>

static std::string AppName = "Instance";
static std::string EngineName = "TestEngine1";
int main(){
    try{
        vk::ApplicationInfo applicationInfo ( AppName.c_str(), 1, EngineName.c_str(), VK_API_VERSION_1_4);

        vk::InstanceCreateInfo instanceCreateInfo({}, &applicationInfo);

        vk::Instance instance = vk::createInstance(instanceCreateInfo);

        instance.destroy();
    }
    catch ( vk::SystemError & err){
        std::cout << "vk::SystemError: " <<err.what() << std::endl;
        exit(-1); 
    }catch ( std::exception & err )
    {
        std::cout << "std::exception: " << err.what() << std::endl;
        exit( -1 );
    }
    catch ( ... )
    {
        std::cout << "unknown error\n";
        exit( -1 );
    }


    return 0;
}