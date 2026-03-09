#include <geometry/scramjet_geometry.hh>
#include <utils/logger.hh>
#include <utils/math.hh>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <cmath>
#include <algorithm>

namespace cfd {

ScramjetGeometry::ScramjetGeometry() {
    LOG_INFO("ScramjetGeometry created");
}

ScramjetGeometry::~ScramjetGeometry() {
    if (external_mesh_.vao) glDeleteVertexArrays(1, &external_mesh_.vao);
    if (external_mesh_.vbo) glDeleteBuffers(1, &external_mesh_.vbo);
    if (external_mesh_.ebo) glDeleteBuffers(1, &external_mesh_.ebo);
}

void ScramjetGeometry::generate(const ScramjetConfig& config) {
    config_ = config;
    needs_regeneration_ = true;
    regenerate();
}

void ScramjetGeometry::regenerate() {
    external_mesh_.clear();
    internal_mesh_.clear();
    inlet_mesh_.clear();
    combustor_mesh_.clear();
    nozzle_mesh_.clear();
    
    generate_inlet();
    generate_combustor();
    generate_nozzle();
    generate_struts();
    generate_fuel_injectors();
    generate_internal_ducts();
    compute_flow_path();
    compute_thermal_profiles();
    
    glGenVertexArrays(1, &external_mesh_.vao);
    glGenBuffers(1, &external_mesh_.vbo);
    glGenBuffers(1, &external_mesh_.ebo);
    
    glBindVertexArray(external_mesh_.vao);
    glBindBuffer(GL_ARRAY_BUFFER, external_mesh_.vbo);
    glBufferData(GL_ARRAY_BUFFER, external_mesh_.vertices.size() * sizeof(Vertex),
                external_mesh_.vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, external_mesh_.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, external_mesh_.indices.size() * sizeof(uint32_t),
                external_mesh_.indices.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)*6));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)*8));
    
    glBindVertexArray(0);
    
    glGenVertexArrays(1, &internal_mesh_.vao);
    glGenBuffers(1, &internal_mesh_.vbo);
    glGenBuffers(1, &internal_mesh_.ebo);
    
    glBindVertexArray(internal_mesh_.vao);
    glBindBuffer(GL_ARRAY_BUFFER, internal_mesh_.vbo);
    glBufferData(GL_ARRAY_BUFFER, internal_mesh_.vertices.size() * sizeof(Vertex),
                internal_mesh_.vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, internal_mesh_.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, internal_mesh_.indices.size() * sizeof(uint32_t),
                internal_mesh_.indices.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)*6));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)*8));
    
    glBindVertexArray(0);
    
    needs_regeneration_ = false;
    LOG_INFO("Scramjet geometry regenerated with {} vertices", external_mesh_.vertices.size());
}

void ScramjetGeometry::generate_inlet() {
    int segments = config_.inlet_segments;
    int radial = config_.radial_segments;
    
    float x_start = -config_.inlet_length;
    float x_end = 0.0f;
    float height_start = config_.inlet_height * 1.2f;
    float height_end = config_.inlet_height;
    float width = config_.inlet_width;
    float thickness = config_.wall_thickness;
    
    for (int ring = 0; ring <= segments; ++ring) {
        float t = static_cast<float>(ring) / segments;
        float x = x_start + t * (x_end - x_start);
        float height = height_start + t * (height_end - height_start);
        
        float capture_ratio = 1.0f - 0.3f * t;
        float current_height = height * capture_ratio;
        
        for (int j = 0; j <= radial; ++j) {
            float angle = 2.0f * PI * j / radial;
            float y = current_height * std::cos(angle) * 0.5f;
            float z = width * 0.5f * std::sin(angle);
            
            add_vertex(external_mesh_, x, y + current_height * 0.5f, z,
                      0.0f, std::cos(angle), std::sin(angle),
                      t, static_cast<float>(j)/radial,
                      0.6f, 0.65f, 0.7f);
            
            add_vertex(external_mesh_, x, y - current_height * 0.5f + thickness, z,
                      0.0f, -std::cos(angle), -std::sin(angle),
                      t, static_cast<float>(j)/radial,
                      0.5f, 0.55f, 0.6f);
        }
    }
    
    for (int ring = 0; ring < segments; ++ring) {
        for (int j = 0; j < radial; ++j) {
            int base = ring * (radial + 1) * 2;
            int next = (ring + 1) * (radial + 1) * 2;
            
            add_triangle(external_mesh_, base + j*2, next + j*2, next + (j+1)*2);
            add_triangle(external_mesh_, base + j*2, next + (j+1)*2, base + (j+1)*2);
            
            add_triangle(external_mesh_, base + j*2 + 1, base + (j+1)*2 + 1, next + (j+1)*2 + 1);
            add_triangle(external_mesh_, base + j*2 + 1, next + (j+1)*2 + 1, next + j*2 + 1);
        }
    }
    
    inlet_mesh_ = external_mesh_;
}

