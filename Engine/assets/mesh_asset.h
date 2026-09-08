#pragma once
#include <unordered_map>
#include <filesystem>

#include <engine_vulkan/core/vk_types.h>
#include <engine_vulkan/descriptors/vk_descriptors.h>
#include <materials/material_instance.h>
#include <geometry/bounds.h>
#include <meshs/gpu_mesh_data.h>
#include <scene/renderable.h>
#include <scene/prepare_draw.h>

class EngineResourceContext;

//=========================== VAI EMBORA
// struct GLTFMaterial {
//     MaterialInstance data;
// };
//=========================== VAI EMBORA


struct GeoSurface {
    uint32_t startIndex;
    uint32_t count;
    Bounds bounds;
	std::shared_ptr<MaterialInstance> material;
};

struct MeshAsset {
    std::string name;
    std::vector<GeoSurface> surfaces;
    GPUMeshBuffers meshBuffers;
};
 
struct Model : public IRenderable {	// LoadedGLTF

    // storage for all the data on a given gltf file
    std::unordered_map<std::string, std::shared_ptr<Node>>              nodes;
    std::unordered_map<std::string, std::shared_ptr<MeshAsset>>         meshes;
    std::unordered_map<std::string, AllocatedImage>                     images;
    std::unordered_map<std::string, std::shared_ptr<MaterialInstance>>  materials;

    // nodes that dont have a parent, for iterating through the file in tree order
    std::vector<std::shared_ptr<Node>> 	topNodes;
    std::vector<VkSampler> 				samplers;
    DescriptorAllocatorGrowable 		descriptorPool;
    AllocatedBuffer 					materialDataBuffer;
    EngineResourceContext* 				creator;

    virtual void Draw(const glm::mat4& topMatrix, DrawContext& ctx);
    ~Model() { clearAll(); };

private: 
    void clearAll();
}; 

  
struct MeshNode : public Node {
	std::shared_ptr<MeshAsset> mesh;

	virtual void Draw(const glm::mat4& topMatrix, DrawContext& ctx) override;
};







 
/*
struct ModelAsset
{
    std::string name; 
    std::unordered_map<std::string, std::shared_ptr<MeshAsset>> meshes; 
    std::unordered_map<std::string, std::shared_ptr<MaterialAsset>> materials;

    std::vector<std::shared_ptr<Node>> rootNodes;
};
*/