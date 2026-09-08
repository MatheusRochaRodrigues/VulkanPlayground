#include "renderer.h"
#include <iostream>

#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <algorithm>

// #define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#include <engine_vulkan/core/vk_types.h>

#include <engine_vulkan/core/vk_context.h>
#include <engine_vulkan/core/vk_setup.h> 

#include <platform/window.h>

//Data for the current scene being rendered
// #include <engine_vulkan/vk_scene.h>
#include <engine_vulkan/synchronization/vk_sync_frame.h>
#include <engine_vulkan/synchronization/vk_sync_immd.h> 

//Resources
#include <resources/render_targets.h>
#include <resources/meta_data_generics.h>
#include <resources/engine_resource_context.h>

//Layouts
#include <scene/scene_rendererDescriptor.h>
#include <scene/render_targetDescriptor.h>

//Pipeline
#include <passes/backgroundCompute_pass.h>

#include <assets/loader/gltf_loader.h>
#include <renderer/materials/material_simple_unlitRM.h> 

#include <engine_vulkan/swapchain/vk_swapchain.h>

#include <engine_vulkan/initialization/vk_initializers.h>
#include <engine_vulkan/resources/vk_images.h>

#include <engine_vulkan/descriptors/persistent_descriptorAlloc.h>

#include <utils/algVisible.h> 

 
void Renderer::createInstanceVulkan(Window& window, VulkanContext* context, bool bUseValidationLayers){
    setUpContextVulkan::init_instance(window, *context, bUseValidationLayers);
};

void Renderer::init_resources(Window& window){  
    _renderTargets = new RenderTargets();
    _renderTargets->init(*_ctx, window.GetWidth(), window.GetHeight());
};  

void Renderer::init_commands(){  
    _frameManager = new SyncFrameCommands();
    _frameManager->init(_ctx);
    _immSubmits = new ImmediateCommandManager();
    _immSubmits->init(_ctx);
}; 

void Renderer::init_descriptor(){
    //Allocator Descriptor
    _global_Descriptor = new PersistentDescriptorAllocManager();
    _global_Descriptor->init(*_ctx);
    auto& globalDescriptor = _global_Descriptor->get_DescriptorAllocator();

    //Datas
    _drawLayouts  = new RenderTargetDescriptor();
    _drawLayouts->init(*_ctx, globalDescriptor, _renderTargets);

    _sceneLayouts = new SceneRendererDescriptor();
    _sceneLayouts->init(*_ctx, globalDescriptor);
}; 

void Renderer::init_pipelines(){
    std::cout << "b1" << std::endl;
    _backgroundPass_Comp = new BackgroundPass_Compute();
    std::cout << "b2" << std::endl;
    _backgroundPass_Comp->initComputePipeline(*_ctx, *_drawLayouts);
    std::cout << "b3" << std::endl;
}
 
void Renderer::init_default_data(){
    _data = new MetaData_generics();
    _data->init_default_data(*_ctx, _immSubmits);
}

void Renderer::init_renderables()
{  
    std::string structurePath = { "assets/house.glb" };  //structure     house
    auto structureFile = load_gltf(_engineResourceContext, structurePath);

    assert(structureFile.has_value());

    loadedScenes["structure"] = *structureFile;
}

void Renderer::init(Window* window, VulkanContext* context, Camera* cam, bool bUseValidationLayers){
    _ctx = context;
    _mainCamera = cam;
    _window = window;

    metalRoughMaterial = new MaterialSimpleUnlitRM();

    std::cout << "a1" << std::endl;

    createInstanceVulkan(*window, context, bUseValidationLayers);

    std::cout << "a2" << std::endl;
    init_resources(*window);

    std::cout << "a3" << std::endl;
    init_commands();

    std::cout << "a4" << std::endl;
    init_descriptor();

    std::cout << "a5" << std::endl;
    init_pipelines(); 

    std::cout << "a6" << std::endl;
    init_default_data();

    std::cout << "a7" << std::endl;
    metalRoughMaterial->init(*_ctx, *_sceneLayouts, *_drawLayouts);
    // IRenderable
    _engineResourceContext = new EngineResourceContext(context, _immSubmits, _data, metalRoughMaterial); 
    
    std::cout << "a8" << std::endl;
    init_renderables();
    std::cout << "a9" << std::endl;
}; 

