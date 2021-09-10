#include "scene.h"

void Scene::update(const Camera &c) {
    float3 eye(cos(c.yaw) * cos(c.pitch), sin(c.pitch), -sin(c.yaw)*cos(c.pitch));
    eye.x = eye.x*2.5;
    eye.y = eye.y*2.5;
    eye.z = eye.z*2.5;
    float3 at(0, 0, 0);
    float3 up(0, 1, 0);
    view_matrix = lookAtMatrix(eye, at, up);
}

void Scene::Render(FrameBuffer &fb) {
    fb.clear(); 
    for (size_t i = 0; i < models.size(); i++) {
        std::shared_ptr<Model> &m = models[i];
        m->draw(fb, view_matrix, projection_matrix);
    }
}

template<typename T>
std::vector<T> access_data(const json &j, std::string base_path, int accessor_id) {
    std::vector<T> data;
    auto accessor = j["accessors"][accessor_id];
    auto buffer_view = j["bufferViews"][int(accessor["bufferView"])];
    int buffer_index = int(buffer_view["buffer"]);
    std::string bin_uri = j["buffers"][buffer_index]["uri"];
    std::string bin_path = base_path + bin_uri;
    std::ifstream bin_stream(bin_path, std::ios::binary);
    int count = accessor["count"];
    int buffer_view_byte_offset = (buffer_view["byteOffset"] == nullptr)
                                ? 0 : buffer_view["byteOffset"].get<int>();
    int accessor_byte_offset = (accessor["byteOffset"] == nullptr)
                                ? 0 : accessor["byteOffset"].get<int>();
    int byte_offset = buffer_view_byte_offset + accessor_byte_offset;
    int stride = (buffer_view["byteStride"] == nullptr) 
                                ? sizeof(T) : int(buffer_view["byteStride"]);
    bin_stream.seekg(byte_offset);
    for (int i = 0; i < count; i++) {
        T d;
        bin_stream.read(reinterpret_cast<char*>(&d), sizeof(T));
        data.push_back(d);
        bin_stream.seekg(int(bin_stream.tellg()) - sizeof(T) + stride);
    }
    return data;
}

std::vector<std::shared_ptr<Model>> process_node(const json &j,
                                std::string base_path,
                                int node_id,
                                std::vector<std::shared_ptr<Material>> materials) {
    std::vector<std::shared_ptr<Model>> models;
    json node = j["nodes"][node_id];
    // Handle all children
    if (node["children"] != nullptr) {
        for (int child_id : node["children"]) {
            std::vector<std::shared_ptr<Model>> child_models = process_node(j, base_path, child_id, materials);
            models.insert(models.end(), child_models.begin(), child_models.end());
        }
    }
    
    std::shared_ptr<Model> model(nullptr);
    std::shared_ptr<Mesh> mesh(nullptr);

    // If node is a mesh node, create a model
    if (node["mesh"] != nullptr) {
        int mesh_id = node["mesh"];
        auto mesh_j = j["meshes"][mesh_id];
        std::cout << "Mesh = " << mesh << "\n";
        for (auto primitive : mesh_j["primitives"]) {
            std::cout << "Primitive = " << primitive << "\n";
            auto attributes = primitive["attributes"];
            std::cout << "Attributes = " << attributes << "\n";
            // Get positions
            int position_accessor_id = attributes["POSITION"];
            std::vector<float3> positions_data = access_data<float3>(j, 
                                                base_path, position_accessor_id);
            std::cout << "Got positions data\n";
            mesh = std::make_shared<Mesh>(positions_data.size()/3, positions_data);

            // Get indices
            if (primitive["indices"] != nullptr) {
                std::cout << "Indices are present\n";
                std::vector<uint16_t> indices = access_data<uint16_t>(j, 
                                                base_path, primitive["indices"]);
                mesh->indices = indices;
                // FIXME: Hacking correct num_triangles here
                mesh->num_triangles = mesh->indices.size()/3;
            }

            // Get normals       
            if (attributes["NORMAL"] != nullptr) {
                int normal_id = attributes["NORMAL"];
                auto normals_data = access_data<float3>(j, base_path, normal_id);
                mesh->normals = normals_data;
            }
            // Get TEXCOORD_0
            if (attributes["TEXCOORD_0"] != nullptr) {
                int texcoord_id = attributes["TEXCOORD_0"];
                auto texture_coord_data = access_data<float2>(j, base_path, texcoord_id);
                mesh->texcoords = texture_coord_data;
            }

            model = std::make_shared<Model>(mesh);

            // Get material
            if (primitive["material"] != nullptr) {
                int material_id = primitive["material"].get<int>();
                model->material = materials[material_id];    
            }    
            models.push_back(model);
        }
    } else if (node["camera"] != nullptr) {
        std::cout << "Not handling camera ATM\n";
    } else if (node["skin"] != nullptr) {
        std::cout << "Not handling skin ATM\n";
    }
    return models;
}

Scene create_scene_from_gltf(const std::string&& base_path, const std::string&& gltf_file_name) {
    json j;
    std::string gltf_path = base_path + gltf_file_name;
    
    std::ifstream gltf(gltf_path);
    gltf >> j;
    Scene scn;

    // Pre-Create all materials
    std::vector<std::shared_ptr<Material>> materials = CreateMaterials(j, base_path);

    auto scenes = j["scenes"];
    auto nodes = j["nodes"];
    for (auto scene : scenes) {
        std::cout << "Scene = " << scene << "\n";
        for (int node_id : scene["nodes"]) {
            std::vector<std::shared_ptr<Model>> models = process_node(j, base_path, node_id, materials);
            scn.models.insert(scn.models.end(), models.begin(), models.end());
        }
    }
    return scn;
}