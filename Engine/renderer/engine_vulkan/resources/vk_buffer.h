#pragma once  

#include <span>   

#include <meshs/gpu_mesh_data.h>

#include <engine_vulkan/core/vk_context.h>
#include <engine_vulkan/core/vk_types.h>
#include <engine_vulkan/descriptors/vk_descriptors.h>

struct VulkanContext;
struct ImmediateCommandManager;

namespace vkBuffer {

    // upload a mesh into a pair of gpu buffers. If descriptor allocator is not
    // null, it will also create a descriptor that points to the vertex buffer
	GPUMeshBuffers uploadMesh(
        const VulkanContext& vkCxt, 
        ImmediateCommandManager* immCmd, 
        std::span<uint32_t> indices, 
        std::span<Vertex> vertices);

    AllocatedBuffer create_buffer(
        const VulkanContext& vkCxt, 
        size_t allocSize, 
        VkBufferUsageFlags usage, 
        VmaMemoryUsage memoryUsage
    );

    void destroy_buffer(const VulkanContext& vkCxt, const AllocatedBuffer& buffer);
 
};
