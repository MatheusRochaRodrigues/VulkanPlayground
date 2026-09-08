#include "meta_data_generics.h"

#include <glm/glm.hpp> 

#include <engine_vulkan/core/vk_context.h>
#include <engine_vulkan/initialization/vk_initializers.h>
#include <meshs/gpu_mesh_data.h>
#include <engine_vulkan/resources/vk_buffer.h>
#include <engine_vulkan/resources/vk_images.h> 

void MetaData_generics::init_default_data(VulkanContext& ctx, ImmediateCommandManager* imm) {
	std::array<Vertex, 4> rect_vertices;

	rect_vertices[0].position = { 0.5,-0.5, 0 };
	rect_vertices[1].position = { 0.5,0.5, 0 };
	rect_vertices[2].position = { -0.5,-0.5, 0 };
	rect_vertices[3].position = { -0.5,0.5, 0 };

	rect_vertices[0].color = { 0,0, 0,1 };
	rect_vertices[1].color = { 0.5,0.5,0.5 ,1 };
	rect_vertices[2].color = { 1,0, 0,1 };
	rect_vertices[3].color = { 0,1, 0,1 };

	rect_vertices[0].uv_x = 1;
	rect_vertices[0].uv_y = 0;
	rect_vertices[1].uv_x = 0;
	rect_vertices[1].uv_y = 0;
	rect_vertices[2].uv_x = 1;
	rect_vertices[2].uv_y = 1;
	rect_vertices[3].uv_x = 0;
	rect_vertices[3].uv_y = 1;

	std::array<uint32_t, 6> rect_indices;

	rect_indices[0] = 0;
	rect_indices[1] = 1;
	rect_indices[2] = 2;

	rect_indices[3] = 2;
	rect_indices[4] = 1;
	rect_indices[5] = 3;

	_rectangle = vkBuffer::uploadMesh(ctx, imm, rect_indices, rect_vertices);

	//3 default textures, white, grey, black. 1 pixel each
	uint32_t white = glm::packUnorm4x8(glm::vec4(1, 1, 1, 1));
	_whiteImage = vkImg::create_image(
		(void*)&white,
		ctx, *imm, 
		VkExtent3D{ 1, 1, 1 }, 
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_IMAGE_USAGE_SAMPLED_BIT);

	uint32_t grey = glm::packUnorm4x8(glm::vec4(0.66f, 0.66f, 0.66f, 1));
	_greyImage = vkImg::create_image(
		(void*)&grey,
		ctx, *imm,  
		VkExtent3D{ 1, 1, 1 }, 
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_IMAGE_USAGE_SAMPLED_BIT);

	uint32_t black = glm::packUnorm4x8(glm::vec4(0, 0, 0, 0));
	_blackImage = vkImg::create_image(
		(void*)&black,
		ctx, *imm,  
		VkExtent3D{ 1, 1, 1 }, 
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_IMAGE_USAGE_SAMPLED_BIT);

	//checkerboard image
	uint32_t magenta = glm::packUnorm4x8(glm::vec4(1, 0, 1, 1));
	std::array<uint32_t, 16 * 16 > pixels; //for 16x16 checkerboard texture
	for (int x = 0; x < 16; x++) {
		for (int y = 0; y < 16; y++) {
			pixels[y * 16 + x] = ((x % 2) ^ (y % 2)) ? magenta : black;
		}
	}

	_errorCheckerboardImage = 
		vkImg::create_image(
			pixels.data(), 
			ctx, *imm, 
			VkExtent3D{ 16, 16, 1 }, 
			VK_FORMAT_R8G8B8A8_UNORM,
			VK_IMAGE_USAGE_SAMPLED_BIT
		);

	VkSamplerCreateInfo sampl = { .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };

	sampl.magFilter = VK_FILTER_NEAREST;
	sampl.minFilter = VK_FILTER_NEAREST;

	vkCreateSampler(ctx.device, &sampl, nullptr, &_defaultSamplerNearest);

	sampl.magFilter = VK_FILTER_LINEAR;
	sampl.minFilter = VK_FILTER_LINEAR;
	vkCreateSampler(ctx.device, &sampl, nullptr, &_defaultSamplerLinear);
}