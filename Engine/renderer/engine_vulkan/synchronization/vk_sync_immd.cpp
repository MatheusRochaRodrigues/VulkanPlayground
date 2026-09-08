#include "vk_sync_immd.h"
#include <engine_vulkan/initialization/vk_initializers.h>

//------------------------------------ImmediateCommandManager--------------------------------------------
void ImmediateCommandManager::init_sync()
{ 
    VkFenceCreateInfo fenceCreateInfo = vkinit::fence_create_info(VK_FENCE_CREATE_SIGNALED_BIT);
    VK_CHECK(vkCreateFence(
        _context->device, 
        &fenceCreateInfo, 
        nullptr, 
        &_immFence )
    );

    _context->mainDeletionQueue.push_function([=]() { 
        vkDestroyFence(_context->device, _immFence, nullptr); 
    }); 
}  

void ImmediateCommandManager::init(VulkanContext* context)
{ 
    _context = context;

    // create a command pool for commands submitted to the graphics queue.
    // we also want the pool to allow for resetting of individual command buffers
    VkCommandPoolCreateInfo commandPoolInfo = 
        vkinit::command_pool_create_info(
            _context->graphicsQueueFamily, 
            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
        );

    VK_CHECK(vkCreateCommandPool(
        _context->device, 
        &commandPoolInfo, 
        nullptr, 
        &_immCommandPool)
    );

    // allocate the default command buffer that we will use for rendering
    VkCommandBufferAllocateInfo cmdAllocInfo = vkinit::command_buffer_allocate_info(_immCommandPool, 1);

    VK_CHECK(vkAllocateCommandBuffers(
        _context->device, 
        &cmdAllocInfo, 
        &_immCommandBuffer)
    );

    _context->mainDeletionQueue.push_function([=]() { 
        vkDestroyCommandPool(_context->device, _immCommandPool, nullptr); 
    });

    init_sync();
}

void ImmediateCommandManager::immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function)
{
    VK_CHECK(vkResetFences(_context->device, 1, &_immFence));
    VK_CHECK(vkResetCommandBuffer(_immCommandBuffer, 0));

    VkCommandBuffer cmd = _immCommandBuffer;
    // begin the command buffer recording. We will use this command buffer exactly
    // once, so we want to let vulkan know that
    VkCommandBufferBeginInfo cmdBeginInfo = vkinit::command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    VK_CHECK(vkBeginCommandBuffer(cmd, &cmdBeginInfo));

    function(cmd);

    VK_CHECK(vkEndCommandBuffer(cmd));

    VkCommandBufferSubmitInfo cmdinfo = vkinit::command_buffer_submit_info(cmd);
    VkSubmitInfo2 submit = vkinit::submit_info(&cmdinfo, nullptr, nullptr);

    // submit command buffer to the queue and execute it.
    //  _renderFence will now block until the graphic commands finish execution
    VK_CHECK(vkQueueSubmit2(_context->graphicsQueue, 1, &submit, _immFence));

    VK_CHECK(vkWaitForFences(_context->device, 1, &_immFence, true, 9999999999));
} 