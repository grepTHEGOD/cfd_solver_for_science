#pragma once

#include <vector>
#include <array>
#include <string>

#include <rendering/mesh.hh>

namespace cfd {

enum class ContourType {
    PRESSURE,
    TEMPERATURE,
    DENSITY,
    VELOCITY,
    MACH
};

struct ContourConfig {
    int num_contours = 20;
    float min_value = 0.0f;
    float max_value = 1.0f;
    bool show_labels = true;
    bool smooth_colors = true;
    
    ContourType type = ContourType::PRESSURE;
};

class Contours {
public:
    Contours();
    ~Contours();
    
    void generate(const ScramjetGeometry& scramjet, float mach, float altitude);
    void update(float mach, float altitude);
    
    void render();
    
    void set_config(const ContourConfig& config) { config_ = config; }
    const ContourConfig& get_config() const { return config_; }
    
    void set_visible(bool visible) { visible_ = visible; }
    bool is_visible() const { return visible_; }
    
    void set_contour_type(ContourType type) { config_.type = type; }
    ContourType get_contour_type() const { return config_.type; }

private:
    std::array<float, 4> get_color_for_value(float value, float min_val, float max_val);
    void generate_surface_contours();
    
    ContourConfig config_;
    std::vector<Mesh> contour_meshes_;
    
    bool visible_ = true;
    bool needs_regeneration_ = true;
};

}
