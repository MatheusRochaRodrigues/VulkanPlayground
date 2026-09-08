#pragma once

#include <vulkan/vulkan.h>

class VulkanContext;
class DescriptorAllocator;
class RenderTargets;

//--------------------------------------------------
// Draw Renderer
//--------------------------------------------------

class RenderTargetDescriptor
{
public:
    void init(
        VulkanContext& ctx,
        DescriptorAllocator& descriptorAllocator,
        RenderTargets* renderTargets
    );

    void updateDescriptors(
        VulkanContext& ctx,
        const RenderTargets& renderTargets
    );

    void destroy(VulkanContext& ctx);

    VkDescriptorSetLayout drawImageLayout() const { return _drawImageLayout; } 
    VkDescriptorSet drawImageSet() const { return _drawImageSet; }

    RenderTargets*  _targets;
    
private:
    void createLayouts(VulkanContext& ctx);
    void createDescriptorSet(
        VulkanContext& ctx,
        DescriptorAllocator& descriptorAllocator
    );

private:
    // Descriptor contract
    VkDescriptorSetLayout _drawImageLayout{};

    // Concrete descriptor set
    VkDescriptorSet _drawImageSet{};

};



/*

//--------------------------------------------------
//  Structs
//--------------------------------------------------

struct DrawRendererLayouts
{
    //Lights, etc, PBR;
}; 

struct DrawRendererSets
{
};

DrawRendererLayouts     _descriptor_layouts{};         //_gpuSceneDataLayout
DrawRendererSets        _descriptor_sets{};          //_gpuSceneDataSets

*/