#pragma once
#include <engine_vulkan/core/vk_types.h>

//> vbuf_types
struct Vertex { 
	glm::vec3 position;
	float uv_x;
	glm::vec3 normal;
	float uv_y;
	glm::vec4 color;
};

// holds the resources needed for a mesh
struct GPUMeshBuffers {

    AllocatedBuffer indexBuffer;
    AllocatedBuffer vertexBuffer;
    VkDeviceAddress vertexBufferAddress;
};

// push constants for our mesh object draws
struct GPUDrawPushConstants {
    glm::mat4 worldMatrix;
    VkDeviceAddress vertexBuffer;
};
//< vbuf_types


struct GPUSceneData {
    glm::mat4 view;
    glm::mat4 proj;
    glm::mat4 viewproj;
    glm::vec4 ambientColor;
    glm::vec4 sunlightDirection; // w for sun power
    glm::vec4 sunlightColor;
};