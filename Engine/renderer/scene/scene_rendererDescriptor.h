#pragma once
#include <vulkan/vulkan.h>

class VulkanContext;
class DescriptorAllocator;

class SceneRendererDescriptor {
public:

    void init(
        VulkanContext& context,
        DescriptorAllocator& descriptorAllocator
    );

    void updateDescriptors(
        VulkanContext& context,
        VkBuffer sceneBuffer,
        VkDeviceSize bufferSize
    );

    void destroy(VulkanContext& context);

    VkDescriptorSetLayout gpuSceneDataLayout() const {
        return _gpuSceneDataLayout;
    }

    VkDescriptorSet gpuSceneDataSet() const {
        return _gpuSceneDataSet;
    }

private:
    void createLayouts(VulkanContext& context);

    void createDescriptorSet(
        VulkanContext& context,
        DescriptorAllocator& descriptorAllocator
    );

private:
    // Descriptor contract
    VkDescriptorSetLayout _gpuSceneDataLayout{};

    // Concrete descriptor set
    VkDescriptorSet _gpuSceneDataSet{};
};





/* 
struct SceneRendererLayouts
{
    VkDescriptorSetLayout gpuSceneData{};
    //Lights, etc, PBR;
}; 
    SceneRendererLayouts _layouts{};     //_gpuSceneDataLayout
*/