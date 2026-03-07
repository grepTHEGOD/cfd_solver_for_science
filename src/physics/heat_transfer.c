#ifndef HEAT_TRANSFER_C
#define HEAT_TRANSFER_C

#include "heat_transfer.h"

void heat_transfer_compute(HeatParams *params, HeatResult *result) {
    if (!params || !result) return;
    
    result->stagnation_temp = heat_get_stagnation_temperature(
        params->temperature, 
        params->mach, 
        params->gamma
    );
    
    result->convective_coeff = heat_get_convective_coeff(
        params->density,
        params->velocity,
        params->characteristic_length,
        params->prandtl,
        1.8e-5
    );
    
    result->surface_temperature = params->temperature;
    result->recovery_factor = sqrt(params->prandtl);
    
    double recovery_temp = params->temperature * (1.0 + (params->gamma - 1.0) / 2.0 * params->mach * params->mach * result->recovery_factor);
    
    result->heat_flux = heat_get_heat_flux(
        result->convective_coeff,
        result->stagnation_temp,
        result->surface_temperature
    );
    
    result->thermal_load = result->heat_flux * params->surface_area;
}

double heat_get_stagnation_temperature(double static_temp, double mach, double gamma) {
    return static_temp * (1.0 + (gamma - 1.0) / 2.0 * mach * mach);
}

double heat_get_convective_coeff(double rho, double v, double L, double pr, double mu) {
    double reynolds = rho * v * L / mu;
    double nusselt = 0.0296 * pow(reynolds, 0.8) * pow(pr, 0.33);
    return nusselt * mu / L * 0.29;
}

double heat_get_heat_flux(double h, double T_stag, double T_wall) {
    return h * (T_stag - T_wall);
}

#endif
