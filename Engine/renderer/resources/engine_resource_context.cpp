#include "engine_resource_context.h"

#include <engine_vulkan/core/vk_context.h>
#include <engine_vulkan/synchronization/vk_sync_immd.h>

#include <engine_vulkan/resources/vk_buffer.h>
#include <engine_vulkan/resources/vk_images.h>

//wrappes

EngineResourceContext::EngineResourceContext(
    VulkanContext* context,
    ImmediateCommandManager* immediateCommands,
    MetaData_generics* metadata,
    MaterialSimpleUnlitRM* metalRoughMaterial
)
    : vkContext(context),
      immSubmits(immediateCommands),
      data(metadata),
      metalRoughMaterial(metalRoughMaterial)
{
}

AllocatedBuffer EngineResourceContext::create_buffer(
    size_t allocSize,
    VkBufferUsageFlags usage,
    VmaMemoryUsage memoryUsage
)
{
    return vkBuffer::create_buffer(
        *vkContext,
        allocSize,
        usage,
        memoryUsage
    );
}

void EngineResourceContext::destroy_buffer(
    const AllocatedBuffer& buffer
)
{
    vkBuffer::destroy_buffer(
        *vkContext,
        buffer
    );
}


GPUMeshBuffers EngineResourceContext::upload_mesh(
    std::span<uint32_t> indices,
    std::span<Vertex> vertices
)
{
    return vkBuffer::uploadMesh(
        *vkContext,
        immSubmits,
        indices,
        vertices
    );
}


//IMAGENS

AllocatedImage EngineResourceContext::create_image(
    VkExtent3D size,
    VkFormat format,
    VkImageUsageFlags usage,
    bool mipmapped
)
{
    return vkImg::create_image(
        *vkContext,
        size,
        format,
        usage,
        mipmapped
    );
}

AllocatedImage EngineResourceContext::create_image(
    void* data,
    VkExtent3D size,
    VkFormat format,
    VkImageUsageFlags usage,
    bool mipmapped
)
{
    return vkImg::create_image(
        data,
        *vkContext,
        *immSubmits,
        size,
        format,
        usage,
        mipmapped
    );
}

void EngineResourceContext::destroy_image(
    const AllocatedImage& image
)
{
    vkImg::destroy_image(
        *vkContext,
        image
    );
}