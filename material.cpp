#include "material.h"

using json = nlohmann::json; 

// Create all samplers from json
std::vector<std::shared_ptr<Sampler>> CreateSamplers(const json &j) {
    std::vector<std::shared_ptr<Sampler>> samplers;
    if (j["samplers"] == nullptr) {
        return samplers;
    }
    for (auto sampler_j : j["samplers"]) {
        std::cout << "Processing sampler " << sampler_j << "\n";
        int mag_filter = sampler_j["magFilter"];   
        int min_filter = sampler_j["minFilter"];   
        int wrap_s = sampler_j["wrapS"];   
        int wrap_t = sampler_j["wrapT"];
        auto sampler = std::make_shared<Sampler>(mag_filter, min_filter, wrap_s, wrap_t);
        samplers.push_back(sampler);  
    }
    return samplers;
}

// Create all textures from json
std::vector<std::shared_ptr<Texture>> CreateTextures(const json &j, const std::string &base_path) {
    std::vector<std::shared_ptr<Texture>> textures;
    if (j["textures"] == nullptr) {
        return textures;
    }
    auto samplers = CreateSamplers(j);
    std::shared_ptr<Sampler> default_sampler = std::make_shared<Sampler>();
    auto images_j = j["images"];
    for (auto texture_j : j["textures"]) {
        std::cout << "Processing texture - " << texture_j << "\n";
        // Find sampler
        std::shared_ptr<Sampler> sampler(nullptr);
        if (texture_j["sampler"] != nullptr) {
            int sampler_id = texture_j["sampler"];
            sampler = samplers[sampler_id];
        } else {
            sampler = default_sampler;
        }
        // Load texture data from image
        if (texture_j["source"] != nullptr) {
            unsigned int width;
            unsigned int height;
            std::vector<unsigned char> bytes;
            int image_id = texture_j["source"];
            std::string uri = images_j[image_id]["uri"];
            std::string image_path = base_path + uri;
            unsigned error = lodepng::decode(bytes, width, height, image_path);
            if (error) {
                std::cerr << error << "\n";
                std::cerr << "Unable to open file " << image_path << "\n";
                exit(-1);
            }
            std::vector<float4> colors;
            for (uint i = 0; i < width*height; i++) {
                unsigned char r = bytes[i*4 + 0];
                unsigned char g = bytes[i*4 + 1];
                unsigned char b = bytes[i*4 + 2];
                unsigned char a = bytes[i*4 + 3];
                float4 color = float4(r, g, b, a)/256;
                colors.push_back(color);
            }
            auto texture = std::make_shared<Texture>(width, height, colors, sampler);
            textures.push_back(texture);
        } else {
            std::cerr << "No texture source provided\n";
            exit(-1);
        }
    }
    return textures;
}

// Create all materials from json object
std::vector<std::shared_ptr<Material>> CreateMaterials(const json &j, const std::string &base_path) {
    std::vector<std::shared_ptr<Material>> materials;
    // early exit if materials don't exist in the gltf file
    if (j["materials"] == nullptr) {
        return materials;
    }
    auto textures = CreateTextures(j, base_path);
    for (auto material_j : j["materials"]) {
        std::cout << "Processing material - " << material_j << "\n";
        auto material = std::make_shared<Material>();
        // Process pbrMetallicRoughness
        if (material_j["pbrMetallicRoughness"] != nullptr) {
            auto pbrMetallicRoughness_j = material_j["pbrMetallicRoughness"];
            material->metallic_factor = (pbrMetallicRoughness_j["metallicFactor"] == nullptr) ?
                                            1.0 : pbrMetallicRoughness_j["metallicFactor"].get<float>();
            material->roughness_factor = (pbrMetallicRoughness_j["roughnessFactor"] == nullptr) ?
                                            1.0 : pbrMetallicRoughness_j["roughnessFactor"].get<float>();
            if (pbrMetallicRoughness_j["baseColorTexture"] != nullptr) {
                auto baseColorTexture_j = pbrMetallicRoughness_j["baseColorTexture"];
                material->base_color_texture = textures[baseColorTexture_j["index"]];
            }
        }
        materials.push_back(material);

    }
    return materials;
}

float4 Texture::Sample(const float2 &coord) {
    float x = coord.x;
    float y = coord.y;
    int pix_x = x * width;
    int pix_y = y * height;
    
    int pix_location = pix_y*width + pix_x;
    return colors[pix_location];
}