#pragma once

#include <memory>
#include <vector>
#include <string>

#include <GLFW/glfw3.h>

#include <rendering/shader.hh>
#include <rendering/mesh.hh>
#include <rendering/camera.hh>
#include <geometry/scramjet_geometry.hh>

namespace cfd {

enum class RenderMode {
    SOLID,
    WIREFRAME,
    SOLID_WIREFRAME
};

struct RenderSettings {
    bool show_grid = true;
    bool show_axis = true;
    bool show_wireframe = false;
    bool enable_lighting = true;
    bool enable_cull_face = true;
    bool enable_depth_test = true;
    
    float background_color[4] = {0.1f, 0.1f, 0.12f, 1.0f};
    float ambient_color[4] = {0.2f, 0.2f, 0.2f, 1.0f};
    float light_direction[3] = {1.0f, 1.0f, 1.0f};
    float light_color[4] = {1.0f, 0.95f, 0.9f, 1.0f};
    
    float mesh_color[4] = {0.7f, 0.7f, 0.75f, 1.0f};
    float wireframe_color[4] = {0.0f, 0.5f, 1.0f, 1.0f};
    float grid_color[4] = {0.3f, 0.3f, 0.35f, 1.0f};
};

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    bool initialize(int width, int height);
    void shutdown();
    
    void begin_frame();
    void end_frame();
    
    void set_viewport(int x, int y, int width, int height);
    void clear();
    
    void render_mesh(const Mesh& mesh, const float* model_matrix, 
                     const float* color = nullptr);
    void render_grid(float size, int divisions);
    void render_axis(float size);
    void render_scramjet(ScramjetGeometry& scramjet, RenderMode mode);
    void render_streamlines();
    void render_contours();
    void render_force_arrows();
    
    void set_camera(Camera* camera) { camera_ = camera; }
    Camera* camera() const { return camera_; }
    
    void set_render_settings(const RenderSettings& settings) { settings_ = settings; }
    const RenderSettings& render_settings() const { return settings_; }
    
    Shader* get_shader(const std::string& name);
    void load_shaders();

private:
    void setup_gl();
    void create_grid_mesh();
    void create_axis_mesh();
    
    GLFWwindow* window_ = nullptr;
    Camera* camera_ = nullptr;
    RenderSettings settings_;
    
    Mesh grid_mesh_;
    Mesh axis_mesh_;
    
    std::unique_ptr<Shader> basic_shader_;
    std::unique_ptr<Shader> grid_shader_;
    std::unique_ptr<Shader> contour_shader_;
    std::unique_ptr<Shader> streamline_shader_;
    
    int width_ = 1600;
    int height_ = 900;
};

}
