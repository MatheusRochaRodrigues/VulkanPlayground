// #pragma once
// #include 

// struct MaterialAsset
// {
//     // dados vindos do GLTF
//     glm::vec4 colorFactor{1.0f};
//     float metallicFactor{1.0f};
//     float roughnessFactor{1.0f};

//     AssetHandle colorTexture;
//     AssetHandle metalRoughTexture;
//     AssetHandle normalTexture;
// };

// /*
// GLTF
//  │
//  ▼
// MaterialAsset       ← dados do arquivo
//  │
//  ▼
// MaterialManager
//  │
//  ▼
// MaterialInstance    ← GPU/Vulkan
// */