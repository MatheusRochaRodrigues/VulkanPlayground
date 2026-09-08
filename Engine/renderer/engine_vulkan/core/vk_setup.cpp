#include "vk_setup.h"
#include <VkBootstrap.h>
#include <engine_vulkan/core/vk_context.h>
#include <platform/window.h>   
 
void setUpContextVulkan::init_resources(VulkanContext& context, vkb::Instance& vkb_inst){  
    VkPhysicalDeviceVulkan13Features features13 { .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES }; 
    features13.dynamicRendering = true;         
    features13.synchronization2 = true;
    VkPhysicalDeviceVulkan12Features features12 { .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
    features12.bufferDeviceAddress = true;       
    features12.descriptorIndexing = true; 

    // use vkbootstrap to select a gpu.
    // We want a gpu that can write to the SDL surface and supports vulkan 1.2
    vkb::PhysicalDeviceSelector selector { vkb_inst };
    vkb::PhysicalDevice physicalDevice = selector.set_minimum_version(1, 3)
                        .set_required_features_13(features13)
                        .set_required_features_12(features12)
                        .set_surface(context.surface)
                        .select().value();

    // physicalDevice.features.
    // create the final vulkan device
    vkb::DeviceBuilder deviceBuilder { physicalDevice };
    vkb::Device vkbDevice = deviceBuilder.build().value();

    // Get the VkDevice handle used in the rest of a vulkan application
    context.device = vkbDevice.device;
    context.physicalDevice = physicalDevice.physical_device;

    // use vkbootstrap to get a Graphics queue
    context.graphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();

    context.graphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

    // initialize the memory allocator
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.physicalDevice = context.physicalDevice;
    allocatorInfo.device = context.device;
    allocatorInfo.instance = context.instance;
    allocatorInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
    vmaCreateAllocator(&allocatorInfo, &context.allocator); 
}

void setUpContextVulkan::init_instance(Window& window, VulkanContext& context, bool bUseValidationLayers){ 
    vkb::InstanceBuilder builder;

    // make the vulkan instance, with basic debug features
    auto inst_ret = builder.set_app_name("Paper Engine - AppVulkan")
                        .request_validation_layers(bUseValidationLayers)
                        .use_default_debug_messenger()
                        .require_api_version(1, 3, 0)
                        .build();

    vkb::Instance vkb_inst = inst_ret.value();

    // grab the instance
    context.instance = vkb_inst.instance;
    context.debugMessenger = vkb_inst.debug_messenger;
    

    // create a surface to draw to
    window.CreateSurface(context);

    // initialize the rest of vulkan (device, memory, etc)
    init_resources(context, vkb_inst);
}


// void init_vulkan(
//     VulkanContext& context,
//     Window& window)
// {
//     // create instance
//     // create surface
//     // select GPU
//     // create device
//     // create queues
//     // create VMA
 
// }