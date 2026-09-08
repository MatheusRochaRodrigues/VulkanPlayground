#include "render_targets.h"
#include <engine_vulkan/initialization/vk_initializers.h>

//------------------------------------
//      RESOURCES IMAGE TARGET
//------------------------------------

void RenderTargets::init(
    const VulkanContext& context,
    uint32_t width,
    uint32_t height)
{
	//depth image size will match the window
    _extent = {
        width,
        height,
        1
    };

    create_draw_image(context, _extent);
    create_depth_image(context, _extent);
}

void RenderTargets::create_draw_image(
    const VulkanContext& context,
    VkExtent3D extent)
{
	//hardcoding the draw format to 32 bit float
    _drawImage.imageFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
    _drawImage.imageExtent = extent;

    VkImageUsageFlags usages =
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
        VK_IMAGE_USAGE_STORAGE_BIT |
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    VkImageCreateInfo imageInfo =
        vkinit::image_create_info(
            _drawImage.imageFormat,
            usages,
            extent
        );

    VmaAllocationCreateInfo allocationInfo{};
    allocationInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    allocationInfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        // VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

	//allocate and create the image
    VK_CHECK(vmaCreateImage(
        context.allocator,
        &imageInfo,
        &allocationInfo,
        &_drawImage.image,
        &_drawImage.allocation,
        nullptr
    ));

	//build a image-view for the draw image to use for rendering
    VkImageViewCreateInfo viewInfo =
        vkinit::imageview_create_info(
            _drawImage.imageFormat,
            _drawImage.image,
            VK_IMAGE_ASPECT_COLOR_BIT
        );

    VK_CHECK(vkCreateImageView(
        context.device,
        &viewInfo,
        nullptr,
        &_drawImage.imageView
    ));
}


void RenderTargets::create_depth_image(
    const VulkanContext& context,
    VkExtent3D extent)
{
    _depthImage.imageFormat = VK_FORMAT_D32_SFLOAT;
    _depthImage.imageExtent = extent;

    VkImageUsageFlags usages = {};
    usages |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

    VkImageCreateInfo imageInfo =
        vkinit::image_create_info(
            _depthImage.imageFormat,
            usages,
            extent
        );

    VmaAllocationCreateInfo allocationInfo{};
    allocationInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    allocationInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    VK_CHECK(vmaCreateImage(
        context.allocator,
        &imageInfo,
        &allocationInfo,
        &_depthImage.image,
        &_depthImage.allocation,
        nullptr
    ));

    VkImageViewCreateInfo viewInfo =
        vkinit::imageview_create_info(
            _depthImage.imageFormat,
            _depthImage.image,
            VK_IMAGE_ASPECT_DEPTH_BIT
        );

    VK_CHECK(vkCreateImageView(
        context.device,
        &viewInfo,
        nullptr,
        &_depthImage.imageView
    ));
}


void RenderTargets::resize(
    const VulkanContext& context,
    uint32_t width,
    uint32_t height)
{
    destroy(context);

    _extent = {
        width,
        height,
        1
    };

    create_draw_image(context, _extent);
    create_depth_image(context, _extent);
}


void RenderTargets::destroy(const VulkanContext& context)
{
    if (_drawImage.imageView)
    {
        vkDestroyImageView(
            context.device,
            _drawImage.imageView,
            nullptr
        );
    }

    if (_drawImage.image)
    {
        vmaDestroyImage(
            context.allocator,
            _drawImage.image,
            _drawImage.allocation
        );
    }

    if (_depthImage.imageView)
    {
        vkDestroyImageView(
            context.device,
            _depthImage.imageView,
            nullptr
        );
    }

    if (_depthImage.image)
    {
        vmaDestroyImage(
            context.allocator,
            _depthImage.image,
            _depthImage.allocation
        );
    }

    _drawImage = {};
    _depthImage = {};
    _extent = {};
}