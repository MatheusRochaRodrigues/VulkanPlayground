#pragma once  
#include <vulkan/vulkan.h>

#include <engine_vulkan/core/vk_types.h>

struct VulkanContext;
struct ImmediateCommandManager;

namespace vkImg {

    AllocatedImage create_image(
        const VulkanContext& vkCxt, 
        VkExtent3D size, 
        VkFormat format, 
        VkImageUsageFlags usage , 
        bool mipmapped = false
    );

    AllocatedImage create_image(
        void* data, 
        
        const VulkanContext& vkCxt, 
        ImmediateCommandManager& immCmd,

        VkExtent3D size, 
        VkFormat format, 
        VkImageUsageFlags usage, 
        bool mipmapped = false
    );

    void destroy_image(const VulkanContext& vkCxt, const AllocatedImage& img);
    
    // Utility functions
    void transition_image(
        const VulkanContext& vkCxt, 
        VkCommandBuffer cmd, 
        VkImage image, 
        VkImageLayout currentLayout, 
        VkImageLayout newLayout
    );

    void copy_image_to_image(
        const VulkanContext& vkCxt, 
        VkCommandBuffer cmd, 
        VkImage source, 
        VkImage destination,
        VkExtent2D srcSize, 
        VkExtent2D dstSize
    );

    void generate_mipmaps( const VulkanContext& vkCxt, VkCommandBuffer cmd, VkImage image, VkExtent2D imageSize );

}  
