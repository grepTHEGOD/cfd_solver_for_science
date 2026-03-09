#pragma once

#include <cmath>
#include <algorithm>

#include <app/scene.hh>
#include <utils/math.hh>

namespace cfd {

class Forces {
public:
    Forces() = default;
    
    AerodynamicForces compute(float velocity, float density, float aoa,
                              float reference_area, float reference_length,
                              float mach, float reynolds);
    
    float lift_coefficient(float aoa, float mach, float reynolds) const;
    float drag_coefficient(float aoa, float mach, float reynolds) const;
    float side_coefficient(float beta) const;
    
    float form_drag(float cd0, float cl, float k, float e, float ar) const;
    float induced_drag(float cl, float e, float ar) const;
    float wave_drag(float mach, float aoa) const;
    float parasite_drag(float cf, float re_ratio, float swet, float area) const;
    
    static float degrees_to_radians(float degrees);
    static float radians_to_degrees(float radians);
    
private:
    float cl_alpha_ = 2.0f * PI;
    float cd0_ = 0.01f;
    float k_ = 0.04f;
    float e_ = 0.8f;
    float ar_ = 10.0f;
};

}
