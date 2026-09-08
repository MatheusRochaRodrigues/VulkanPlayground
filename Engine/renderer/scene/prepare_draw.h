#pragma once
#include <materials/material_instance.h>
#include <geometry/bounds.h>

struct RenderObject {
    uint32_t indexCount;
    uint32_t firstIndex;
    VkBuffer indexBuffer;
    
    MaterialInstance* material;
    Bounds bounds;
    glm::mat4 transform;
    VkDeviceAddress vertexBufferAddress;
}; 

struct DrawContext {
    std::vector<RenderObject> OpaqueSurfaces;
    std::vector<RenderObject> TransparentSurfaces;
}; 