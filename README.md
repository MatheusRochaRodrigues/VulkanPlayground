# VulkanPlayground

A personal playground for learning, experimenting with, and exploring the Vulkan API.

This repository contains experiments, prototypes, and implementations created while studying Vulkan and low-level GPU programming.

## About

The goal of this project is to understand Vulkan by building things from scratch and experimenting with different aspects of modern graphics programming.

Rather than being a finished engine or production-ready framework, VulkanPlayground is a space for learning, testing ideas, and exploring how Vulkan and the GPU work at a lower level.

## What I'm Exploring

- Vulkan initialization and instance/device setup
- Physical and logical devices
- Command buffers and command pools
- Queues and command submission
- Swapchains and presentation
- Render passes and dynamic rendering
- Graphics and compute pipelines
- Descriptor sets and descriptor layouts
- Push constants
- Buffers and images
- GPU memory management
- Vertex and index buffers
- Texture loading and sampling
- GLTF/GLB asset loading
- Shaders and compute shaders
- GPU synchronization
- Vulkan Synchronization 2
- Pipeline barriers
- Multithreading
- GPU performance and optimization

## Goals

The main goal is to gain a deeper understanding of:

- Modern GPU architecture
- Explicit graphics APIs
- CPU-GPU communication
- GPU memory management
- Command submission
- GPU synchronization
- Rendering pipelines
- Resource management
- Low-level performance optimization

## Status

This is an ongoing learning project.

The code may change frequently as I experiment with different approaches and Vulkan features.

Some implementations are intentionally simple or experimental and are mainly intended for learning and exploration.

## Technologies

- C++
- Vulkan
- GLSL
- CMake
- GLFW
- GLM
- GLTF / GLB
- Vulkan Memory Allocator (VMA)
- Dear ImGui

## Why Vulkan?

Vulkan provides a much more explicit interface to the GPU compared to higher-level graphics APIs.

Working with Vulkan makes it possible to explore concepts such as explicit synchronization, command buffers, GPU memory management, resource management, and parallel command submission.

This project is also an opportunity to develop a deeper understanding of what happens between the application, CPU, and GPU when rendering graphics.

## Repository Structure

```text
VulkanPlayground/
├── src/
├── shaders/
├── third_party/
├── CMakeLists.txt
└── README.md
