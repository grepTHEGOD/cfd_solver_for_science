#include <app/application.hh>
#include <utils/logger.hh>
#include <rendering/gl_functions.hh>

#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>

namespace cfd {

Application::Application() {
    LOG_INFO("CFD Studio C++ Application created");
}

Application::~Application() {
    LOG_INFO("CFD Studio C++ Application destroyed");
}

void Application::initialize(int argc, char** argv) {
    LOG_INFO("Initializing CFD Studio C++");
    
    setup_glfw();
    
    renderer_ = std::make_unique<Renderer>();
    if (!renderer_->initialize(viewport_width_, viewport_height_)) {
        throw std::runtime_error("Failed to initialize renderer");
    }
    
    camera_ = std::make_unique<Camera>();
    renderer_->set_camera(camera_.get());
    
    setup_scramjet();
    
    ui_manager_ = std::make_unique<UIManager>(window_);
    ui_manager_->initialize();
    ui_manager_->set_renderer(renderer_.get());
    ui_manager_->set_params(&params_);
    
    streamlines_ = std::make_unique<Streamlines>();
    contours_ = std::make_unique<Contours>();
    force_arrows_ = std::make_unique<ForceArrows>();
    
    LOG_INFO("CFD Studio C++ initialized successfully");
}

void Application::setup_glfw() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    
    glfwSetErrorCallback(glfw_error_callback);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    window_ = glfwCreateWindow(viewport_width_, viewport_height_, "CFD Studio C++ - Scramjet Simulation", 
                              nullptr, nullptr);
    if (!window_) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }
    
    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);
    
    glfwSetWindowUserPointer(window_, this);
    
    glfwSetFramebufferSizeCallback(window_, [](GLFWwindow* w, int width, int height) {
        glViewport(0, 0, width, height);
        auto* app = static_cast<Application*>(glfwGetWindowUserPointer(w));
        if (app) {
            app->viewport_width_ = width;
            app->viewport_height_ = height;
        }
    });
    
    glfwSetKeyCallback(window_, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        auto* app = static_cast<Application*>(glfwGetWindowUserPointer(w));
        if (app && action == GLFW_PRESS) {
            if (key == GLFW_KEY_ESCAPE) {
                glfwSetWindowShouldClose(w, GLFW_TRUE);
            }
        }
    });
    
    glfwSetMouseButtonCallback(window_, [](GLFWwindow* w, int button, int action, int mods) {
        auto* app = static_cast<Application*>(glfwGetWindowUserPointer(w));
        if (app) {
            if (action == GLFW_PRESS) {
                app->mouse_dragging_ = true;
                app->mouse_drag_button_ = button;
            } else {
                app->mouse_dragging_ = false;
                app->mouse_drag_button_ = -1;
            }
        }
    });
    
    glfwSetScrollCallback(window_, [](GLFWwindow* w, double xoffset, double yoffset) {
        auto* app = static_cast<Application*>(glfwGetWindowUserPointer(w));
        if (app && app->camera_) {
            app->camera_->zoom(static_cast<float>(yoffset) * 0.5f);
        }
    });
    
    glfwSetCursorPosCallback(window_, [](GLFWwindow* w, double xpos, double ypos) {
        auto* app = static_cast<Application*>(glfwGetWindowUserPointer(w));
        if (app && app->mouse_dragging_ && app->camera_) {
            float delta_x = static_cast<float>(xpos - app->last_mouse_x_);
            float delta_y = static_cast<float>(ypos - app->last_mouse_y_);
            
            if (app->mouse_drag_button_ == GLFW_MOUSE_BUTTON_LEFT) {
                app->camera_->orbit(delta_x * 0.01f, delta_y * 0.01f);
            } else if (app->mouse_drag_button_ == GLFW_MOUSE_BUTTON_MIDDLE) {
                app->camera_->pan(delta_x * 0.01f, -delta_y * 0.01f);
            }
            
            app->last_mouse_x_ = xpos;
            app->last_mouse_y_ = ypos;
        }
    });
    
    LOG_INFO("GLFW initialized");
}

void Application::setup_scramjet() {
    scramjet_ = std::make_unique<ScramjetGeometry>();
    scramjet_->generate();
    LOG_INFO("Scramjet geometry generated");
}

void Application::run() {
    LOG_INFO("Starting main loop");
    last_time_ = std::chrono::high_resolution_clock::now();
    
    while (!glfwWindowShouldClose(window_) && running_) {
        auto current_time = std::chrono::high_resolution_clock::now();
        delta_time_ = std::chrono::duration<float>(current_time - last_time_).count();
        last_time_ = current_time;
        
        accumulator_ += delta_time_;
        
        process_input();
        
        while (accumulator_ >= fixed_timestep_) {
            update_physics(fixed_timestep_);
            accumulator_ -= fixed_timestep_;
        }
        
        render();
        
        glfwPollEvents();
    }
    
    LOG_INFO("Main loop ended");
}

void Application::process_input() {
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        running_ = false;
    }
    
    float camera_speed = 0.1f;
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
        camera_->pan(0.0f, -camera_speed);
    }
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
        camera_->pan(0.0f, camera_speed);
    }
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
        camera_->pan(-camera_speed, 0.0f);
    }
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
        camera_->pan(camera_speed, 0.0f);
    }
}

