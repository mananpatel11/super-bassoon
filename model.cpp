#include "model.h"

struct Varyings {
    float4 position;
    float3 color;
    float2 texture_coord;
    Varyings() = default;
};

Varyings vertex_shader(const float3 &position,
                       const float3 &color,
                       const float2 &texture_coord,
                       const float4x4 &mvp) {
    Varyings vout;
    vout.position.x = position.x;
    vout.position.y = position.y;
    vout.position.z = position.z;
    vout.position.w = 1;
    vout.position = mvp*vout.position;
    vout.position = vout.position/vout.position.w;
    vout.color = color;
    vout.texture_coord = texture_coord;
    return vout;
}

float3 fragment_shader(const Varyings &frag_in, const std::shared_ptr<Texture> &texture) {
    float2 coord = frag_in.texture_coord;
    float4 c = texture->Sample(coord);
    return float3(c.x, c.y, c.z);
}

float edge_function(const float4 &a, const float4 &b, const float4 &c) {
    float w = (a.x - b.x)*(c.y - b.y) - (a.y - b.y)*(c.x - b.x);
    return w;
}

void Model::draw(FrameBuffer &fb, const float4x4 &view_transform, const float4x4 &projection_matrix) {
    float4x4 mvp = projection_matrix*view_transform*transform;
    std::array<Varyings, 3> vertex_outs;
    int depth_test_failure_count = 0;
    for (int i = 0; i < mesh->num_triangles; i++) {    
        float2 bbmin(INFINITY, INFINITY);
        float2 bbmax(-INFINITY, -INFINITY);
        bool use_indices = (mesh->indices.size() != 0);
        for (int vid = 0; vid < 3; vid++) {
            // run vertex shading
            int index = -1;
            if (use_indices) {
                index = mesh->indices[i*3 + vid];
            } else {
                index = i*3 + vid;
            }
            float3 pos_in = mesh->vertices[index];
            float3 color_in = mesh->colors[index];
            float2 texture_coord = mesh->texcoords[index];
            Varyings vertex_out = vertex_shader(pos_in, color_in, texture_coord, mvp);
            if (vertex_out.position.x < bbmin.x) bbmin.x = vertex_out.position.x;
            if (vertex_out.position.y < bbmin.y) bbmin.y = vertex_out.position.y;
            if (vertex_out.position.x > bbmax.x) bbmax.x = vertex_out.position.x;
            if (vertex_out.position.y > bbmax.y) bbmax.y = vertex_out.position.y;
            vertex_outs[vid] = vertex_out;
        }
        //exit(0);

        // TODO: Clipping
        // FIXME:Some missing steps here after vertex shader is run...

        // Rasterize and shade
        // https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
        float4 p;
        for (int x = 0; x < fb.width; x++) {
            p.x = (x * 2.0/fb.width) - 1.0 + (1.0 / fb.width);
            if (p.x < bbmin.x || p.x > bbmax.x) {
                continue;
            }
            for (int y = 0; y < fb.height; y++) {
                p.y = (y * 2.0/fb.height) - 1.0 + (1.0 / fb.height);
                if (p.y < bbmin.y || p.y > bbmax.y) {
                    continue;
                }
                // Check if pixel is covered by triangle
                float area = edge_function(vertex_outs[0].position, vertex_outs[1].position, vertex_outs[2].position);
                float e01 = edge_function(p, vertex_outs[0].position, vertex_outs[1].position);
                float e12 = edge_function(p, vertex_outs[1].position, vertex_outs[2].position);
                float e20 = edge_function(p, vertex_outs[2].position, vertex_outs[0].position);

                // If pixel is covered
                if ((e01 >= 0) && (e12 >= 0) && (e20 >= 0)) {
                    // Compute barycentrics
                    float w2 = e01/area;
                    float w0 = e12/area;
                    float w1 = e20/area;

                    // Compute Z-value
                    float inverse_z = w0/vertex_outs[0].position.z + w1/vertex_outs[1].position.z + w2/vertex_outs[2].position.z;
                    float new_z = 1/inverse_z;
                    
                    
                    // Do depth testing
                    float old_z = fb.readDepth(Coord2D(x, y));
                    if (new_z < old_z) {         
                        fb.writeDepth(Coord2D(x, y), new_z);

                        // Interpolate varyings using barycentrics computed above
                        Varyings varyings;
                        varyings.position = w0*vertex_outs[0].position + w1*vertex_outs[1].position + w2*vertex_outs[2].position;
                        varyings.position.z = new_z;
                        varyings.color = w0*vertex_outs[0].color + w1*vertex_outs[1].color + w2*vertex_outs[2].color;
                        varyings.texture_coord = w0*vertex_outs[0].texture_coord + w1*vertex_outs[1].texture_coord + w2*vertex_outs[2].texture_coord;
                        // Run fragment shader
                        float3 color = fragment_shader(varyings, material->base_color_texture);
                        fb.writeColor(Coord2D(x, y), color);
                    } else {
                        depth_test_failure_count++;
                    }
                }
            }
        }
    }
    // std::cout << "Failed depth test " << depth_test_failure_count << " times\n";
}