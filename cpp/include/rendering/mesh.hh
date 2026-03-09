#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace cfd {

struct Vertex {
    float position[3];
    float normal[3];
    float texcoord[2];
    float color[4];
    
    Vertex() {
        position[0] = position[1] = position[2] = 0.0f;
        normal[0] = normal[1] = normal[2] = 0.0f;
        texcoord[0] = texcoord[1] = 0.0f;
        color[0] = color[1] = color[2] = 1.0f;
        color[3] = 1.0f;
    }
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::string name;
    
    uint32_t vao = 0;
    uint32_t vbo = 0;
    uint32_t ebo = 0;
    
    void clear() {
        vertices.clear();
        indices.clear();
    }
    
    size_t vertex_count() const { return vertices.size(); }
    size_t index_count() const { return indices.size(); }
    size_t triangle_count() const { return indices.size() / 3; }
};

}