void Renderer::resize(uint32_t w, uint32_t h){ 
    _renderTargets->resize(*_ctx, w, h);
}; 

void Renderer::run(){ 
    std::cout << "ab11" << std::endl;
    update_scene();
    std::cout << "ab22" << std::endl;
    
    draw();

    std::cout << "ab33" << std::endl;
    _frameManager->add1frame();
}; 



void Renderer::update_scene()
{
	// mainCamera.update();
    auto _drawExtent = _renderTargets->extent();

	glm::mat4 view = _mainCamera->getViewMatrix();

	// camera projection
	glm::mat4 projection = glm::perspective(
        glm::radians(70.f), 
        (float)_drawExtent.width / (float)_drawExtent.height, 
        10000.f, 
        0.1f);

	// invert the Y direction on projection matrix so that we are more similar
	// to opengl and gltf axis
	projection[1][1] *= -1;

	sceneData.view = view;
	sceneData.proj = projection;
	sceneData.viewproj = projection * view;

    // for (int i = 0; i < 16; i++)         {
        loadedScenes["structure"]->Draw(glm::mat4{ 1.f }, _drawCommands);
    //}
	
}



void Renderer::shutdown(){ 
    // _backgroundPass_Comp->destroy(*_ctx);

    // metalRoughMaterial->destroy(*_ctx);

    _frameManager->shutdown();      //delete _frameManager;

    // Destroy descriptor Allocations
    _global_Descriptor->shutdown(*_ctx);        //_descriptorManager

    // Destroy descriptor layouts
    _drawLayouts->destroy(*_ctx);
    _sceneLayouts->destroy(*_ctx);
}; 








