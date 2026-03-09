#pragma once

#include <array>
#include <vector>
#include <string>

namespace cfd {

class Camera {
public:
    Camera();
    
    void set_position(float x, float y, float z);
    void set_target(float x, float y, float z);
    void set_up(float x, float y, float z);
    void set_fov(float fov);
    void set_aspect(float aspect);
    void set_near(float near);
    void set_far(float far);
    
    void orbit(float delta_theta, float delta_phi);
    void pan(float delta_x, float delta_y);
    void zoom(float delta);
    
    void reset();
    
    const float* position() const { return position_; }
    const float* target() const { return target_; }
    const float* up() const { return up_; }
    float fov() const { return fov_; }
    float aspect() const { return aspect_; }
    float near() const { return near_; }
    float far() const { return far_; }
    
    std::array<float, 16> view_matrix() const;
    std::array<float, 16> projection_matrix() const;
    std::array<float, 3> forward() const;
    std::array<float, 3> right() const;
    std::array<float, 3> up_dir() const;
    
private:
    void update_vectors();
    
    float position_[3] = {0.0f, 5.0f, 15.0f};
    float target_[3] = {0.0f, 0.0f, 0.0f};
    float up_[3] = {0.0f, 1.0f, 0.0f};
    
    float theta_ = 0.0f;
    float phi_ = 0.3f;
    float distance_ = 15.0f;
    
    float fov_ = 45.0f;
    float aspect_ = 16.0f / 9.0f;
    float near_ = 0.1f;
    float far_ = 1000.0f;
};

}
