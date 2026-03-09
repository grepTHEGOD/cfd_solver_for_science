#pragma once

#include <memory>
#include <string>
#include <functional>

#include <GLFW/glfw3.h>

#include <app/scene.hh>

namespace cfd {

class Renderer;

class UIManager {
public:
    UIManager(GLFWwindow* window);
    ~UIManager();
    
    void initialize();
    void shutdown();
    
    void begin_frame();
    void end_frame();
    
    void render_control_panel(SimulationParameters& params);
    void render_results_panel(const AtmosphericConditions& atm,
                             const AerodynamicForces& forces,
                             const HeatTransferResults& heat,
                             const DerivedValues& derived,
                             const ScramjetState& scramjet);
    void render_toolbar(bool& show_grid, bool& show_axis, bool& show_wireframe,
                       bool& show_streamlines, bool& show_pressure_contours,
                       bool& show_thermal_map, bool& show_force_vectors,
                       bool& show_internal_flow);
    void render_object_browser();
    void render_properties_panel();
    void render_status_bar();
    
    void set_renderer(Renderer* renderer) { renderer_ = renderer; }
    void set_params(SimulationParameters* params) { params_ = params; }
    
    bool is_hovered() const { return hovered_; }

private:
    void setup_style();
    void render_slider_float(const char* label, float& value, float min_val, float max_val);
    void render_slider_int(const char* label, int& value, int min_val, int max_val);
    void render_value_display(const char* label, float value, const char* unit);
    void render_value_display(const char* label, int value, const char* unit);
    
    GLFWwindow* window_ = nullptr;
    Renderer* renderer_ = nullptr;
    SimulationParameters* params_ = nullptr;
    
    bool hovered_ = false;
    float control_panel_height_ = 200.0f;
    float results_panel_width_ = 280.0f;
    float toolbar_height_ = 40.0f;
    float status_bar_height_ = 24.0f;
};

}
