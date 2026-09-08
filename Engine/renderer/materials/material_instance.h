#pragma once
#include "materialGPU.h"

struct MaterialInstance
{
    MaterialPipeline*   pipeline{};
    VkDescriptorSet     materialSet{};
    MaterialPass        passType{};          
};

// struct GLTFMaterial {
//     MaterialInstance data;
// };