void ScramjetGeometry::generate_combustor() {
    int segments = config_.combustor_segments;
    int radial = config_.radial_segments;
    
    float x_start = 0.0f;
    float x_end = config_.combustor_length;
    float height = config_.combustor_height;
    float width = config_.combustor_width;
    float thickness = config_.wall_thickness;
    
    for (int ring = 0; ring <= segments; ++ring) {
        float t = static_cast<float>(ring) / segments;
        float x = x_start + t * (x_end - x_start);
        
        float expansion = 1.0f + 0.1f * t;
        float current_height = height * expansion;
        
        for (int j = 0; j <= radial; ++j) {
            float angle = 2.0f * PI * j / radial;
            float y = current_height * std::cos(angle) * 0.5f;
            float z = width * 0.5f * std::sin(angle);
            
            float temp = 0.3f + 0.7f * t;
            add_vertex(external_mesh_, x, y + current_height * 0.5f, z,
                      0.0f, std::cos(angle), std::sin(angle),
                      t, static_cast<float>(j)/radial,
                      0.7f, 0.3f + 0.3f*t, 0.2f);
            
            add_vertex(external_mesh_, x, y - current_height * 0.5f + thickness, z,
                      0.0f, -std::cos(angle), -std::sin(angle),
                      t, static_cast<float>(j)/radial,
                      0.6f, 0.25f + 0.25f*t, 0.15f);
        }
    }
    
    for (int ring = 0; ring < segments; ++ring) {
        for (int j = 0; j < radial; ++j) {
            int base = ring * (radial + 1) * 2;
            int next = (ring + 1) * (radial + 1) * 2;
            
            add_triangle(external_mesh_, base + j*2, next + j*2, next + (j+1)*2);
            add_triangle(external_mesh_, base + j*2, next + (j+1)*2, base + (j+1)*2);
            
            add_triangle(external_mesh_, base + j*2 + 1, base + (j+1)*2 + 1, next + (j+1)*2 + 1);
            add_triangle(external_mesh_, base + j*2 + 1, next + (j+1)*2 + 1, next + j*2 + 1);
        }
    }
    
    combustor_mesh_ = external_mesh_;
}

void ScramjetGeometry::generate_nozzle() {
    int segments = config_.nozzle_segments;
    int radial = config_.radial_segments;
    
    float x_start = config_.combustor_length;
    float x_end = config_.combustor_length + config_.nozzle_length;
    float height_start = config_.combustor_height;
    float height_end = config_.nozzle_exit_height;
    float width_start = config_.combustor_width;
    float width_end = config_.nozzle_exit_width;
    float thickness = config_.wall_thickness;
    
    for (int ring = 0; ring <= segments; ++ring) {
        float t = static_cast<float>(ring) / segments;
        float x = x_start + t * (x_end - x_start);
        float height = height_start + t * (height_end - height_start);
        float width = width_start + t * (width_end - width_start);
        
        for (int j = 0; j <= radial; ++j) {
            float angle = 2.0f * PI * j / radial;
            float y = height * std::cos(angle) * 0.5f;
            float z = width * 0.5f * std::sin(angle);
            
            add_vertex(external_mesh_, x, y + height * 0.5f, z,
                      0.0f, std::cos(angle), std::sin(angle),
                      t, static_cast<float>(j)/radial,
                      0.65f, 0.65f, 0.7f);
            
            add_vertex(external_mesh_, x, y - height * 0.5f + thickness, z,
                      0.0f, -std::cos(angle), -std::sin(angle),
                      t, static_cast<float>(j)/radial,
                      0.55f, 0.55f, 0.6f);
        }
    }
    
    for (int ring = 0; ring < segments; ++ring) {
        for (int j = 0; j < radial; ++j) {
            int base = ring * (radial + 1) * 2;
            int next = (ring + 1) * (radial + 1) * 2;
            
            add_triangle(external_mesh_, base + j*2, next + j*2, next + (j+1)*2);
            add_triangle(external_mesh_, base + j*2, next + (j+1)*2, base + (j+1)*2);
            
            add_triangle(external_mesh_, base + j*2 + 1, base + (j+1)*2 + 1, next + (j+1)*2 + 1);
            add_triangle(external_mesh_, base + j*2 + 1, next + (j+1)*2 + 1, next + j*2 + 1);
        }
    }
    
    nozzle_mesh_ = external_mesh_;
}

