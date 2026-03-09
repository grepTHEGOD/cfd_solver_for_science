#pragma once

#include <vector>
#include <array>
#include <string>

#include <rendering/mesh.hh>

namespace cfd {

enum class ForceType {
    LIFT,
    DRAG,
    SIDE,
    THRUST
};

struct ForceArrowConfig {
    float arrow_scale = 1.0f;
    float arrow_head_size = 0.2f;
    float line_thickness = 3.0f;
    
    bool show_labels = true;
    bool show_magnitude = true;
    
    float lift_color[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    float drag_color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    float side_color[4] = {0.0f, 0.0f, 1.0f, 1.0f};
    float thrust_color[4] = {1.0f, 0.5f, 0.0f, 1.0f};
};

class ForceArrows {
public:
    ForceArrows();
    ~ForceArrows();
    
    void generate();
    void update(const AerodynamicForces& forces, const ScramjetState& scramjet_state);
    
    void render();
    
    void set_config(const ForceArrowConfig& config) { config_ = config; }
    const ForceArrowConfig& get_config() const { return config_; }
    
    void set_visible(bool visible) { visible_ = visible; }
    bool is_visible() const { return visible_; }

private:
    void generate_arrow_mesh(Mesh& mesh, float length, float head_size);
    
    ForceArrowConfig config_;
    
    Mesh lift_arrow_;
    Mesh drag_arrow_;
    Mesh side_arrow_;
    Mesh thrust_arrow_;
    
    float current_lift_ = 0.0f;
    float current_drag_ = 0.0f;
    float current_side_ = 0.0f;
    float current_thrust_ = 0.0f;
    
    bool visible_ = true;
};

}
