#pragma once

#include <span>
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include <meshs/gpu_mesh_data.h>
#include <engine_vulkan/core/vk_types.h>

struct  VulkanContext;
class   ImmediateCommandManager;
class   MetaData_generics;

class   MaterialSimpleUnlitRM;

class EngineResourceContext {
public:

    EngineResourceContext(
        VulkanContext* context,
        ImmediateCommandManager* immediateCommands,
        MetaData_generics* metadata,
        MaterialSimpleUnlitRM* metalRoughMaterial
    );

    // ---------------------------------------------------------
    // Buffers
    // ---------------------------------------------------------

    AllocatedBuffer create_buffer(
        size_t allocSize,
        VkBufferUsageFlags usage,
        VmaMemoryUsage memoryUsage
    );

    void destroy_buffer(
        const AllocatedBuffer& buffer
    );

    GPUMeshBuffers upload_mesh(
        std::span<uint32_t> indices,
        std::span<Vertex> vertices
    );


    // ---------------------------------------------------------
    // Images
    // ---------------------------------------------------------

    AllocatedImage create_image(
        VkExtent3D size,
        VkFormat format,
        VkImageUsageFlags usage,
        bool mipmapped = false
    );

    AllocatedImage create_image(
        void* data,
        VkExtent3D size,
        VkFormat format,
        VkImageUsageFlags usage,
        bool mipmapped = false
    );

    void destroy_image(
        const AllocatedImage& image
    );

    VulkanContext*              vkContext; 
    MetaData_generics*          data; 
    
    MaterialSimpleUnlitRM* metalRoughMaterial;

private:

    // VulkanContext*              vkContext;
    ImmediateCommandManager*    immSubmits;
    // MetaData_generics*          data;
};