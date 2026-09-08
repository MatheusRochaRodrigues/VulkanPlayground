#include "material_simple_unlitRM.h"

#include <engine_vulkan/initialization/vk_initializers.h>

#include <engine_vulkan/core/vk_context.h>
#include <engine_vulkan/descriptors/vk_descriptors.h>
#include <engine_vulkan/pipelines/vk_pipelines.h>

#include <meshs/gpu_mesh_data.h>
#include <engine_vulkan/core/vk_types.h>

#include <scene/scene_rendererDescriptor.h> //descriptors
#include <scene/render_targetDescriptor.h>  //descriptors

#include <resources/render_targets.h>


void MaterialSimpleUnlitRM::init(
    VulkanContext& context, 
    const SceneRendererDescriptor& sceneRenderer,
    const RenderTargetDescriptor& renderTargets)
{
    create_descriptor_layout(context);

    create_pipelines(context, sceneRenderer, *renderTargets._targets );

    // Por enquanto deixamos os pipelines aqui.
    // Depois podemos separar Pipeline/MaterialPipeline.
}

void MaterialSimpleUnlitRM::create_descriptor_layout( VulkanContext& context) {
    DescriptorLayoutBuilder builder;

    builder.add_binding( 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER           ); 
    builder.add_binding( 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER   ); 
    builder.add_binding( 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER   );

    _materialLayout =
        builder.build( context.device, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT );
}


MaterialInstance MaterialSimpleUnlitRM::write_material(   //createMaterial
    VulkanContext& context,
    DescriptorAllocatorGrowable& descriptorAllocator,   //DescriptorAllocator
    MaterialPass pass,
    const MaterialResources& resources)
{
    MaterialInstance material{};
    material.passType = pass;

    material.pipeline = pass == MaterialPass::Transparent
        ? &_transparentPipeline // yes
        : &_opaquePipeline;     // no

    material.materialSet = descriptorAllocator.allocate( context.device, _materialLayout ); 

    _writer.clear();

    _writer.write_buffer(
        0,
        resources.dataBuffer,
        sizeof(MaterialConstants),
        resources.dataBufferOffset,
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
    );

    _writer.write_image(
        1,
        resources.colorImage.imageView,
        resources.colorSampler,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
    );

    _writer.write_image(
        2,
        resources.metalRoughImage.imageView,
        resources.metalRoughSampler,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
    );

    //update the descriptor set with the new bindings
    _writer.update_set( context.device, material.materialSet);
    
    return material;
} 

void MaterialSimpleUnlitRM::create_pipelines( 
    VulkanContext& context,
    const SceneRendererDescriptor& sceneRenderer,
    const RenderTargets& renderTargets)    // const RenderTargetDescriptor& renderTargets
{
    VkShaderModule meshFragShader{};
    if (!vkutil::load_shader_module("shaders/mesh.frag.spv", context.device, &meshFragShader)) {
        fmt::println( "Error when building mesh fragment shader" );
    }
    VkShaderModule meshVertexShader{};
    if (!vkutil::load_shader_module( "shaders/mesh.vert.spv", context.device, &meshVertexShader)) {
        fmt::println( "Error when building mesh vertex shader" );
    }

    //--------------------------------------------------
    // Pipeline layout
    //--------------------------------------------------

    VkPushConstantRange matrixRange{};
    matrixRange.offset = 0;
    matrixRange.size = sizeof(GPUDrawPushConstants);
    matrixRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayout layouts[] = {
        // set 0
        sceneRenderer.gpuSceneDataLayout(),
        // set 1
        _materialLayout
    };

    VkPipelineLayoutCreateInfo layoutInfo = vkinit::pipeline_layout_create_info();

    layoutInfo.setLayoutCount = 2;
    layoutInfo.pSetLayouts = layouts;

    layoutInfo.pushConstantRangeCount = 1;
    layoutInfo.pPushConstantRanges = &matrixRange;

    VkPipelineLayout pipelineLayout{};

    VK_CHECK(vkCreatePipelineLayout(
        context.device,
        &layoutInfo,
        nullptr,
        &pipelineLayout
    ));

    _opaquePipeline.layout = pipelineLayout;
    _transparentPipeline.layout = pipelineLayout;

    //--------------------------------------------------
    // Pipeline builder
    //--------------------------------------------------

	// build the stage-create-info for both vertex and fragment stages. This lets
	// the pipeline know the shader modules per stage
    PipelineBuilder pipelineBuilder;

    pipelineBuilder.set_shaders( meshVertexShader, meshFragShader );

    pipelineBuilder.set_input_topology( VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST );

    pipelineBuilder.set_polygon_mode( VK_POLYGON_MODE_FILL );

    pipelineBuilder.set_cull_mode( VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE );

    pipelineBuilder.set_multisampling_none();

    pipelineBuilder.disable_blending();

    pipelineBuilder.enable_depthtest( true, VK_COMPARE_OP_GREATER_OR_EQUAL );

	//render format
    pipelineBuilder.set_color_attachment_format( renderTargets.drawImage().imageFormat );
    pipelineBuilder.set_depth_format( renderTargets.depthImage().imageFormat );

	// use the triangle layout we created
    pipelineBuilder._pipelineLayout = pipelineLayout;

    //--------------------------------------------------
    // Opaque
    //--------------------------------------------------
    
	// finally build the pipeline
    _opaquePipeline.pipeline = pipelineBuilder.build_pipeline( context.device );

    //--------------------------------------------------
    // Transparent
    //--------------------------------------------------

	// create the transparent variant
    pipelineBuilder.enable_blending_additive();

    pipelineBuilder.enable_depthtest(
        false,
        VK_COMPARE_OP_GREATER_OR_EQUAL
    );

    _transparentPipeline.pipeline = pipelineBuilder.build_pipeline( context.device );

    //--------------------------------------------------
    // Cleanup shader modules
    //--------------------------------------------------
    vkDestroyShaderModule( context.device, meshFragShader, nullptr ); 
    vkDestroyShaderModule( context.device, meshVertexShader, nullptr );
}