#include "backgroundCompute_pass.h"
#include <scene/render_targetDescriptor.h> 
#include <engine_vulkan/initialization/vk_initializers.h>
#include <engine_vulkan/core/vk_types.h>
#include <engine_vulkan/core/vk_context.h>

#include <engine_vulkan/pipelines/vk_pipelines.h>

void BackgroundPass_Compute::initComputePipeline( VulkanContext& context, const RenderTargetDescriptor& renderTargetDesc) 
{
    VkDescriptorSetLayout drawImageLayout = renderTargetDesc.drawImageLayout();

    VkPipelineLayoutCreateInfo comp_LayoutInfo{};   
    comp_LayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	comp_LayoutInfo.pNext = nullptr;
    comp_LayoutInfo.setLayoutCount = 1;
    comp_LayoutInfo.pSetLayouts = &drawImageLayout;

    VkPushConstantRange pushConstant{};
    pushConstant.offset = 0;
    pushConstant.size = sizeof(ComputePushConstants);
    pushConstant.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    comp_LayoutInfo.pushConstantRangeCount = 1;
    comp_LayoutInfo.pPushConstantRanges = &pushConstant;

    // Create pipeline layout -> defines how shader data is accessed
    VK_CHECK(vkCreatePipelineLayout(
        context.device,
        &comp_LayoutInfo,
        nullptr,
        &_pipelineLayout
    ));

    //Shader
	VkShaderModule gradientShader;
	if (!vkutil::load_shader_module("shaders/gradient_color.comp.spv", context.device, &gradientShader)) {
		fmt::print("Error when building the compute shader \n");
	} 

	VkShaderModule skyShader;
	if (!vkutil::load_shader_module("shaders/sky.comp.spv", context.device, &skyShader)) {
        fmt::print("Error when building the compute shader\n");
	}

	VkPipelineShaderStageCreateInfo stageinfo{};
	stageinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	stageinfo.pNext = nullptr;
	stageinfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
	stageinfo.module = gradientShader;
	stageinfo.pName = "main";

	VkComputePipelineCreateInfo computePipelineCreateInfo{};
	computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	computePipelineCreateInfo.pNext = nullptr;
	computePipelineCreateInfo.layout = _pipelineLayout;
	computePipelineCreateInfo.stage = stageinfo;
 
	ComputeEffect gradient;
	gradient.layout = _pipelineLayout;
	gradient.name = "gradient";
	gradient.data = {};

	//default colors
	gradient.data.data1 = glm::vec4(1, 0, 0, 1);
	gradient.data.data2 = glm::vec4(0, 0, 1, 1);

	VK_CHECK(vkCreateComputePipelines(context.device, VK_NULL_HANDLE, 1, &computePipelineCreateInfo, nullptr, &gradient.pipeline));

	//change the shader module only to create the sky shader
	computePipelineCreateInfo.stage.module = skyShader;

	ComputeEffect sky;
	sky.layout = _pipelineLayout;
	sky.name = "sky";
	sky.data = {};
	//default sky parameters
	sky.data.data1 = glm::vec4(0.1, 0.2, 0.4, 0.97);

	VK_CHECK(vkCreateComputePipelines(context.device, VK_NULL_HANDLE, 1, &computePipelineCreateInfo, nullptr, &sky.pipeline));

	//add the 2 background effects into the array
	backgroundEffects.push_back(gradient);
	backgroundEffects.push_back(sky);

	//destroy structures properly
	vkDestroyShaderModule(context.device, gradientShader, nullptr);
	vkDestroyShaderModule(context.device, skyShader, nullptr);

	context.mainDeletionQueue.push_function([&]() {
		vkDestroyPipelineLayout(context.device, _pipelineLayout, nullptr);
		vkDestroyPipeline(context.device, sky.pipeline, nullptr);
		vkDestroyPipeline(context.device, gradient.pipeline, nullptr);
    });
        
}

 