void ScramjetGeometry::generate_struts() {
    int struts = static_cast<int>(config_.struts_count);
    int segments = 20;
    float length = config_.combustor_length;
    float strut_width = 0.05f;
    float strut_thickness = 0.02f;
    
    for (int s = 0; s < struts; ++s) {
        float angle = 2.0f * PI * s / struts;
        float radius = config_.combustor_height * 0.3f;
        
        for (int i = 0; i <= segments; ++i) {
            float t = static_cast<float>(i) / segments;
            float x = t * length;
            float y = radius * std::cos(angle);
            float z = radius * std::sin(angle);
            
            add_vertex(strut_mesh_, x, y, z,
                      0.0f, std::cos(angle), std::sin(angle),
                      t, 0.0f, 0.8f, 0.8f, 0.85f);
            
            add_vertex(strut_mesh_, x, y + strut_thickness, z,
                      0.0f, std::cos(angle), std::sin(angle),
                      t, 1.0f, 0.8f, 0.8f, 0.85f);
        }
    }
}

void ScramjetGeometry::generate_fuel_injectors() {
    int injectors = static_cast<int>(config_.fuel_injector_count);
    float x_pos = config_.combustor_length * 0.1f;
    float radius = config_.combustor_height * 0.4f;
    
    for (int i = 0; i < injectors; ++i) {
        float angle = 2.0f * PI * i / injectors;
        
        for (int j = 0; j <= 8; ++j) {
            float t = static_cast<float>(j) / 8;
            float x = x_pos + t * 0.3f;
            float y = radius * std::cos(angle) * (1.0f - 0.3f * t);
            float z = radius * std::sin(angle) * (1.0f - 0.3f * t);
            
            add_vertex(injector_mesh_, x, y, z,
                      1.0f, 0.0f, 0.0f,
                      t, 0.0f, 0.9f, 0.5f, 0.1f);
        }
    }
}

void ScramjetGeometry::generate_internal_ducts() {
    int radial = 16;
    float x_start = -config_.inlet_length;
    float x_end = config_.combustor_length + config_.nozzle_length;
    int total_segments = 100;
    
    for (int ring = 0; ring <= total_segments; ++ring) {
        float t = static_cast<float>(ring) / total_segments;
        float x = x_start + t * (x_end - x_start);
        
        float duct_radius;
        if (x < 0.0f) {
            duct_radius = config_.inlet_height * 0.4f * (1.0f - 0.2f * t);
        } else if (x < config_.combustor_length) {
            duct_radius = config_.combustor_height * 0.35f * (1.0f + 0.1f * t);
        } else {
            float t2 = (x - config_.combustor_length) / config_.nozzle_length;
            duct_radius = config_.combustor_height * 0.35f * (1.0f + 0.1f + 0.2f * t2);
        }
        
        for (int j = 0; j <= radial; ++j) {
            float angle = 2.0f * PI * j / radial;
            float y = duct_radius * std::cos(angle);
            float z = duct_radius * std::sin(angle);
            
            add_vertex(internal_mesh_, x, y, z,
                      -std::cos(angle), -std::sin(angle), 0.0f,
                      t, static_cast<float>(j)/radial,
                      0.3f + 0.4f*t, 0.15f + 0.2f*t, 0.1f + 0.1f*t);
        }
    }
    
    for (int ring = 0; ring < total_segments; ++ring) {
        for (int j = 0; j < radial; ++j) {
            int base = ring * (radial + 1);
            int next = (ring + 1) * (radial + 1);
            
            add_triangle(internal_mesh_, base + j, next + j, next + (j+1));
            add_triangle(internal_mesh_, base + j, next + (j+1), base + (j+1));
        }
    }
}

