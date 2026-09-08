#pragma once 

#include <engine_vulkan/core/vk_types.h>
#include <engine_vulkan/descriptors/vk_descriptors.h>
#include <engine_vulkan/core/vk_context.h>

#include <utils/clean_behave.h> 

// FRAME    -> alternate between frames, so we can have multiple frames in flight 

class SyncFrameCommands  {  // syncFrameSubmit   //  FrameManager
public:
    struct FrameData {
        // for synchronization
        VkSemaphore                     _swapchainSemaphore = {};
        VkSemaphore                     _renderSemaphore = {}; 
        VkFence                         _renderFence = {};
        // for descriptor allocations
        DescriptorAllocatorGrowable     _frameDescriptors;  //descriptors   _frameDescriptors
        // for command buffer submission
        VkCommandPool                   _commandPool = {};
        VkCommandBuffer                 _commandBuffer = {};    //_mainCommandBuffer
        // deletion queue for this frame, so we can do a lot of cleanup at once
        DeletionQueue                   _deletionQueue;     // local
    };

    void init(VulkanContext*);
    void shutdown();

    FrameData& get_current_frame() {return _frames[_frameNumber % FRAME_OVERLAP];};
    FrameData& get_last_frame() {return _frames[(_frameNumber - 1) % FRAME_OVERLAP];};
    uint32_t frame_index() const { return _frameIndex; };
    
    void add1frame() {_frameNumber++;};

private:
    std::array<FrameData, FRAME_OVERLAP> _frames;
    uint32_t _frameIndex = 0;
    
    int _frameNumber { 0 };

    VulkanContext* _context;

    //the following functions are used to initialize the frame data
    //Initialize the command pool and command buffer
    void init_commands(); 
    //Initialize the semaphores and fences
    void init_sync();
    //Initialize the descriptor allocator
    void init_descriptor_allocator();
};


 
/*
FrameData& frame = _frames.current();

auto descriptorSet =
    frame.descriptors.allocate(...);
*/