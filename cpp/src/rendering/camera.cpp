#include <rendering/camera.hh>
#include <utils/math.hh>
#include <cmath>

namespace cfd {

Camera::Camera() {
    update_vectors();
}

void Camera::set_position(float x, float y, float z) {
    position_[0] = x; position_[1] = y; position_[2] = z;
    update_vectors();
}

void Camera::set_target(float x, float y, float z) {
    target_[0] = x; target_[1] = y; target_[2] = z;
    update_vectors();
}

void Camera::set_up(float x, float y, float z) {
    up_[0] = x; up_[1] = y; up_[2] = z;
    update_vectors();
}

void Camera::set_fov(float fov) { fov_ = fov; }
void Camera::set_aspect(float aspect) { aspect_ = aspect; }
void Camera::set_near(float near) { near_ = near; }
void Camera::set_far(float far) { far_ = far; }

void Camera::orbit(float delta_theta, float delta_phi) {
    theta_ += delta_theta;
    phi_ += delta_phi;
    phi_ = std::clamp(phi_, -PI_2 + 0.01f, PI_2 - 0.01f);
    update_vectors();
}

void Camera::pan(float delta_x, float delta_y) {
    auto right_vec = right();
    auto up_dir_vec = up_dir();
    
    target_[0] += right_vec[0] * delta_x + up_dir_vec[0] * delta_y;
    target_[1] += right_vec[1] * delta_x + up_dir_vec[1] * delta_y;
    target_[2] += right_vec[2] * delta_x + up_dir_vec[2] * delta_y;
    
    update_vectors();
}

void Camera::zoom(float delta) {
    distance_ = std::clamp(distance_ + delta, 1.0f, 100.0f);
    update_vectors();
}

void Camera::reset() {
    theta_ = 0.0f;
    phi_ = 0.3f;
    distance_ = 15.0f;
    target_[0] = target_[1] = target_[2] = 0.0f;
    update_vectors();
}

void Camera::update_vectors() {
    position_[0] = target_[0] + distance_ * std::cos(phi_) * std::sin(theta_);
    position_[1] = target_[1] + distance_ * std::sin(phi_);
    position_[2] = target_[2] + distance_ * std::cos(phi_) * std::cos(theta_);
}

std::array<float, 16> Camera::view_matrix() const {
    auto f = forward();
    auto r = right();
    auto u = up_dir();
    
    return std::array<float, 16>{
        r[0], u[0], -f[0], 0.0f,
        r[1], u[1], -f[1], 0.0f,
        r[2], u[2], -f[2], 0.0f,
        -(r[0]*position_[0] + r[1]*position_[1] + r[2]*position_[2]),
        -(u[0]*position_[0] + u[1]*position_[1] + u[2]*position_[2]),
        (f[0]*position_[0] + f[1]*position_[1] + f[2]*position_[2]),
        1.0f
    };
}

std::array<float, 16> Camera::projection_matrix() const {
    float tan_half_fov = std::tan(fov_ * 0.5f * DEG_TO_RAD);
    float z_range = far_ - near_;
    
    return std::array<float, 16>{
        1.0f / (aspect_ * tan_half_fov), 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f / tan_half_fov, 0.0f, 0.0f,
        0.0f, 0.0f, -(far_ + near_) / z_range, -1.0f,
        0.0f, 0.0f, -(2.0f * far_ * near_) / z_range, 0.0f
    };
}

std::array<float, 3> Camera::forward() const {
    float fx = position_[0] - target_[0];
    float fy = position_[1] - target_[1];
    float fz = position_[2] - target_[2];
    float len = std::sqrt(fx*fx + fy*fy + fz*fz);
    return {fx/len, fy/len, fz/len};
}

std::array<float, 3> Camera::right() const {
    auto f = forward();
    auto u = up_dir();
    
    float rx = u[1] * f[2] - u[2] * f[1];
    float ry = u[2] * f[0] - u[0] * f[2];
    float rz = u[0] * f[1] - u[1] * f[0];
    float len = std::sqrt(rx*rx + ry*ry + rz*rz);
    return {rx/len, ry/len, rz/len};
}

std::array<float, 3> Camera::up_dir() const {
    auto r = right();
    auto f = forward();
    
    return std::array<float, 3>{
        f[1]*r[2] - f[2]*r[1],
        f[2]*r[0] - f[0]*r[2],
        f[0]*r[1] - f[1]*r[0]
    };
}

}
