#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <optional>
#include <variant>
#include <array>

#include <GLFW/glfw3.h>

#include <app/scene.hh>
#include <rendering/renderer.hh>
#include <rendering/camera.hh>
#include <geometry/scramjet_geometry.hh>
#include <visualization/streamlines.hh>
#include <visualization/contours.hh>
#include <visualization/force_arrows.hh>
#include <ui/ui_manager.hh>
#include <physics/atmosphere.hh>
#include <physics/forces.hh>
#include <physics/heat_transfer.hh>
#include <utils/logger.hh>

namespace cfd {

class Application {
public:
    Application();
    ~Application();

    void initialize(int argc, char** argv);
    void run();
    void shutdown();

private:
    void setup_glfw();
    void setup_renderer();
    void setup_ui();
    void setup_scramjet();
    void update_physics(float dt);
    void render();
    void process_input();
    
    static void glfw_error_callback(int error, const char* description);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

    GLFWwindow* window_ = nullptr;
    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<Camera> camera_;
    std::unique_ptr<UIManager> ui_manager_;
    std::unique_ptr<ScramjetGeometry> scramjet_;
    std::unique_ptr<Streamlines> streamlines_;
    std::unique_ptr<Contours> contours_;
    std::unique_ptr<ForceArrows> force_arrows_;
    
    Atmosphere atmosphere_;
    Forces forces_;
    HeatTransfer heat_transfer_;
    
    SimulationParameters params_;
    AtmosphericConditions atmosphere_conditions_;
    AerodynamicForces forces_result_;
    HeatTransferResults heat_result_;
    DerivedValues derived_;
    ScramjetState scramjet_state_;
    
    bool running_ = true;
    bool show_wireframe_ = false;
    bool show_grid_ = true;
    bool show_axis_ = true;
    bool show_streamlines_ = true;
    bool show_pressure_contours_ = true;
    bool show_thermal_map_ = true;
    bool show_force_vectors_ = true;
    bool show_internal_flow_ = true;
    
    int viewport_width_ = 1600;
    int viewport_height_ = 900;
    
    double last_mouse_x_ = 0.0;
    double last_mouse_y_ = 0.0;
    bool mouse_dragging_ = false;
    int mouse_drag_button_ = -1;
    
    float camera_distance_ = 15.0f;
    float camera_theta_ = 0.5f;
    float camera_phi_ = 0.3f;
    float camera_target_[3] = {0.0f, 0.0f, 0.0f};
    
    std::chrono::high_resolution_clock::time_point last_time_;
    float delta_time_ = 0.0f;
    float accumulator_ = 0.0f;
    const float fixed_timestep_ = 1.0f / 60.0f;
};

}
