#include <physics/heat_transfer.hh>
#include <app/scene.hh>
#include <utils/math.hh>

namespace cfd {

HeatTransferResults HeatTransfer::compute(float velocity, float density, float temperature,
                                       float characteristic_length, float surface_area) {
    HeatTransferResults result;
    
    result.stagnation_temperature = stagnation_temperature(temperature, velocity);
    
    result.heat_flux = convective_heat_flux(RECOVERY_FACTOR, result.stagnation_temperature, temperature, 
                                           velocity / speed_of_sound(temperature));
    
    result.surface_temperature = equilibrium_temperature(result.heat_flux, 0.8f, temperature);
    
    result.thermal_load = result.heat_flux * surface_area;
    
    return result;
}

float HeatTransfer::stagnation_temperature(float static_temp, float velocity, float gamma) const {
    float a = speed_of_sound(static_temp);
    float mach = velocity / a;
    float mach_sq = mach * mach;
    
    return static_temp * (1.0f + (gamma - 1.0f) / 2.0f * mach_sq);
}

float HeatTransfer::stagnation_pressure(float static_press, float mach, float gamma) const {
    float mach_sq = mach * mach;
    float gamma_ratio = (gamma - 1.0f) / (gamma + 1.0f);
    
    return static_press * std::pow(1.0f + gamma_ratio * mach_sq, gamma / (gamma - 1.0f));
}

float HeatTransfer::convective_heat_flux(float recovery_factor, float stagnation_temp, 
                                        float static_temp, float mach) const {
    float re = recovery_factor;
    float temp_diff = stagnation_temp - static_temp;
    
    float pr_exp = 0.333f;
    float heat_flux_coeff = 0.0001f * (1.0f + mach);
    
    return heat_flux_coeff * re * std::pow(pr_exp, 0.33f) * temp_diff;
}

float HeatTransfer::film_coefficient(float heat_flux, float stagnation_temp, float wall_temp) const {
    float temp_diff = stagnation_temp - wall_temp;
    if (temp_diff <= 0.0f) return 0.0f;
    return heat_flux / temp_diff;
}

float HeatTransfer::radiation_heat_loss(float emissivity, float surface_temp, float ambient_temp) const {
    float t4 = std::pow(surface_temp, 4.0f);
    float t_ambient4 = std::pow(ambient_temp, 4.0f);
    return emissivity * STEFAN_BOLTZMANN * (t4 - t_ambient4);
}

float HeatTransfer::equilibrium_temperature(float convective_flux, float emissivity, float ambient_temp) const {
    constexpr float NEWTON_ITERATIONS = 10;
    float temp = ambient_temp;
    
    for (int i = 0; i < NEWTON_ITERATIONS; ++i) {
        float rad_loss = radiation_heat_loss(emissivity, temp, ambient_temp);
        float residual = convective_flux - rad_loss;
        
        float t4 = std::pow(temp, 4.0f);
        float derivative = 4.0f * emissivity * STEFAN_BOLTZMANN * t4 / temp;
        
        if (std::abs(derivative) > 0.001f) {
            temp += residual / derivative;
        }
        
        temp = std::max(ambient_temp, temp);
    }
    
    return temp;
}

float HeatTransfer::prandtl_meyer_expansion(float mach_in, float gamma) const {
    if (mach_in <= 1.0f) return 0.0f;
    
    float gm1 = gamma - 1.0f;
    float gp1 = gamma + 1.0f;
    
    float nu = std::sqrt(gm1 / gp1) * std::atan(std::sqrt(gm1 / gp1 * (mach_in * mach_in - 1.0f))) -
               std::atan(std::sqrt(mach_in * mach_in - 1.0f));
    
    return nu * RAD_TO_DEG;
}

float HeatTransfer::boundary_layer_thickness(float reynolds, float x) const {
    if (reynolds <= 0.0f || x <= 0.0f) return 0.0f;
    return 5.0f * x / std::sqrt(reynolds);
}

float HeatTransfer::skin_friction_coefficient(float reynolds, float x) const {
    if (reynolds <= 0.0f || x <= 0.0f) return 0.0f;
    return 0.664f / std::sqrt(reynolds);
}

}
