#pragma once
#include "../mesh_asset.h"
 
struct EngineResourceContext; 

std::optional<std::shared_ptr<Model>> load_gltf( 
    EngineResourceContext* contextEng, 
    std::string_view filePath
);