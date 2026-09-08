#pragma once

#include <vulkan/vulkan.h>

#include <engine_vulkan/descriptors/vk_descriptors.h> 
#include "material_instance.h"

struct VulkanContext;
struct DescriptorAllocator;
class SceneRendererDescriptor;
class RenderTargetDescriptor;   
class RenderTargets;   
 
   
struct MaterialSimpleUnlitRM { 
    struct MaterialConstants {
		glm::vec4 colorFactors;
		glm::vec4 metal_rough_factors;
        //padding, we need it anyway for uniform buffers
		glm::vec4 extra[14];
    }; 

    struct MaterialResources {
        AllocatedImage  colorImage; 
        VkSampler       colorSampler;
        AllocatedImage  metalRoughImage;
        VkSampler       metalRoughSampler;
        VkBuffer        dataBuffer; 
        uint32_t        dataBufferOffset;
    };

    void init(
        VulkanContext& context, 
        const SceneRendererDescriptor& sceneRenderer,
        const RenderTargetDescriptor& renderTargets
    );

    MaterialInstance write_material(
        VulkanContext& context,
        DescriptorAllocatorGrowable& descriptorAllocator,
        MaterialPass pass,
        const MaterialResources& resources
    ); 

    VkDescriptorSetLayout descriptor_layout() const { return _materialLayout; }

    void destroy( VulkanContext& context );

private:
    void create_descriptor_layout( VulkanContext& context );  //createDescriptorLayout

    void create_pipelines(
        VulkanContext& context,
        const SceneRendererDescriptor& sceneRenderer,
        const RenderTargets& renderTargets      // RenderTargetDescriptor
    );

private:
    VkDescriptorSetLayout   _materialLayout{};
    DescriptorWriter        _writer;

    MaterialPipeline        _opaquePipeline{};
    MaterialPipeline        _transparentPipeline{};
};