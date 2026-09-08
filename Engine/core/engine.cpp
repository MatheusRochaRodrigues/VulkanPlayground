#include "engine.h"  

#include <chrono>
#include <iostream> 

#include <platform/window.h>   
#include <renderer/engine_vulkan/swapchain/vk_swapchain.h>
#include <input/input.h>   
#include <core/camera.h>   
#include <engine_vulkan/core/vk_context.h> 
#include <renderer/renderer.h>    

#include <VkBootstrap.h>
#include <vulkan/vulkan_core.h>

// #define VMA_IMPLEMENTATION
// #include <vk_mem_alloc.h>

Engine* loadedEngine = nullptr; 
Engine& Engine::Get() { return *loadedEngine; }
 
void Engine::init() { 
    // only one engine initialization is allowed with the application.
    assert(loadedEngine == nullptr);
    loadedEngine = this;

    //WINDOW
    _window = new Window();
    _window->Create("Paper Engine [Vulkan]", 1700, 900);
  
    //RENDERER
    _vulkanContext = new VulkanContext();
    _renderer = new Renderer();  
    _camera = new Camera();
      
    _renderer->init(_window, _vulkanContext, _camera, BuildWithValidationLayers);
 
    //SWAPCHAIN
    _swapchain = new Swapchain();
    _swapchain->init(*_vulkanContext, _window->GetWidth(), _window->GetHeight());
     
    //Input

    //Camera
    _renderer->_swapchain = _swapchain;//teste 
    _window->_camera = _camera;
} 
 
int Engine::run() {    

    while (!_window->ShouldClose()) {  
        _window->PollEvents(); 

        if(_window->ShouldFreezeRendering()) continue;

        if (_window->ResizeRequested()) resize();  

        update(); 
    }
    
    return 0;
} 

void Engine::update() { 
    _camera->update();
    _renderer->run(); 
}

void Engine::resize() {   
    VkExtent2D extent = _window->updateExtent();
    
    //Swapchain
    _swapchain->resize(*_vulkanContext, extent.width, extent.height);     //RecreateSwapchain();

    //Renderer
    // _renderer->resize(extent.width, extent.height);   
}

void Engine::cleanup() {   
    if (loadedEngine != nullptr) { 
        // make sure the gpu has stopped doing its things
        vkDeviceWaitIdle(_vulkanContext->device);

        // loadedScenes.clear();

        _renderer->shutdown();

        _vulkanContext->mainDeletionQueue.flush();

        _swapchain->destroy_swapchain(*_vulkanContext);

        vkDestroySurfaceKHR(_vulkanContext->instance, _vulkanContext->surface, nullptr);

        vmaDestroyAllocator(_vulkanContext->allocator);

        vkDestroyDevice(_vulkanContext->device, nullptr);
        vkb::destroy_debug_utils_messenger(_vulkanContext->instance, _vulkanContext->debugMessenger);
        vkDestroyInstance(_vulkanContext->instance, nullptr);

        _window->Destroy();
    }
}
