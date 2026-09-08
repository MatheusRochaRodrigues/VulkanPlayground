#include "scene_rendererDescriptor.h"

#include <engine_vulkan/core/vk_context.h>
#include <engine_vulkan/descriptors/vk_descriptors.h>

void SceneRendererDescriptor::init(
    VulkanContext& context,
    DescriptorAllocator& descriptorAllocator)
{
    createLayouts(context);

    createDescriptorSet(
        context,
        descriptorAllocator
    );
}

void SceneRendererDescriptor::createLayouts(
    VulkanContext& context)
{ 
    // --------------------------------------------------
    // GPU Scene Data
    // -------------------------------------------------- 
    {
        DescriptorLayoutBuilder builder; 
        builder.add_binding( 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER );

        _gpuSceneDataLayout = builder.build ( 
            context.device,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT
        );
    }
}

void SceneRendererDescriptor::createDescriptorSet(
    VulkanContext& context,
    DescriptorAllocator& descriptorAllocator)
{
    _gpuSceneDataSet =
        descriptorAllocator.allocate(
            context.device,
            _gpuSceneDataLayout
        );
}

void SceneRendererDescriptor::updateDescriptors(
    VulkanContext& context,
    VkBuffer sceneBuffer,
    VkDeviceSize bufferSize)
{
    DescriptorWriter writer;

    writer.write_buffer(
        0,
        sceneBuffer,
        bufferSize,
        0,
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
    );

    writer.update_set(
        context.device,
        _gpuSceneDataSet
    );
}

void SceneRendererDescriptor::destroy(
    VulkanContext& context)
{
    if (_gpuSceneDataLayout != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorSetLayout(
            context.device,
            _gpuSceneDataLayout,
            nullptr
        );

        _gpuSceneDataLayout = VK_NULL_HANDLE;
    }

    _gpuSceneDataSet = VK_NULL_HANDLE;
}
 