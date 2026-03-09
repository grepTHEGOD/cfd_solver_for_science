#pragma once

#include <vector>
#include <array>
#include <string>

#include <rendering/mesh.hh>

namespace cfd {

struct StreamlineConfig {
    int num_streamlines = 50;
    int points_per_streamline = 100;
    float step_size = 0.1f;
    float max_length = 20.0f;
    
    bool show_velocity = true;
    bool show_temperature = false;
    bool show_pressure = false;
    
    float velocity_scale = 1.0f;
    float line_thickness = 2.0f;
    
    float inlet_y_start = -0.5f;
    float inlet_y_end = 0.5f;
    float inlet_z_start = -0.4f;
    float inlet_z_end = 0.4f;
};

class Streamlines {
public:
    Streamlines();
    ~Streamlines();
    
    void generate(const ScramjetGeometry& scramjet, float mach, float altitude);
    void update(float mach, float altitude);
    
    void render();
    
    void set_config(const StreamlineConfig& config) { config_ = config; }
    const StreamlineConfig& get_config() const { return config_; }
    
    void set_visible(bool visible) { visible_ = visible; }
    bool is_visible() const { return visible_; }
    
    const std::vector<Mesh>& get_streamline_meshes() const { return streamline_meshes_; }
    const std::vector<float>& get_velocity_magnitudes() const { return velocity_magnitudes_; }

private:
    std::array<float, 3> compute_velocity(float x, float y, float z, float mach);
    float compute_temperature(float x, float y, float z);
    float compute_pressure(float x, float y, float z);
    
    StreamlineConfig config_;
    std::vector<Mesh> streamline_meshes_;
    std::vector<float> velocity_magnitudes_;
    std::vector<std::array<float, 3>> colors_;
    
    bool visible_ = true;
    bool needs_regeneration_ = true;
    
    float current_mach_ = 0.0f;
    float current_altitude_ = 0.0f;
};

}
