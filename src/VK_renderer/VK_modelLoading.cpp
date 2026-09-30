#include "VK_renderer.h"

#include "../utils/logging.h"
#include "../utils/readfile.h"
#include <iostream>
#include <assert.h>

#include "../geometry/vertex.h"

vk::raii::ImageView VK_Renderer::createImageView(vk::raii::Image &image, vk::Format format, vk::ImageAspectFlags aspectFlags){
    vk::ImageViewCreateInfo viewInfo{
        .image = *image,
        .viewType = vk::ImageViewType::e2D,
        .format = format,
        .subresourceRange = {aspectFlags, 0, 1, 0, 1}
    };

    return vk::raii::ImageView(device, viewInfo);
}

uint32_t VK_Renderer::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties){
    vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++){
        
        if((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties){
            return i;
        }
    }
    throw std::runtime_error("Failed to find suitable memory type");
}

void VK_Renderer::createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::raii::Image &image, vk::raii::DeviceMemory &imageMemory){

    vk::ImageCreateInfo imageInfo{
		    .imageType     = vk::ImageType::e2D,
		    .format        = format,
		    .extent        = {width, height, 1},
		    .mipLevels     = 1,
		    .arrayLayers   = 1,
		    .samples       = vk::SampleCountFlagBits::e1,
		    .tiling        = tiling,
		    .usage         = usage,
		    .sharingMode   = vk::SharingMode::eExclusive,
		    .initialLayout = vk::ImageLayout::eUndefined};
		image = vk::raii::Image(device, imageInfo);

		vk::MemoryRequirements memRequirements = image.getMemoryRequirements();
		vk::MemoryAllocateInfo allocInfo{
		    .allocationSize  = memRequirements.size,
		    .memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties)};
		imageMemory = vk::raii::DeviceMemory(device, allocInfo);
		image.bindMemory(*imageMemory, 0);

}

void VK_Renderer::createDepthResources(){
    vk::Format depthFormat = findDepthFormat();

    createImage(swapChainExtent.width, swapChainExtent.height, depthFormat, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal, depthImage, depthImageMemory);
    depthImageView = createImageView(depthImage, depthFormat, vk::ImageAspectFlagBits::eDepth);
}
void createTextureImage();
void createTextureImageView();
void createTextureSampler();
void loadModel(); //?
void createVertexBuffer(); //?
void createIndexBuffer();
void setupGameObjects();
void createUniformBuffers();