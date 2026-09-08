#pragma once

#include <vulkan/vulkan.h>

#include <vk_mem_alloc.h>
#include <utils/clean_behave.h>

constexpr uint32_t FRAME_OVERLAP = 2;   

struct VulkanContext {
    VkInstance                  instance{};
    VkSurfaceKHR                surface{};

    VkPhysicalDevice            physicalDevice{};   //physGPU       //physicalDevice    //physicalDevice       //chosenGPU     _gpu
    VkDevice                    device{};           //logicGPU      //logicDevice       //device

    VkQueue                     graphicsQueue{};
    uint32_t                    graphicsQueueFamily{};

    VmaAllocator                allocator{};

    VkDebugUtilsMessengerEXT    debugMessenger{};


    DeletionQueue               mainDeletionQueue;   // Analyze     MAIN
    
    // VkPhysicalDeviceProperties getGpuInfo() {
    //     VkPhysicalDeviceProperties properties;
    //     properties.apiVersion = 0;
    //     properties.driverVersion = 0;
    //     properties.vendorID = 0;
    //     properties.deviceID = 0;
    //     properties.deviceType = VK_PHYSICAL_DEVICE_TYPE_OTHER;
    //     // strcpy(properties.deviceName, "");
    //     vkGetPhysicalDeviceProperties(physicalDevice, &properties);
    //     return properties;
    // }
};








/*
class VulkanContext {
public:
    void init();
    void shutdown();

    VkInstance instance();
    VkDevice device();
    VkQueue graphicsQueue();
    uint32_t graphicsQueueFamily();

private:
    void create_instance();
    void create_device();

    VkInstance _instance{};
    VkPhysicalDevice _physicalDevice{};
    VkDevice _device{};
    VkQueue _graphicsQueue{};
    uint32_t _graphicsQueueFamily{};
};
*/








/*
struct VulkanContext {
    VkInstance instance{};
    VkSurfaceKHR surface{};

    VkPhysicalDevice _chosenGPU{};      //physicalDevice
    VkDevice _device;

    VkQueue _graphicsQueue;
    uint32_t _graphicsQueueFamily;

    VmaAllocator _allocator; // vma lib allocator

    VkDebugUtilsMessengerEXT _debug_messenger;  //debugMessenger
};
*/