void Renderer::draw()
{
    std::cout << "ab44" << std::endl;
    auto currentFrame = &_frameManager->get_current_frame();
    auto _drawExtent = _renderTargets->extent();

	//wait until the gpu has finished rendering the last frame. Timeout of 1 second
	VK_CHECK(vkWaitForFences(_ctx->device, 1, &currentFrame->_renderFence, true, 1000000000));
  
	currentFrame->_deletionQueue.flush();
    currentFrame->_frameDescriptors.clear_pools(_ctx->device);
	//request image from the swapchain
	uint32_t swapchainImageIndex;

    std::cout << "ab55" << std::endl;
	VkResult e = vkAcquireNextImageKHR(
        _ctx->device, _swapchain->_swapchain, 
        1000000000, 
        currentFrame->_swapchainSemaphore, 
        nullptr, 
        &swapchainImageIndex);
	
    if (e == VK_ERROR_OUT_OF_DATE_KHR) {
        _window->RequestResizeMessage();  //resize_requested = true;
		return ;
	}

    std::cout << "ab66" << std::endl;
	_drawExtent.height = std::min(_swapchain->_imageExtent.height, _drawExtent.height) * renderScale;
	_drawExtent.width = std::min(_swapchain->_imageExtent.width, _drawExtent.width) * renderScale;

	VK_CHECK(vkResetFences(_ctx->device, 1, &currentFrame->_renderFence));

	//now that we are sure that the commands finished executing, we can safely reset the command buffer to begin recording again.
	VK_CHECK(vkResetCommandBuffer(currentFrame->_commandBuffer, 0));

	//naming it cmd for shorter writing
	VkCommandBuffer cmd = currentFrame->_commandBuffer;

	//begin the command buffer recording. We will use this command buffer exactly once, so we want to let vulkan know that
	VkCommandBufferBeginInfo cmdBeginInfo = vkinit::command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	//> draw_first
	VK_CHECK(vkBeginCommandBuffer(cmd, &cmdBeginInfo));

    std::cout << "ab77" << std::endl;
    auto _drawImage = _renderTargets->drawImage();
    auto _depthImage = _renderTargets->depthImage();

	// transition our main draw image into general layout so we can write into it
	// we will overwrite it all so we dont care about what was the older layout
	vkImg::transition_image(*_ctx, cmd, _drawImage.image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);
    vkImg::transition_image(*_ctx, cmd, _depthImage.image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL);

	draw_main(cmd);

	//transtion the draw image and the swapchain image into their correct transfer layouts
	vkImg::transition_image(*_ctx, cmd, _drawImage.image, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
	vkImg::transition_image(*_ctx, cmd, _swapchain->_images[swapchainImageIndex], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	VkExtent2D extent;
	extent.height = _window->GetExtent().height;
	extent.width = _window->GetExtent().width;
	//< draw_first

	//> imgui_draw
    auto& choosenImageSwapchain = _swapchain->_images[swapchainImageIndex];

	// execute a copy from the draw image into the swapchain
    vkImg::copy_image_to_image(*_ctx, cmd, _drawImage.image, choosenImageSwapchain, VkExtent2D(_drawExtent.width, _drawExtent.height) ,_swapchain->_imageExtent);//Extent

	// set swapchain image layout to Attachment Optimal so we can draw it
	vkImg::transition_image(*_ctx, cmd, choosenImageSwapchain, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

	//draw imgui into the swapchain image
	// draw_imgui(cmd, _swapchain->_imageViews[swapchainImageIndex]);

	// set swapchain image layout to Present so we can draw it
	vkImg::transition_image(*_ctx, cmd, choosenImageSwapchain, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

	//finalize the command buffer (we can no longer add commands, but it can now be executed)
	VK_CHECK(vkEndCommandBuffer(cmd));

	//prepare the submission to the queue. 
	//we want to wait on the _presentSemaphore, as that semaphore is signaled when the swapchain is ready
	//we will signal the _renderSemaphore, to signal that rendering has finished

	VkCommandBufferSubmitInfo cmdinfo = vkinit::command_buffer_submit_info(cmd);

	VkSemaphoreSubmitInfo waitInfo = vkinit::semaphore_submit_info(
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR, currentFrame->_swapchainSemaphore);
	VkSemaphoreSubmitInfo signalInfo = vkinit::semaphore_submit_info(
        VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT, currentFrame->_renderSemaphore);

	VkSubmitInfo2 submit = vkinit::submit_info(&cmdinfo, &signalInfo, &waitInfo);

	//submit command buffer to the queue and execute it.
	// _renderFence will now block until the graphic commands finish execution
	VK_CHECK(vkQueueSubmit2(_ctx->graphicsQueue, 1, &submit, currentFrame->_renderFence));

	
    std::cout << "ab88" << std::endl;

	//prepare present
	// this will put the image we just rendered to into the visible window.
	// we want to wait on the _renderSemaphore for that, 
	// as its necessary that drawing commands have finished before the image is displayed to the user
	VkPresentInfoKHR presentInfo = vkinit::present_info();

	presentInfo.pSwapchains = &_swapchain->_swapchain;
	presentInfo.swapchainCount = 1;

	presentInfo.pWaitSemaphores = &currentFrame->_renderSemaphore;
	presentInfo.waitSemaphoreCount = 1;

	presentInfo.pImageIndices = &swapchainImageIndex;

	VkResult presentResult = vkQueuePresentKHR(_ctx->graphicsQueue, &presentInfo);
	if (e == VK_ERROR_OUT_OF_DATE_KHR) {
        _window->RequestResizeMessage();    //resize_requested = true;
        return;
	}
	//increase the number of frames drawn
	// _frameNumber++;

    std::cout << "ab99" << std::endl;
}





void Renderer::draw_main(VkCommandBuffer cmd)
{
    // ADAPTATION TEMPORARY
	ComputeEffect& effect = _backgroundPass_Comp->getBackgroundEffect();
    auto& _gradientPipelineLayout = _backgroundPass_Comp->_pipelineLayout;
    auto& _gradientPipeline = _backgroundPass_Comp->_pipeline;
    auto _drawImageDescriptors = _drawLayouts->drawImageSet();
    auto _windowExtent = _window->GetExtent();
    auto _drawImage = _renderTargets->drawImage();
    auto _depthImage = _renderTargets->depthImage();
    //===============================================================


	// bind the background compute pipeline
	vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, effect.pipeline);

	// bind the descriptor set containing the draw image for the compute pipeline
	vkCmdBindDescriptorSets(
        cmd, 
        VK_PIPELINE_BIND_POINT_COMPUTE, 
        _gradientPipelineLayout, 
        0, 
        1, 
        &_drawImageDescriptors, 
        0, 
        nullptr);

	vkCmdPushConstants(cmd, _gradientPipelineLayout, VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(ComputePushConstants), &effect.data);
	// execute the compute pipeline dispatch. We are using 16x16 workgroup size so we need to divide by it
	vkCmdDispatch(cmd, std::ceil(_windowExtent.width / 16.0), std::ceil(_windowExtent.height / 16.0), 1);

	//draw the triangle

    vkImg::transition_image(*_ctx, cmd, _drawImage.image, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

	VkRenderingAttachmentInfo colorAttachment = vkinit::attachment_info(_drawImage.imageView, nullptr, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	VkRenderingAttachmentInfo depthAttachment = vkinit::depth_attachment_info(_depthImage.imageView, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL);

	VkRenderingInfo renderInfo = vkinit::rendering_info(_windowExtent, &colorAttachment, &depthAttachment);

	vkCmdBeginRendering(cmd, &renderInfo);
	// auto start = std::chrono::system_clock::now();
	draw_geometry(cmd);

	// auto end = std::chrono::system_clock::now();
	// auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	// stats.mesh_draw_time = elapsed.count() / 1000.f;

	vkCmdEndRendering(cmd);
}




void Renderer::draw_geometry(VkCommandBuffer cmd)
{
    auto _windowExtent = _window->GetExtent();
    auto currentFrame = &_frameManager->get_current_frame();



    std::vector<uint32_t> opaque_draws;
    opaque_draws.reserve(_drawCommands.OpaqueSurfaces.size());

    for (int i = 0; i < _drawCommands.OpaqueSurfaces.size(); i++) {
       if (is_visible(_drawCommands.OpaqueSurfaces[i], sceneData.viewproj)) {
            opaque_draws.push_back(i);
       }
    }

    // sort the opaque surfaces by material and mesh
    std::sort(opaque_draws.begin(), opaque_draws.end(), [&](const auto& iA, const auto& iB) {
		const RenderObject& A = _drawCommands.OpaqueSurfaces[iA];
		const RenderObject& B = _drawCommands.OpaqueSurfaces[iB];
        if (A.material == B.material) {
            return A.indexBuffer < B.indexBuffer;
        } else {
            return A.material < B.material;
        }
    });

    //allocate a new uniform buffer for the scene data
    // AllocatedBuffer gpuSceneDataBuffer = create_buffer(sizeof(GPUSceneData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
    AllocatedBuffer gpuSceneDataBuffer = _engineResourceContext->create_buffer(
        sizeof(GPUSceneData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

    //add it to the deletion queue of this frame so it gets deleted once its been used
    currentFrame->_deletionQueue.push_function([=,this](){
        // destroy_buffer(gpuSceneDataBuffer);
        _engineResourceContext->destroy_buffer(gpuSceneDataBuffer);
    });

    //write the buffer
    // GPUSceneData* sceneUniformData = (GPUSceneData*)gpuSceneDataBuffer.allocation->GetMappedData();
    GPUSceneData* sceneUniformData = static_cast<GPUSceneData*>(gpuSceneDataBuffer.info.pMappedData);
    
    *sceneUniformData = sceneData;

    //create a descriptor set that binds that buffer and update it
    VkDescriptorSet globalDescriptor = currentFrame->_frameDescriptors.allocate(_ctx->device, _sceneLayouts->gpuSceneDataLayout());    //_gpuSceneDataDescriptorLayout

	DescriptorWriter writer;
	writer.write_buffer(0, gpuSceneDataBuffer.buffer, sizeof(GPUSceneData), 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
	writer.update_set(_ctx->device, globalDescriptor);

    MaterialPipeline* lastPipeline = nullptr;
    MaterialInstance* lastMaterial = nullptr;
    VkBuffer lastIndexBuffer = VK_NULL_HANDLE;

    auto draw = [&](const RenderObject& r) {
        if (r.material != lastMaterial) {
            lastMaterial = r.material;
            if (r.material->pipeline != lastPipeline) {

                lastPipeline = r.material->pipeline;
                vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, r.material->pipeline->pipeline);
                vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,r.material->pipeline->layout, 0, 1,
                    &globalDescriptor, 0, nullptr);

				VkViewport viewport = {};
				viewport.x = 0;
				viewport.y = 0;
				viewport.width = (float)_windowExtent.width;
				viewport.height = (float)_windowExtent.height;
				viewport.minDepth = 0.f;
				viewport.maxDepth = 1.f;

				vkCmdSetViewport(cmd, 0, 1, &viewport);

				VkRect2D scissor = {};
				scissor.offset.x = 0;
				scissor.offset.y = 0;
				scissor.extent.width = _windowExtent.width;
				scissor.extent.height = _windowExtent.height;

				vkCmdSetScissor(cmd, 0, 1, &scissor);
            }

            vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, r.material->pipeline->layout, 1, 1,
                &r.material->materialSet, 0, nullptr);
        }
        if (r.indexBuffer != lastIndexBuffer) {
            lastIndexBuffer = r.indexBuffer;
            vkCmdBindIndexBuffer(cmd, r.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        }
        // calculate final mesh matrix
        GPUDrawPushConstants push_constants;
        push_constants.worldMatrix = r.transform;
        push_constants.vertexBuffer = r.vertexBufferAddress;

        vkCmdPushConstants(cmd, r.material->pipeline->layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(GPUDrawPushConstants), &push_constants);

        // stats.drawcall_count++;
        // stats.triangle_count += r.indexCount / 3;
        vkCmdDrawIndexed(cmd, r.indexCount, 1, r.firstIndex, 0, 0);
    };

    // stats.drawcall_count = 0;
    // stats.triangle_count = 0;

    for (auto& r : opaque_draws) {
        draw(_drawCommands.OpaqueSurfaces[r]);
    }

    for (auto& r : _drawCommands.TransparentSurfaces) {
        draw(r);
    }

    // we delete the draw commands now that we processed them
    _drawCommands.OpaqueSurfaces.clear();
    _drawCommands.TransparentSurfaces.clear();
}









/*
void Renderer::init()
{
    _context.init();
    _swapchain.init(_context);

    _persistentDescriptors.init_pool(...);

    _renderTargets.init(
        _context,
        width,
        height
    );

    _RenderTargetDescriptor.init(
        _context,
        _persistentDescriptors,
        _renderTargets
    );

    _SceneRendererDescriptor.init(
        _context,
        _persistentDescriptors
    );

    _materialManager.init(
        _context,
        _persistentDescriptors,
        _SceneRendererDescriptor,
        _renderTargets
    );

    _backgroundPass.init(
        _context,
        _RenderTargetDescriptor
    );
}
*/
