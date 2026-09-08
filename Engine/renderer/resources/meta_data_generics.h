#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include <vulkan/vulkan.h>
#include <engine_vulkan/core/vk_types.h> 
#include <meshs/gpu_mesh_data.h>

struct VulkanContext;
struct ImmediateCommandManager;

class MetaData_generics {
public:  

    void init_default_data(VulkanContext&, ImmediateCommandManager*);

    GPUMeshBuffers 	_rectangle;

	AllocatedImage 	_whiteImage;
	AllocatedImage 	_blackImage;
	AllocatedImage 	_greyImage;
	AllocatedImage 	_errorCheckerboardImage;
    
	VkSampler 		_defaultSamplerLinear;
	VkSampler 		_defaultSamplerNearest;

};

