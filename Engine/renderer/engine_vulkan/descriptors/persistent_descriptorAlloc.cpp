#include "persistent_descriptorAlloc.h"
#include <vector>
  
void PersistentDescriptorAllocManager::init_global_allocator( VulkanContext& context) 
{
    // create a descriptor pool
    std::vector< DescriptorAllocator::PoolSizeRatio > sizes = 
    {
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 3 },
        
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 3 },

        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 3 }
    };  
 
    _globalAllocatorD.init_pool(
        context.device,
        10,
        sizes
    );

    // _mainDeletionQueue.push_function( [&]() { vkDestroyDescriptorPool(_device, globalDescriptorAllocator.pool, nullptr); });
}
 
void PersistentDescriptorAllocManager::init(VulkanContext& context)
{
    init_global_allocator(context); 
}

void PersistentDescriptorAllocManager::shutdown(VulkanContext& context)
{
    VkDevice device = context.device;  

    // Destroy per-frame descriptor pools
    // for (auto& allocator : _frameAllocators) 
    //     allocator.destroy_pools(device); 

    // Destroy global descriptor pool
    if (_globalAllocatorD.pool != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorPool(
            device,
            _globalAllocatorD.pool,
            nullptr
        );

        _globalAllocatorD.pool = VK_NULL_HANDLE;
    }
}


 



 

/*
void DescriptorManager::init_layouts( VulkanContext& context)
{
    VkDevice device = context.device;

    // --------------------------------------------------
    // Draw Image
    // -------------------------------------------------- 
    {
        DescriptorLayoutBuilder builder; 
        builder.add_binding( 0, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE );

        _layouts.drawImage =                              
            builder.build( 
                device, 
                VK_SHADER_STAGE_COMPUTE_BIT 
            );  
    } 

    // --------------------------------------------------
    // GPU Scene Data
    // -------------------------------------------------- 
    {
        DescriptorLayoutBuilder builder;
        builder.add_binding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER); 

        _layouts.gpuSceneData =
            builder.build( 
                device,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT
            );
    }
}




void DescriptorManager::shutdown(VulkanContext& context)
{
    VkDevice device = context.device();

    // Destroy descriptor layouts
    if (_layouts.drawImage != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorSetLayout(
            device,
            _layouts.drawImage,
            nullptr
        );

        _layouts.drawImage = VK_NULL_HANDLE;
    }

    if (_layouts.gpuSceneData != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorSetLayout(
            device,
            _layouts.gpuSceneData,
            nullptr
        );

        _layouts.gpuSceneData = VK_NULL_HANDLE;
    }

    // Destroy per-frame descriptor pools
    for (auto& allocator : _frameAllocators)
    {
        allocator.destroy_pools(device);
    }

    // Destroy global descriptor pool
    if (_globalAllocatorD.pool != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorPool(
            device,
            _globalAllocatorD.pool,
            nullptr
        );

        _globalAllocatorD.pool = VK_NULL_HANDLE;
    }
}


*/ 