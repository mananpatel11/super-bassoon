#pragma once

#include <iostream>
#include <vector>

// External dependencies
#include "json.hpp"
#include "lodepng.h"
#include "data_types.h"

using json = nlohmann::json;

struct Sampler {
    Sampler(int _mag_filter, int _min_filter, int _wrap_s, int _wrap_t) : 
        mag_filter(_mag_filter), min_filter(_min_filter), wrap_s(_wrap_s), wrap_t(_wrap_t) {}
    Sampler() {}
    int mag_filter = 9728;  // NEAREST
    int min_filter = 9728;  // NEAREST
    int wrap_s = 10497;     // REPEAT
    int wrap_t = 10497;     // REPEAT
}; 

struct Texture {
    Texture(int _width, int _height, std::vector<float4> _colors, std::shared_ptr<Sampler> _sampler) :
            width(_width), height(_height), colors(_colors), sampler(_sampler) {};
    unsigned int width;
    unsigned int height;
    std::vector<float4> colors;
    std::shared_ptr<Sampler> sampler;
    float4 Sample(const float2 &coord);
};

// https://github.com/KhronosGroup/glTF/blob/master/specification/2.0/schema/material.pbrMetallicRoughness.schema.json
class Material {
    public:
    // material.pbrMetallicRoughness.schema.json
    float4 base_color_factor = float4(1.0, 1.0, 1.0, 1.0);
    float metallic_factor = 1.0;
    float roughness_factor = 1.0;
    std::shared_ptr<Texture> base_color_texture = nullptr;
    std::shared_ptr<Texture> metallic_roughness_factor = nullptr;
    
    // material.normalTextureInfo.schema.json
    std::shared_ptr<Texture> normal_texture = nullptr;
    
    // material.occlusionTextureInfo.schema.json
    std::shared_ptr<Texture> occlusion_texture = nullptr;
    
    //textureInfo.schema.json
    float3 emissive_factor = float3(0.0, 0.0, 0.0);
    std::shared_ptr<Texture> emissive_texture = nullptr;
};

// Create all materials from json object
std::vector<std::shared_ptr<Material>> CreateMaterials(const json &j, const std::string &base_path);