#ifndef HEAT_TRANSFER_H
#define HEAT_TRANSFER_H

#include <math.h>

typedef struct {
    double heat_flux;
    double surface_temperature;
    double convective_coeff;
    double stagnation_temp;
    double recovery_factor;
    double thermal_load;
} HeatResult;

typedef struct {
    double velocity;
    double density;
    double temperature;
    double mach;
    double surface_area;
    double characteristic_length;
    double gamma;
    double cp;
    double prandtl;
} HeatParams;

void heat_transfer_compute(HeatParams *params, HeatResult *result);
double heat_get_stagnation_temperature(double static_temp, double mach, double gamma);
double heat_get_convective_coeff(double rho, double v, double L, double pr, double mu);
double heat_get_heat_flux(double h, double T_stag, double T_wall);

#endif
