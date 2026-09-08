
add_executable (engine 
  main.cpp
  vma.cpp
) 

# target_compile_definitions(engine PRIVATE
#     VMA_IMPLEMENTATION
# )

# --------------------------------------------------
# Third-party
# --------------------------------------------------

add_subdirectory(third_party) 

# --------------------------------------------------
# Dependencies Projects
# --------------------------------------------------

add_subdirectory(platform) 
add_subdirectory(core) 
add_subdirectory(renderer)                                    
add_subdirectory(scene) 

add_subdirectory(assets) 
add_subdirectory(geometry)   
add_subdirectory(utils) 


# --------------------------------------------------
# PROJECT SETTINGS
# --------------------------------------------------

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









# file(GLOB_RECURSE ENGINE_SOURCES
#     CONFIGURE_DEPENDS
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
# )

# target_sources(PaperEngineEngine
#     PRIVATE
#         ${ENGINE_SOURCES}
# )





# file(GLOB SOURCES
#     src/*.cpp
# )



  
# target_link_libraries(engine PUBLIC vma glm Vulkan::Vulkan fmt::fmt stb_image SDL2::SDL2 vkbootstrap imgui fastgltf::fastgltf)





  # target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})










  
# target_include_directories(engine PUBLIC
#     "${CMAKE_CURRENT_SOURCE_DIR}"
# )

# target_include_directories(engine PRIVATE
#     "${CMAKE_SOURCE_DIR}/src"
# )




# target_include_directories(engine PUBLIC
#     "${CMAKE_CURRENT_SOURCE_DIR}"
#     "${CMAKE_SOURCE_DIR}/src"
#     "${CMAKE_SOURCE_DIR}/third_party"
# )





# Add source to this project's executable.
# add_executable (engine 
#   main.cpp
#   vk_engine.cpp
#   vk_engine.h
# )
