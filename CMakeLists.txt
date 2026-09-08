cmake_minimum_required(VERSION 3.20)

project(PaperEngine LANGUAGES C CXX)

# --------------------------------------------------
# C++ standard
# --------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# VSCode 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --------------------------------------------------
# Output directories
# --------------------------------------------------

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/bin
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/bin
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/bin
)

# --------------------------------------------------
# Dependencies
# --------------------------------------------------

find_package(Vulkan REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Threads REQUIRED)
find_package(fmt REQUIRED)

# --------------------------------------------------
# Third-party
# --------------------------------------------------

# add_subdirectory(third_party)

# --------------------------------------------------
# Project modules
# --------------------------------------------------

add_subdirectory(Engine)
# add_subdirectory(editor)
# add_subdirectory(game)

# CUIDADO DESABILITANDO TEMPORARIAMENTE WARNINGS DE POINTERS NULLABLE PARA CALAR A BBOCA DO VMA
target_compile_options(engine PRIVATE
    -Wno-nullability-completeness
)

# --------------------------------------------------
# Shaders
# --------------------------------------------------

find_program(
    GLSLC
    NAMES glslc
    HINTS
        /usr/bin
        /usr/local/bin
)

if(NOT GLSLC)
    message(FATAL_ERROR "glslc not found. Install shaderc package.")
endif()

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/Engine/shaders/*.vert"
    "${PROJECT_SOURCE_DIR}/Engine/shaders/*.frag"
    "${PROJECT_SOURCE_DIR}/Engine/shaders/*.comp"
)

set(SPIRV_BINARY_FILES "")

foreach(GLSL ${GLSL_SOURCE_FILES})

    get_filename_component(FILE_NAME ${GLSL} NAME)

    set(SPIRV
        "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv"
    )

    add_custom_command(
        OUTPUT ${SPIRV}

        COMMAND
            ${GLSLC}
            ${GLSL}
            -o
            ${SPIRV}

        DEPENDS ${GLSL}

        COMMENT "Compiling shader ${FILE_NAME}"
    )

    list(APPEND SPIRV_BINARY_FILES ${SPIRV})

endforeach()

add_custom_target(
    Shaders
    DEPENDS ${SPIRV_BINARY_FILES}
)

add_dependencies(engine Shaders)

# --------------------------------------------------
# Linux info
# --------------------------------------------------

message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CXX Compiler: ${CMAKE_CXX_COMPILER}")