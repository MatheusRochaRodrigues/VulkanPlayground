#include "render_targetDescriptor.h"

#include <engine_vulkan/core/vk_context.h>
#include <engine_vulkan/descriptors/vk_descriptors.h>

#include <resources/render_targets.h>

void RenderTargetDescriptor::init(
    VulkanContext& ctx,
    DescriptorAllocator& descriptorAllocator,
    RenderTargets* renderTargets)
{
    createLayouts(ctx);

    createDescriptorSet(
        ctx,
        descriptorAllocator
    );

    updateDescriptors(
        ctx,
        *renderTargets
    );

    _targets = renderTargets;
}

void RenderTargetDescriptor::createLayouts(VulkanContext& ctx)
{
    DescriptorLayoutBuilder builder;

    builder.add_binding(
        0,
        VK_DESCRIPTOR_TYPE_STORAGE_IMAGE
    );

    _drawImageLayout = builder.build(
        ctx.device,
        VK_SHADER_STAGE_COMPUTE_BIT
    );
}

void RenderTargetDescriptor::createDescriptorSet(
    VulkanContext& ctx,
    DescriptorAllocator& descriptorAllocator)
{
    _drawImageSet =
        descriptorAllocator.allocate(
            ctx.device,
            _drawImageLayout
        );
}

void RenderTargetDescriptor::updateDescriptors(
    VulkanContext& ctx,
    const RenderTargets& renderTargets)
{
    DescriptorWriter writer;

    writer.write_image(
        0,
        renderTargets.drawImage().imageView,
        VK_NULL_HANDLE,
        VK_IMAGE_LAYOUT_GENERAL,
        VK_DESCRIPTOR_TYPE_STORAGE_IMAGE
    );

    writer.update_set(
        ctx.device,
        _drawImageSet
    );
}

void RenderTargetDescriptor::destroy(VulkanContext& ctx)
{
    if (_drawImageLayout != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorSetLayout(
            ctx.device,
            _drawImageLayout,
            nullptr
        );

        _drawImageLayout = VK_NULL_HANDLE;
    }

    _drawImageSet = VK_NULL_HANDLE;
}