void ScramjetGeometry::compute_flow_path() {
    flow_path_.clear();
    int points = 100;
    
    for (int i = 0; i <= points; ++i) {
        float t = static_cast<float>(i) / points;
        float x, y, z;
        
        if (t < 0.3f) {
            x = -config_.inlet_length + t / 0.3f * config_.inlet_length;
            float capture = 1.0f - 0.3f * (t / 0.3f);
            y = 0.0f;
            z = 0.0f;
        } else if (t < 0.7f) {
            x = (t - 0.3f) / 0.4f * config_.combustor_length;
            y = 0.0f;
            z = 0.0f;
        } else {
            x = config_.combustor_length + (t - 0.7f) / 0.3f * config_.nozzle_length;
            y = 0.0f;
            z = 0.0f;
        }
        
        flow_path_.push_back({x, y, z});
    }
}

void ScramjetGeometry::compute_thermal_profiles() {
    temperature_profile_.clear();
    pressure_profile_.clear();
    velocity_profile_.clear();
    
    for (size_t i = 0; i < flow_path_.size(); ++i) {
        float t = static_cast<float>(i) / (flow_path_.size() - 1);
        
        float temp;
        if (t < 0.3f) {
            temp = 288.15f + 100.0f * t / 0.3f;
        } else if (t < 0.7f) {
            temp = 388.15f + 600.0f * (t - 0.3f) / 0.4f;
        } else {
            temp = 988.15f - 200.0f * (t - 0.7f) / 0.3f;
        }
        temperature_profile_.push_back(temp);
        
        float pressure;
        if (t < 0.3f) {
            pressure = 101325.0f * (1.0f - 0.2f * t / 0.3f);
        } else if (t < 0.7f) {
            pressure = 81060.0f * (1.0f + 0.3f * (t - 0.3f) / 0.4f);
        } else {
            pressure = 105378.0f * (1.0f + 0.1f * (t - 0.7f) / 0.3f);
        }
        pressure_profile_.push_back(pressure);
        
        float velocity;
        if (t < 0.3f) {
            velocity = 300.0f + 200.0f * t / 0.3f;
        } else if (t < 0.7f) {
            velocity = 500.0f - 100.0f * (t - 0.3f) / 0.4f;
        } else {
            velocity = 400.0f + 200.0f * (t - 0.7f) / 0.3f;
        }
        velocity_profile_.push_back(velocity);
    }
}

void ScramjetGeometry::update_flow_simulation(float mach, float altitude) {
    float stagnation_factor = 1.0f + 0.1f * mach;
    
    for (auto& temp : temperature_profile_) {
        temp *= stagnation_factor;
    }
    
    for (auto& vel : velocity_profile_) {
        vel *= (1.0f + 0.05f * mach);
    }
    
    needs_regeneration_ = true;
}

void ScramjetGeometry::compute_thermal_loads(float total_temperature) {
    float base_area = PI * config_.combustor_height * config_.combustor_length;
    float heat_flux = 0.001f * total_temperature * total_temperature;
    
    float thermal_load = heat_flux * base_area;
    
    LOG_DEBUG("Thermal load computed: {} W", thermal_load);
}

void ScramjetGeometry::compute_internal_geometry() {
    generate_internal_ducts();
}

void ScramjetGeometry::add_vertex(Mesh& mesh, float x, float y, float z,
                                   float nx, float ny, float nz,
                                   float u, float v,
                                   float r, float g, float b, float a) {
    Vertex vert;
    vert.position[0] = x; vert.position[1] = y; vert.position[2] = z;
    vert.normal[0] = nx; vert.normal[1] = ny; vert.normal[2] = nz;
    vert.texcoord[0] = u; vert.texcoord[1] = v;
    vert.color[0] = r; vert.color[1] = g; vert.color[2] = b; vert.color[3] = a;
    mesh.vertices.push_back(vert);
}

void ScramjetGeometry::add_quad(Mesh& mesh, int v0, int v1, int v2, int v3) {
    mesh.indices.push_back(v0);
    mesh.indices.push_back(v1);
    mesh.indices.push_back(v2);
    
    mesh.indices.push_back(v0);
    mesh.indices.push_back(v2);
    mesh.indices.push_back(v3);
}

void ScramjetGeometry::add_triangle(Mesh& mesh, int v0, int v1, int v2) {
    mesh.indices.push_back(v0);
    mesh.indices.push_back(v1);
    mesh.indices.push_back(v2);
}

}
