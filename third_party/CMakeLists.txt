
# vk-bootstrap
add_library(vkbootstrap STATIC)

target_sources(vkbootstrap PRIVATE
    vkbootstrap/VkBootstrap.cpp
)

target_include_directories(vkbootstrap PUBLIC
    vkbootstrap
)

target_link_libraries(vkbootstrap PUBLIC
    Vulkan::Vulkan
    ${CMAKE_DL_LIBS}
)

target_compile_features(vkbootstrap PUBLIC cxx_std_20)

# glm (header-only)
add_library(glm INTERFACE)

target_include_directories(glm INTERFACE
    glm
)

# Vulkan Memory Allocator (header-only)
add_library(vma INTERFACE)

target_include_directories(vma INTERFACE
    vma
)

#[[
]]

# stb_image (header-only)
add_library(stb_image INTERFACE)

target_include_directories(stb_image INTERFACE
    stb_image
)

#[[
# fmt
# option 1
add_subdirectory(fmt EXCLUDE_FROM_ALL)

# option 2
add_library(fmt INTERFACE)

target_include_directories(fmt INTERFACE
    fmt/include
)

]]

# fastgltf
# add_subdirectory(fastgltf EXCLUDE_FROM_ALL)


# imgui
add_library(imgui STATIC)

target_sources(imgui PRIVATE

    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp

    imgui/backends/imgui_impl_vulkan.cpp
    imgui/backends/imgui_impl_sdl2.cpp
)

target_include_directories(imgui PUBLIC
    imgui
    imgui/backends
)

target_link_libraries(imgui PUBLIC
    Vulkan::Vulkan
    SDL2::SDL2
)

target_compile_features(imgui PUBLIC cxx_std_20)

