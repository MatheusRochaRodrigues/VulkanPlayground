#pragma once

#include <vulkan/vulkan.h>

#include <engine_vulkan/core/vk_types.h>
#include <engine_vulkan/core/vk_context.h>

class RenderTargets
{
public:
    void init(const VulkanContext& context, uint32_t width, uint32_t height);
    void resize(const VulkanContext& context, uint32_t width, uint32_t height);
    void destroy(const VulkanContext& context);

    const AllocatedImage& drawImage() const { return _drawImage; }
    const AllocatedImage& depthImage() const { return _depthImage; }
    VkExtent3D extent() const { return _extent; }

private:
    void create_draw_image(const VulkanContext& context, VkExtent3D extent);
    void create_depth_image(const VulkanContext& context, VkExtent3D extent);

    AllocatedImage _drawImage{};
    AllocatedImage _depthImage{};

    VkExtent3D _extent{};
};