#pragma once 

#include <engine_vulkan/core/vk_context.h>
#include <engine_vulkan/descriptors/vk_descriptors.h>
  
// it's a persistent descriptor manager that lives independently of a frame.
class PersistentDescriptorAllocManager   
{
public:
    void init(VulkanContext& context);
    void shutdown(VulkanContext& context);

    // Persistent/global descriptors
    DescriptorAllocator& get_DescriptorAllocator() { return _globalAllocatorD; } 

private:
    void init_global_allocator(VulkanContext& context);

private:
    // Descriptors that live independently of a frame.
    DescriptorAllocator _globalAllocatorD; 
};





















 