#include <physics/forces.hh>
#include <app/scene.hh>
#include <utils/math.hh>

namespace cfd {

AerodynamicForces Forces::compute(float velocity, float density, float aoa,
                                 float reference_area, float reference_length,
                                 float mach, float reynolds) {
    AerodynamicForces result;
    
    float cl = lift_coefficient(aoa, mach, reynolds);
    float cd = drag_coefficient(aoa, mach, reynolds);
    
    float q = 0.5f * density * velocity * velocity;
    
    result.lift = cl * q * reference_area;
    result.drag = cd * q * reference_area;
    result.side = side_coefficient(0.0f) * q * reference_area;
    
    result.lift_coefficient = cl;
    result.drag_coefficient = cd;
    result.side_coefficient = side_coefficient(0.0f);
    
    return result;
}

float Forces::lift_coefficient(float aoa, float mach, float reynolds) const {
    float cl_alpha_eff = cl_alpha_ * std::sqrt(1.0f - mach * mach);
    float cl = cl_alpha_eff * aoa;
    
    if (std::abs(aoa) > 0.3f) {
        float cl_max = 1.5f;
        float aoa_stall = 0.35f;
        if (std::abs(aoa) > aoa_stall) {
            cl = cl_max * std::sin(2.0f * aoa);
        }
    }
    
    return cl;
}

float Forces::drag_coefficient(float aoa, float mach, float reynolds) const {
    float cl = lift_coefficient(aoa, mach, reynolds);
    
    float cd_induced = induced_drag(cl, e_, ar_);
    float cd_wave = wave_drag(mach, aoa);
    float cd_parasite = parasite_drag(0.01f, 1.0f, 1.0f, 1.0f);
    
    return cd0_ + cd_induced + cd_wave + cd_parasite;
}

float Forces::side_coefficient(float beta) const {
    return 0.001f * beta;
}

float Forces::form_drag(float cd0, float cl, float k, float e, float ar) const {
    return cd0 + k * cl * cl;
}

float Forces::induced_drag(float cl, float e, float ar) const {
    if (ar <= 0.0f) return 0.0f;
    return (cl * cl) / (PI * e * ar);
}

float Forces::wave_drag(float mach, float aoa) const {
    if (mach < 1.0f) return 0.0f;
    
    float mc = mach - 1.0f;
    float wave_drag = 0.0f;
    
    if (mc < 0.3f) {
        wave_drag = 0.1f * mc * mc * std::abs(aoa);
    } else {
        wave_drag = 0.1f * (mc + 0.1f) * std::abs(aoa);
    }
    
    return wave_drag;
}

float Forces::parasite_drag(float cf, float re_ratio, float swet, float area) const {
    return cf * re_ratio * swet / area;
}

float Forces::degrees_to_radians(float degrees) {
    return degrees * DEG_TO_RAD;
}

float Forces::radians_to_degrees(float radians) {
    return radians * RAD_TO_DEG;
}

}
