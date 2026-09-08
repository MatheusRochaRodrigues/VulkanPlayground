#pragma once 
#include <iostream>

#include <SDL.h>
#include <SDL_vulkan.h>

#include <engine_vulkan/core/vk_types.h>
#include <engine_vulkan/core/vk_context.h>
#include <engine_vulkan/descriptors/vk_descriptors.h>

#include <core/camera.h>   

class Window {
public:
    bool Create(
        const char* title = "Paper Engine [Vulkan]",
        uint32_t width = 1700,
        uint32_t height = 900)
    {
        winName = title;
        extent = { width, height };

        return InitializeSDL();
    };

    bool InitializeSDL() {  
        // We initialize SDL and create a window with it.
        if ( SDL_Init(SDL_INIT_VIDEO) != 0 ) {
            printf("Failed to initialize SDL: %s\n", SDL_GetError());
            return false;
        }

        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);  // windowFlags

        _window = SDL_CreateWindow(
            winName, 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            static_cast<int>(extent.width), 
            static_cast<int>(extent.height), 
            window_flags
        ); 
        // m_window = SDL_CreateWindow("Paper Engine [Vulkan]", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, extent.width, extent.height, window_flags);
    
        
        if (!_window) {
            printf("Failed to create SDL window: %s\n", SDL_GetError()); 
            SDL_Quit();
            return false;
        }

        return true;
    } 

    void CreateSurface(VulkanContext& context) {  
        if (!SDL_Vulkan_CreateSurface(_window, context.instance, &context.surface)) {
            // Handle error
            printf("Failed to create Vulkan surface: %s\n", SDL_GetError());
            assert(false);
        }
    }    

    void PollEvents()
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            inputSDLEvent(event);
        }
    } 

    VkExtent2D updateExtent(){ 
        int w, h;
        
        SDL_Vulkan_GetDrawableSize(_window, &w, &h);      //_window->GetDrawableSize(w, h);  
        /*extentDrawableWin*/extent = {static_cast<uint32_t>(w), static_cast<uint32_t>(h)};  

        SDL_GetWindowSize(_window, &w, &h);         // SDL_GetWindowSizeInPixels()
        extentWindowOpen = {static_cast<uint32_t>(w), static_cast<uint32_t>(h)};    

        ClearResizeRequest(); 
        return extent;
    }

    void inputSDLEvent(SDL_Event& e) { 
        // Handle SDL events here
        // close the window when user alt-f4s or clicks the X button
        if (e.type == SDL_QUIT)
            _shouldClose = true;

        if (e.type == SDL_WINDOWEVENT) { 
            if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                _resizeRequested = true;        //updateExtent();
            }
            if (e.window.event == SDL_WINDOWEVENT_MINIMIZED) {
                _freezeRendering = true;
            }
            if (e.window.event == SDL_WINDOWEVENT_RESTORED) {
                _freezeRendering = false;
            }
        }
        
        if (_freezeRendering) return;

        inputEvents(e);
    }

    void inputEvents(SDL_Event& e) { 
        // Handle input events here  
        _camera->processSDLEvent(e);
    }

    void GetDrawableSize(int& width, int& height) const
    {
        SDL_Vulkan_GetDrawableSize(
            _window,
            &width,
            &height
        );
    }
 
    void GetWindowSize(int& width, int& height) const {
        SDL_GetWindowSize(_window, &width, &height);
    } 

    void Destroy()
    {
        if (_window)
        {
            SDL_DestroyWindow(_window);
            _window = nullptr;
        }

        SDL_Quit();
    } 

    bool ShouldClose() const { return _shouldClose; }
 
    bool ShouldFreezeRendering() { return _freezeRendering; }   //IsMinimized

    bool ResizeRequested() const { return _resizeRequested; } 
    
    void RequestResizeMessage() { _resizeRequested = true; } 

    VkExtent2D GetExtent() const { return extent; }

    SDL_Window* GetNativeWindow() {  return _window;  }       

    
    float GetAspectRatio() const
    {
        if (extent.height == 0) return 0.0f; 
        return static_cast<float>(extent.width) / static_cast<float>(extent.height);
    }

    uint32_t GetWidth() const { return extent.width; } 
    uint32_t GetHeight() const { return extent.height; }
    
    Camera*         _camera;  
private:
    const char* winName;  
    VkExtent2D extent{ 1700, 900 }; // extentDrawableWin

    VkExtent2D extentWindowOpen{ 1700, 900 };
    struct SDL_Window* _window { nullptr };

    bool _shouldClose = false; 
    bool _resizeRequested = false; 
    bool _freezeRendering = false; 
    // bool _windowMinimized = false;

    void ClearResizeRequest() { _resizeRequested = false; } 
    
};





    // void SetWindowViewport0() { glViewport(0, 0, m_width, m_height); }

    // input::MouseState GetMouseState() const { 
    //     input::MouseState mouseState;
    //     int x, y;
    //     Uint32 buttons = SDL_GetMouseState(&x, &y);
    //     mouseState.x = x;
    //     mouseState.y = y;
    //     mouseState.leftButton = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
    //     mouseState.rightButton = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
    //     return mouseState;
    // }