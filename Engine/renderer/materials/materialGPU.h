#pragma once
#include <vulkan/vulkan.h>  
#include <engine_vulkan/core/vk_types.h>
 
enum class MaterialPass :uint8_t {
    MainColor,
    Transparent,
    Other
};

struct MaterialResources
{
    AllocatedImage  colorImage{};
    VkSampler       colorSampler{};

    AllocatedImage  metalRoughImage{};
    VkSampler       metalRoughSampler{};

    VkBuffer        dataBuffer{};
    uint32_t        dataBufferOffset{};
};

struct MaterialPipeline
{
    VkPipeline          pipeline{};
    VkPipelineLayout    layout{};
}; 

 
struct MaterialConstants
{
    glm::vec4       colorFactors;
    glm::vec4       metalRoughFactors;
    // Mandatory Memory Alignment (Padding):
    // Ensures the structure occupies exactly 256 bytes on the GPU,
    // optimizing the space layout for Uniform Buffer Objects (UBO). 
    glm::vec4       extra[14];
};


/*
struct GPUGLTFMaterial {
    glm::vec4 colorFactors;
    glm::vec4 metal_rough_factors;
    glm::vec4 extra[14];
};

static_assert(sizeof(GPUGLTFMaterial) == 256); 
*/

  