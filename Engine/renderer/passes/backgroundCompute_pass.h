#pragma once

#include <vulkan/vulkan.h>


class VulkanContext;
class RenderTargetDescriptor;

struct ComputePushConstants
{
    glm::vec4 data1;
    glm::vec4 data2;
    glm::vec4 data3;
    glm::vec4 data4;
};

struct ComputeEffect
{
    const char* name{};

    VkPipeline pipeline{};
    VkPipelineLayout layout{};

    ComputePushConstants data{};
};

// PASS == PIPELINE

class BackgroundPass_Compute
{
public:
    // COMPUTE PIPELINES
    void initComputePipeline(
        VulkanContext& context,
        const RenderTargetDescriptor& drawRenderer
    );

    // void execute(
    //     VkCommandBuffer commandBuffer,
    //     const RenderTargetDescriptor& drawRenderer
    // );

    // void destroy(
    //     VulkanContext& context
    // );

    ComputeEffect& getBackgroundEffect() {return backgroundEffects[currentBackgroundEffect];};

// private:
    VkPipelineLayout _pipelineLayout {};        //_gradientPipelineLayout
    VkPipeline _pipeline {};                    //_gradientPipeline

	std::vector<ComputeEffect> backgroundEffects;
	int currentBackgroundEffect{ 0 };
    
};
    // ComputeEffect _gradient{};
    // ComputeEffect _sky{};