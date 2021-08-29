#pragma once

#include "data_types.h"

class Texture {
    int width;
    int height;
};

// https://github.com/KhronosGroup/glTF/blob/master/specification/2.0/schema/material.pbrMetallicRoughness.schema.json
struct Material {
    // material.pbrMetallicRoughness.schema.json
    float4 base_color_factor = float4(1.0, 1.0, 1.0, 1.0);
    float metallic_factor = 1.0;
    float roughness_factor = 1.0;
    Texture *base_color_texture;
    Texture *metallic_roughness_factor;
    
    // material.normalTextureInfo.schema.json
    Texture *normalTexture;
    
    // material.occlusionTextureInfo.schema.json
    Texture *occlusionTexture;
    
    //textureInfo.schema.json
    float3 emissive_factor = float3(0.0, 0.0, 0.0);
    Texture *emissiveTexture;
};