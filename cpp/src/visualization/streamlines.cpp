#include <visualization/streamlines.hh>
#include <geometry/scramjet_geometry.hh>
#include <utils/math.hh>

namespace cfd {

Streamlines::Streamlines() {
}

Streamlines::~Streamlines() {
}

void Streamlines::generate(const ScramjetGeometry& scramjet, float mach, float altitude) {
    current_mach_ = mach;
    current_altitude_ = altitude;
    
    streamline_meshes_.clear();
    velocity_magnitudes_.clear();
    colors_.clear();
    
    int num_streamlines = config_.num_streamlines;
    int points = config_.points_per_streamline;
    
    for (int s = 0; s < num_streamlines; ++s) {
        float y_start = config_.inlet_y_start + 
            (static_cast<float>(s) / num_streamlines) * (config_.inlet_y_end - config_.inlet_y_start);
        float z_start = config_.inlet_z_start + 
            (static_cast<float>(s % 5) / 5.0f) * (config_.inlet_z_end - config_.inlet_z_start);
        
        Mesh streamline;
        
        for (int i = 0; i <= points; ++i) {
            float t = static_cast<float>(i) / points;
            float x = -3.0f + t * 10.0f;
            
            float y = y_start;
            float z = z_start;
            
            auto vel = compute_velocity(x, y, z, mach);
            float vel_mag = std::sqrt(vel[0]*vel[0] + vel[1]*vel[1] + vel[2]*vel[2]);
            velocity_magnitudes_.push_back(vel_mag);
            
            float color[4];
            if (config_.show_velocity) {
                float normalized_vel = std::min(vel_mag / 1000.0f, 1.0f);
                color[0] = normalized_vel;
                color[1] = 1.0f - normalized_vel;
                color[2] = 0.0f;
                color[3] = 1.0f;
            } else {
                color[0] = 0.0f; color[1] = 0.5f; color[2] = 1.0f; color[3] = 1.0f;
            }
            
            Vertex v;
            v.position[0] = x; v.position[1] = y; v.position[2] = z;
            v.normal[0] = 0.0f; v.normal[1] = 1.0f; v.normal[2] = 0.0f;
            v.texcoord[0] = t; v.texcoord[1] = 0.0f;
            v.color[0] = color[0]; v.color[1] = color[1]; v.color[2] = color[2]; v.color[3] = color[3];
            streamline.vertices.push_back(v);
        }
        
        for (int i = 0; i < points; ++i) {
            streamline.indices.push_back(i);
            streamline.indices.push_back(i + 1);
        }
        
        streamline_meshes_.push_back(std::move(streamline));
    }
    
    needs_regeneration_ = false;
}

void Streamlines::update(float mach, float altitude) {
    if (std::abs(current_mach_ - mach) > 0.01f || 
        std::abs(current_altitude_ - altitude) > 10.0f) {
        needs_regeneration_ = true;
    }
}

void Streamlines::render() {
    if (!visible_) return;
}

std::array<float, 3> Streamlines::compute_velocity(float x, float y, float z, float mach) {
    float base_velocity = 300.0f + 100.0f * mach;
    
    if (x < 0.0f) {
        float compression = 1.0f - x * 0.1f;
        return {base_velocity * compression, 0.0f, 0.0f};
    } else if (x < 4.0f) {
        float combustion = 1.0f + 0.3f * (x / 4.0f);
        return {base_velocity * combustion, 0.0f, 0.0f};
    } else {
        float expansion = 1.2f + 0.2f * ((x - 4.0f) / 6.0f);
        return {base_velocity * expansion, 0.0f, 0.0f};
    }
}

float Streamlines::compute_temperature(float x, float y, float z) {
    if (x < 0.0f) return 288.15f;
    if (x < 4.0f) return 388.15f + 600.0f * (x / 4.0f);
    return 988.15f - 200.0f * ((x - 4.0f) / 6.0f);
}

float Streamlines::compute_pressure(float x, float y, float z) {
    if (x < 0.0f) return 101325.0f * (1.0f - 0.1f * (-x) / 3.0f);
    if (x < 4.0f) return 91000.0f * (1.0f + 0.2f * (x / 4.0f));
    return 110000.0f * (1.0f + 0.05f * ((x - 4.0f) / 6.0f));
}

}
