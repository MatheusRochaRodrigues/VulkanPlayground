#pragma once     
#include <vulkan/vulkan.h>

class VulkanContext;
class Window;

class Swapchain {
public:
    void init(VulkanContext& context, uint32_t width, uint32_t height);
    void resize(VulkanContext&, uint32_t width, uint32_t height);
    void destroy_swapchain(VulkanContext&); 

    VkSwapchainKHR              _swapchain {}; 

    VkFormat                    _imageFormat {};
    VkExtent2D                  _imageExtent {}; 

    std::vector<VkImage>        _images;  
    std::vector<VkImageView>    _imageViews; 
 
private:    
    void create_swapchain(VulkanContext& ctx, uint32_t width, uint32_t height);
};

 

    // VmaAllocator _allocator;
    // DeletionQueue _deletionQueue; 
    // AllocatedImage _drawImage; 