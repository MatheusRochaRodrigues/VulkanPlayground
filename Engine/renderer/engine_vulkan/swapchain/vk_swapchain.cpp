#include "vk_swapchain.h"
#include <engine_vulkan/core/vk_context.h>
#include <VkBootstrap.h>
#include <platform/window.h>
 
void Swapchain::create_swapchain( VulkanContext& context, uint32_t width, uint32_t height )
{
	vkb::SwapchainBuilder swapchainBuilder{ 
		context.physicalDevice, 		/*_chosenGPU*/
		context.device, 						/*Logic of the chosenGPU*/
		context.surface 
	};	

	_imageFormat = VK_FORMAT_B8G8R8A8_UNORM;

	vkb::Swapchain vkbSwapchain = swapchainBuilder
		//.use_default_format_selection()
		.set_desired_format(VkSurfaceFormatKHR{ .format = _imageFormat, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR })
		//use vsync present mode
		.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
		.set_desired_extent(width, height)
		.add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
		.build()
		.value();

	_imageExtent 	= vkbSwapchain.extent;
	//store swapchain and its related images
	_swapchain 		= vkbSwapchain.swapchain;
	_images 		= vkbSwapchain.get_images().value();
	_imageViews 	= vkbSwapchain.get_image_views().value();
}

void Swapchain::init( VulkanContext& context, uint32_t width, uint32_t height )
{
    create_swapchain(context, width, height); 
}

void Swapchain::destroy_swapchain(VulkanContext& ctx) 
{
	vkDestroySwapchainKHR(ctx.device, _swapchain, nullptr);

	// destroy swapchain resources
	for (int i = 0; i < _imageViews.size(); i++) { 
		vkDestroyImageView(ctx.device, _imageViews[i], nullptr);
	}
}

void Swapchain::resize(VulkanContext& ctx, uint32_t width, uint32_t height)
{
	vkDeviceWaitIdle(ctx.device);

	destroy_swapchain(ctx);

	create_swapchain(ctx, width, height);
}




	// int w, h;	SDL_GetWindowSize(window, &w, &h);
	// _windowExtent.width = w;		_windowExtent.height = h;



























/*
void Swapchain::init( VulkanContext& context, uint32_t width, uint32_t height )
{
 
	//hardcoding the draw format to 32 bit float
	_drawImage.imageFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
    _drawImage.imageExtent = drawImageExtent;

	VkImageUsageFlags drawImageUsages{};
	drawImageUsages |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	drawImageUsages |= VK_IMAGE_USAGE_STORAGE_BIT;
	drawImageUsages |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	VkImageCreateInfo rimg_info = vkinit::image_create_info(_drawImage.imageFormat, drawImageUsages, drawImageExtent);

	//for the draw image, we want to allocate it from gpu local memory
	VmaAllocationCreateInfo rimg_allocinfo = {};
	rimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	rimg_allocinfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	//allocate and create the image
	vmaCreateImage(_allocator, &rimg_info, &rimg_allocinfo, &_drawImage.image, &_drawImage.allocation, nullptr);

	//build a image-view for the draw image to use for rendering
	VkImageViewCreateInfo rview_info = vkinit::imageview_create_info(_drawImage.imageFormat, _drawImage.image, VK_IMAGE_ASPECT_COLOR_BIT);

	VK_CHECK(vkCreateImageView(_device, &rview_info, nullptr, &_drawImage.imageView));

    //create a depth image too
	//hardcoding the draw format to 32 bit float
	_depthImage.imageFormat = VK_FORMAT_D32_SFLOAT;
    _depthImage.imageExtent = drawImageExtent;

	VkImageUsageFlags depthImageUsages{};
	depthImageUsages |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

	VkImageCreateInfo dimg_info = vkinit::image_create_info(_depthImage.imageFormat, depthImageUsages, drawImageExtent);

	//allocate and create the image
	vmaCreateImage(_allocator, &dimg_info, &rimg_allocinfo, &_depthImage.image, &_depthImage.allocation, nullptr);

	//build a image-view for the draw image to use for rendering
	VkImageViewCreateInfo dview_info = vkinit::imageview_create_info(_depthImage.imageFormat, _depthImage.image, VK_IMAGE_ASPECT_DEPTH_BIT);

	VK_CHECK(vkCreateImageView(_device, &dview_info, nullptr, &_depthImage.imageView));


	//add to deletion queues
	_mainDeletionQueue.push_function([=]() {
		vkDestroyImageView(_device, _drawImage.imageView, nullptr);
		vmaDestroyImage(_allocator, _drawImage.image, _drawImage.allocation);

		vkDestroyImageView(_device, _depthImage.imageView, nullptr);
		vmaDestroyImage(_allocator, _depthImage.image, _depthImage.allocation);
	});

	


*/


/*




void Swapchain::init(...)
{
    // cria recursos

    _deletionQueue.push_function([this]() {
        vkDestroyImageView(
            _device,
            _drawImage.imageView,
            nullptr
        );

        vmaDestroyImage(
            _allocator,
            _drawImage.image,
            _drawImage.allocation
        );
    });
}

void Swapchain::init(...)
{
    // cria recursos

    _deletionQueue.push_function([this]() {
        vkDestroyImageView(
            _device,
            _drawImage.imageView,
            nullptr
        );

        vmaDestroyImage(
            _allocator,
            _drawImage.image,
            _drawImage.allocation
        );
    });
}
	*/