void Application::update_physics(float dt) {
    float altitude_m = params_.altitude_in_meters ? params_.altitude : Atmosphere::feet_to_meters(params_.altitude);
    float velocity_ms = params_.speed_in_mach ? 
        (params_.airspeed * atmosphere_.speed_of_sound(SEA_LEVEL_TEMP)) : 
        Atmosphere::mph_to_mps(params_.airspeed);
    
    atmosphere_conditions_ = atmosphere_.compute(altitude_m);
    
    if (params_.speed_in_mach) {
        velocity_ms = params_.airspeed * atmosphere_conditions_.speed_of_sound;
    }
    
    derived_.mach_number = atmosphere_.mach_from_velocity(velocity_ms, atmosphere_conditions_.speed_of_sound);
    derived_.reynolds_number = atmosphere_.reynolds_number(
        velocity_ms, 
        params_.reference_length,
        atmosphere_conditions_.density,
        atmosphere_conditions_.viscosity
    );
    derived_.dynamic_pressure = atmosphere_.dynamic_pressure(atmosphere_conditions_.density, velocity_ms);
    derived_.air_density = atmosphere_conditions_.density;
    derived_.speed_of_sound = atmosphere_conditions_.speed_of_sound;
    
    float aoa_rad = params_.angle_of_attack * DEG_TO_RAD;
    forces_result_ = forces_.compute(
        velocity_ms,
        atmosphere_conditions_.density,
        aoa_rad,
        params_.reference_area,
        params_.reference_length,
        derived_.mach_number,
        derived_.reynolds_number
    );
    
    heat_result_ = heat_transfer_.compute(
        velocity_ms,
        atmosphere_conditions_.density,
        atmosphere_conditions_.temperature,
        params_.reference_length,
        params_.reference_area
    );
    
    if (derived_.mach_number > 3.0f) {
        float stagnation_temp = heat_transfer_.stagnation_temperature(
            atmosphere_conditions_.temperature,
            velocity_ms
        );
        
        scramjet_state_.combustion_temperature = stagnation_temp * 3.0f;
        scramjet_state_.thrust = forces_result_.drag * 2.0f;
        scramjet_state_.inlet_capture_ratio = std::min(1.0f, derived_.mach_number / 8.0f);
        
        float fuel_energy = 43.0e6f;
        scramjet_state_.specific_impulse = scramjet_state_.thrust / (params_.fuel * GRAVITY) * 1000.0f;
        
        scramjet_state_.thermal_efficiency = 0.3f + 0.1f * derived_.mach_number;
        scramjet_state_.propulsive_efficiency = 2.0f / (derived_.mach_number + 1.0f / derived_.mach_number);
        scramjet_state_.overall_efficiency = scramjet_state_.thermal_efficiency * scramjet_state_.propulsive_efficiency;
    }
    
    if (scramjet_) {
        scramjet_->update_flow_simulation(derived_.mach_number, altitude_m);
    }
}

void Application::render() {
    renderer_->begin_frame();
    renderer_->clear();
    
    if (show_grid_) {
        renderer_->render_grid(20.0f, 20);
    }
    
    if (show_axis_) {
        renderer_->render_axis(3.0f);
    }
    
    if (scramjet_) {
        RenderMode mode = show_wireframe_ ? RenderMode::WIREFRAME : RenderMode::SOLID;
        renderer_->render_scramjet(*scramjet_, mode);
    }
    
    if (show_streamlines_ && streamlines_ && scramjet_) {
        streamlines_->generate(*scramjet_, derived_.mach_number, params_.altitude);
        streamlines_->render();
    }
    
    if (show_pressure_contours_ && contours_ && scramjet_) {
        contours_->generate(*scramjet_, derived_.mach_number, params_.altitude);
        contours_->render();
    }
    
    if (show_force_vectors_ && force_arrows_) {
        force_arrows_->update(forces_result_, scramjet_state_);
        force_arrows_->render();
    }
    
    renderer_->end_frame();
    
    ui_manager_->begin_frame();
    ui_manager_->render_control_panel(params_);
    ui_manager_->render_results_panel(atmosphere_conditions_, forces_result_, heat_result_, derived_, scramjet_state_);
    ui_manager_->render_toolbar(show_grid_, show_axis_, show_wireframe_, show_streamlines_, 
                                show_pressure_contours_, show_thermal_map_, show_force_vectors_,
                                show_internal_flow_);
    ui_manager_->render_status_bar();
    ui_manager_->end_frame();
    
    glfwSwapBuffers(window_);
}

void Application::shutdown() {
    LOG_INFO("Shutting down CFD Studio C++");
    
    ui_manager_.reset();
    force_arrows_.reset();
    contours_.reset();
    streamlines_.reset();
    scramjet_.reset();
    camera_.reset();
    renderer_.reset();
    
    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
    
    glfwTerminate();
    LOG_INFO("CFD Studio C++ shutdown complete");
}

void Application::glfw_error_callback(int error, const char* description) {
    LOG_ERROR("GLFW Error {}: {}", error, description);
}

}
