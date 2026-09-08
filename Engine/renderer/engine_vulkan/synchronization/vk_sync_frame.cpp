#include "vk_sync_frame.h"
#include <engine_vulkan/initialization/vk_initializers.h>

//------------------------------------SyncFrameCommands--------------------------------------------
void SyncFrameCommands::init_descriptor_allocator(){  
	for (int i = 0; i < FRAME_OVERLAP; i++) {
		// create a descriptor pool
		std::vector<DescriptorAllocatorGrowable::PoolSizeRatio> frame_sizes = {
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 3 },

			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 3 },

			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 3 },

			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4 },
		};

		_frames[i]._frameDescriptors = DescriptorAllocatorGrowable{};
		_frames[i]._frameDescriptors.init(_context->device, 1000, frame_sizes);

		_context->mainDeletionQueue.push_function([&, i]() { _frames[i]._frameDescriptors.destroy_pools(_context->device); });
	}
}

void SyncFrameCommands::init_sync()
{
    // create syncronization structures
    // one fence to control when the gpu has finished rendering the frame,
    // and 2 semaphores to syncronize rendering with swapchain
    // we want the fence to start signalled so we can wait on it on the first
    // frame
    VkFenceCreateInfo fenceCreateInfo = vkinit::fence_create_info(VK_FENCE_CREATE_SIGNALED_BIT);  

    for (int i = 0; i < FRAME_OVERLAP; i++) {

        VK_CHECK(vkCreateFence(
            _context->device,
            &fenceCreateInfo, 
            nullptr, 
            &_frames[i]._renderFence )
        );

        VkSemaphoreCreateInfo semaphoreCreateInfo = vkinit::semaphore_create_info();

        VK_CHECK(vkCreateSemaphore(
            _context->device, 
            &semaphoreCreateInfo, 
            nullptr, 
            &_frames[i]._swapchainSemaphore)
        );

        VK_CHECK(vkCreateSemaphore(
            _context->device, 
            &semaphoreCreateInfo, 
            nullptr, 
            &_frames[i]._renderSemaphore )
        );

        _context->mainDeletionQueue.push_function([=]() {
            vkDestroyFence(_context->device, _frames[i]._renderFence, nullptr);
            vkDestroySemaphore(_context->device, _frames[i]._swapchainSemaphore, nullptr);
            vkDestroySemaphore(_context->device, _frames[i]._renderSemaphore, nullptr);
        });
    }

}  

void SyncFrameCommands::init_commands()
{  
    // create a command pool for commands submitted to the graphics queue.
    // we also want the pool to allow for resetting of individual command buffers
    VkCommandPoolCreateInfo commandPoolInfo = 
        vkinit::command_pool_create_info(
            _context->graphicsQueueFamily, 
            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
        );

    // for create the CommandPool and your respective CommandBuffer for each frame
    for (int i = 0; i < FRAME_OVERLAP; i++) {

        VK_CHECK(vkCreateCommandPool(
            _context->device, 
            &commandPoolInfo, 
            nullptr, 
            &_frames[i]._commandPool )
        );

        // allocate the default command buffer that we will use for rendering
        VkCommandBufferAllocateInfo cmdAllocInfo = vkinit::command_buffer_allocate_info(_frames[i]._commandPool, 1);

        VK_CHECK(vkAllocateCommandBuffers(
            _context->device, 
            &cmdAllocInfo, 
            &_frames[i]._commandBuffer )
        );

        _context->mainDeletionQueue.push_function([=]() { 
            vkDestroyCommandPool(_context->device, _frames[i]._commandPool, nullptr); 
        });

    } 
}

void SyncFrameCommands::init(VulkanContext* context)
{
    // Store the context pointer for later use
    _context = context;
    // Initialize the command pool and command buffer
    init_commands();
    // Initialize the semaphores and fences
    init_sync();
    // Initialize the descriptor allocator
    init_descriptor_allocator();
}

void SyncFrameCommands::shutdown(){ 
    for (auto& frame : _frames) frame._deletionQueue.flush(); 
    
    // for (int i = 0; i < FRAME_OVERLAP; i++) {
    //     _frames[i]._frameDescriptors.destroy_pools(_context->device);
    // }
};
 


 