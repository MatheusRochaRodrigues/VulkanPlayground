#pragma once 

#include <engine_vulkan/core/vk_types.h>

#include <engine_vulkan/descriptors/vk_descriptors.h>
#include <engine_vulkan/core/vk_context.h> 

#include <utils/clean_behave.h>  

class ImmediateCommandManager         //ImmediateSubmit           ImmediateCommands
{
public:
    void init(VulkanContext*);
    void shutdown();

    void immediate_submit( std::function<void(VkCommandBuffer)>&& function );   // immediate_submit          //execute

private:
    VkCommandPool _immCommandPool{};
    VkCommandBuffer _immCommandBuffer{};
    VkFence _immFence{};                       //_immFence

    VulkanContext* _context;

    void init_sync();
};
 
 