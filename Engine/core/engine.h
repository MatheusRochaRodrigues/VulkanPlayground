#pragma once    

class Window;
class VulkanContext;
class Swapchain; 
class Renderer;
class Scene;
class Camera;
class Input;

constexpr bool BuildWithValidationLayers = true;   //  bUseValidationLayers = true;

class Engine {
public:  
    void    init();
    int     run(); 
    void    cleanup();
 
    // singleton style getter.multiple engines is not supported
    static Engine& Get();

private:
    Window*         _window; 
    VulkanContext*  _vulkanContext; 
    Renderer*       _renderer;
    Swapchain*      _swapchain;
    Camera*         _camera;
    Input*          _input;

    void update(); 
    void resize();
};

 