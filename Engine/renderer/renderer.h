#pragma once 
#include <cstdint>

#include "scene/prepare_draw.h" 
#include <assets/mesh_asset.h>

struct VulkanContext;

class Window;
class Camera; 

//Syncronize commands
class SyncFrameCommands;
class ImmediateCommandManager;

//Resources of render
class RenderTargets;

//Layouts and Sets to setup up the pipeline and render the scene this way
class RenderTargetDescriptor;
class SceneRendererDescriptor;

//Allocator for persistent descriptors that live independently of a frame.
class PersistentDescriptorAllocManager;

//Pipeline / Pass
class BackgroundPass_Compute;  

class MetaData_generics;


struct MaterialSimpleUnlitRM;

class Swapchain;

class Renderer {
public: 
    void init(Window* window, VulkanContext* context, Camera* cam, bool bUseValidationLayers);
    void run();    //run
    void resize(uint32_t w, uint32_t h);
    void shutdown();
  
    Swapchain*                              _swapchain;
private: 
    VulkanContext*                          _ctx; 
    Window*                                 _window; 
    RenderTargets*                          _renderTargets; //resources
    EngineResourceContext*                  _engineResourceContext;

    Camera*                                 _mainCamera;

    SyncFrameCommands*                      _frameManager;   //frames
    ImmediateCommandManager*                _immSubmits;     //ImmediateCommands

    // Data for the current scene being rendered
    RenderTargetDescriptor*                 _drawLayouts;
    SceneRendererDescriptor*                _sceneLayouts;

    PersistentDescriptorAllocManager*       _global_Descriptor;  // persistent descriptors that live independently of a frame.

    //Pass/Pipelines
    BackgroundPass_Compute*                 _backgroundPass_Comp;
     
    // Functions
    void createInstanceVulkan(Window& window, VulkanContext* context, bool bUseValidationLayers);
    void init_resources(Window& window);  
    void init_commands(); 
    void init_descriptor(); 
    void init_pipelines();  //Passes 
    void init_default_data();


    // TEMPORARY
    void draw();    //run
    void update_scene();    //run
    void init_renderables();
    void draw_main(VkCommandBuffer cmd);
    void draw_geometry(VkCommandBuffer cmd);

    //What i'm going to render
    DrawContext 	_drawCommands;
    MetaData_generics* _data;
    std::unordered_map<std::string, std::shared_ptr<Model>> loadedScenes;

    GPUSceneData sceneData;

    MaterialSimpleUnlitRM* metalRoughMaterial;
    float renderScale = 1;
 
};

 