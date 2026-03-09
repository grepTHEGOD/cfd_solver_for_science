#pragma once

#include <cmath>
#include <algorithm>

#include <app/scene.hh>
#include <utils/math.hh>

namespace cfd {

class HeatTransfer {
public:
    HeatTransfer() = default;
    
    HeatTransferResults compute(float velocity, float density, float temperature,
                               float characteristic_length, float surface_area);
    
    float stagnation_temperature(float static_temp, float velocity, float gamma = 1.4f) const;
    float stagnation_pressure(float static_press, float mach, float gamma = 1.4f) const;
    float convective_heat_flux(float recovery_factor, float stagnation_temp, 
                             float static_temp, float mach) const;
    float film_coefficient(float heat_flux, float stagnation_temp, float wall_temp) const;
    float radiation_heat_loss(float emissivity, float surface_temp, float ambient_temp) const;
    float equilibrium_temperature(float convective_flux, float emissivity, float ambient_temp) const;
    
    static constexpr float PRANDTL_NUMBER = 0.71f;
    static constexpr float RECOVERY_FACTOR = 0.85f;
    static constexpr float STEFAN_BOLTZMANN = 5.670374419e-8f;

private:
    float prandtl_meyer_expansion(float mach_in, float gamma = 1.4f) const;
    float boundary_layer_thickness(float reynolds, float x) const;
    float skin_friction_coefficient(float reynolds, float x) const;
};

}
