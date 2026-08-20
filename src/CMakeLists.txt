
# Add source to this project's executable.
# add_executable (engine 
#   main.cpp
#   vk_engine.cpp
#   vk_engine.h
# )

add_executable (engine 
  main.cpp
  vk_types.h
  vk_initializers.cpp
  vk_initializers.h
  vk_images.h
  vk_images.cpp 
  vk_descriptors.h
  vk_descriptors.cpp
  vk_pipelines.h
  vk_pipelines.cpp
  vk_engine.h
  vk_engine.cpp
  vk_loader.h
  vk_loader.cpp
  camera.cpp
  camera.h
)


set_property(TARGET engine PROPERTY CXX_STANDARD 20)
set_property(TARGET engine PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:engine>)

target_compile_definitions(engine PUBLIC GLM_FORCE_DEPTH_ZERO_TO_ONE)
target_include_directories(engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(engine PUBLIC
  Vulkan::Vulkan
  vkbootstrap
  SDL2::SDL2
  vma  
  glm
  fmt::fmt 
  imgui
  stb_image
  fastgltf::fastgltf
) #imgui vkguide_shared fastgltf::fastgltf  stb_image


# target_compile_definitions(engine PRIVATE
#     VK_USE_PLATFORM_XLIB_KHR
# )

target_precompile_headers(engine PUBLIC <optional> <vector> <memory> <string> <vector> <unordered_map> <glm/mat4x4.hpp>  <glm/vec4.hpp> <vulkan/vulkan.h>)


if(WIN32)

    add_custom_command(TARGET engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_RUNTIME_DLLS:engine>
        $<TARGET_FILE_DIR:engine>
        COMMAND_EXPAND_LISTS
    )

endif()





  
# target_link_libraries(engine PUBLIC vma glm Vulkan::Vulkan fmt::fmt stb_image SDL2::SDL2 vkbootstrap imgui fastgltf::fastgltf)
