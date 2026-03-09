#pragma once

#include <vector>
#include <array>
#include <string>
#include <cstdint>

#include <rendering/mesh.hh>

namespace cfd {

struct ScramjetConfig {
    float inlet_length = 3.0f;
    float inlet_height = 1.5f;
    float inlet_width = 1.0f;
    
    float combustor_length = 4.0f;
    float combustor_height = 0.8f;
    float combustor_width = 0.9f;
    
    float nozzle_length = 3.0f;
    float nozzle_exit_height = 1.2f;
    float nozzle_exit_width = 1.1f;
    
    float wall_thickness = 0.05f;
    
    int inlet_segments = 32;
    int combustor_segments = 48;
    int nozzle_segments = 32;
    int radial_segments = 24;
    
    float fuel_injector_count = 8;
    float struts_count = 4;
};

class ScramjetGeometry {
public:
    ScramjetGeometry();
    ~ScramjetGeometry();
    
    void generate(const ScramjetConfig& config = ScramjetConfig());
    void regenerate();
    
    const Mesh& get_external_mesh() const { return external_mesh_; }
    const Mesh& get_internal_mesh() const { return internal_mesh_; }
    const Mesh& get_inlet_mesh() const { return inlet_mesh_; }
    const Mesh& get_combustor_mesh() const { return combustor_mesh_; }
    const Mesh& get_nozzle_mesh() const { return nozzle_mesh_; }
    const Mesh& get_strut_mesh() const { return strut_mesh_; }
    const Mesh& get_fuel_injector_mesh() const { return injector_mesh_; }
    
    const std::vector<std::array<float, 3>>& get_flow_path() const { return flow_path_; }
    const std::vector<float>& get_temperature_profile() const { return temperature_profile_; }
    const std::vector<float>& get_pressure_profile() const { return pressure_profile_; }
    
    void update_flow_simulation(float mach, float altitude);
    void compute_thermal_loads(float total_temperature);
    void compute_internal_geometry();
    
    void set_config(const ScramjetConfig& config) { config_ = config; }
    const ScramjetConfig& get_config() const { return config_; }
    
    float total_length() const { 
        return config_.inlet_length + config_.combustor_length + config_.nozzle_length; 
    }
    float max_width() const { 
        float w1 = config_.inlet_width > config_.combustor_width ? config_.inlet_width : config_.combustor_width;
        return w1 > config_.nozzle_exit_width ? w1 : config_.nozzle_exit_width; 
    }
    float max_height() const { 
        float h1 = config_.inlet_height > config_.combustor_height ? config_.inlet_height : config_.combustor_height;
        return h1 > config_.nozzle_exit_height ? h1 : config_.nozzle_exit_height; 
    }
    
    void set_show_internal(bool show) { show_internal_ = show; }
    bool show_internal() const { return show_internal_; }

private:
    void generate_inlet();
    void generate_combustor();
    void generate_nozzle();
    void generate_struts();
    void generate_fuel_injectors();
    void generate_internal_ducts();
    void compute_flow_path();
    void compute_thermal_profiles();
    
    void add_vertex(Mesh& mesh, float x, float y, float z, 
                   float nx, float ny, float nz,
                   float u, float v,
                   float r, float g, float b, float a = 1.0f);
    void add_quad(Mesh& mesh, int v0, int v1, int v2, int v3);
    void add_triangle(Mesh& mesh, int v0, int v1, int v2);
    
    ScramjetConfig config_;
    
    Mesh external_mesh_;
    Mesh internal_mesh_;
    Mesh inlet_mesh_;
    Mesh combustor_mesh_;
    Mesh nozzle_mesh_;
    Mesh strut_mesh_;
    Mesh injector_mesh_;
    
    std::vector<std::array<float, 3>> flow_path_;
    std::vector<float> temperature_profile_;
    std::vector<float> pressure_profile_;
    std::vector<float> velocity_profile_;
    
    bool show_internal_ = true;
    bool needs_regeneration_ = true;
};

}
