#ifndef ATMOSPHERE_C
#define ATMOSPHERE_C

#include "atmosphere.h"
#include <math.h>

void atmosphere_update(AtmosphereData *atm, double altitude) {
    if (!atm) return;
    
    atm->altitude = altitude;
    
    const double g = 9.80665;
    const double R = 287.058;
    const double gamma = 1.4;
    const double T0 = 288.15;
    const double P0 = 101325.0;
    const double rho0 = 1.225;
    
    if (altitude < 11000.0) {
        double T = T0 - 0.0065 * altitude;
        atm->temperature = T;
        atm->pressure = P0 * pow(T / T0, 5.2561);
        atm->density = atm->pressure / (R * T);
        atm->sound_speed = sqrt(gamma * R * T);
    } else {
        double T = 216.65;
        atm->temperature = T;
        atm->pressure = 22632.0 * exp(-g * (altitude - 11000.0) / (R * T));
        atm->density = atm->pressure / (R * T);
        atm->sound_speed = sqrt(gamma * R * T);
    }
    
    atm->dynamic_viscosity = 1.81e-5 * pow(atm->temperature / 273.15, 1.5) * (273.15 + 110.4) / (atm->temperature + 110.4);
    atm->kinematic_viscosity = atm->dynamic_viscosity / atm->density;
}

double atmosphere_get_pressure(double altitude) {
    double T = (altitude < 11000.0) ? (288.15 - 0.0065 * altitude) : 216.65;
    double P = (altitude < 11000.0) ? 
        101325.0 * pow(T / 288.15, 5.2561) : 
        22632.0 * exp(-9.80665 * (altitude - 11000.0) / (287.058 * 216.65));
    return P;
}

double atmosphere_get_temperature(double altitude) {
    return (altitude < 11000.0) ? (288.15 - 0.0065 * altitude) : 216.65;
}

double atmosphere_get_density(double altitude) {
    double T = atmosphere_get_temperature(altitude);
    double P = atmosphere_get_pressure(altitude);
    return P / (287.058 * T);
}

double atmosphere_get_sound_speed(double temperature) {
    return sqrt(1.4 * 287.058 * temperature);
}

double atmosphere_get_dynamic_viscosity(double temperature) {
    return 1.81e-5 * pow(temperature / 273.15, 1.5) * (273.15 + 110.4) / (temperature + 110.4);
}

double atmosphere_get_kinematic_viscosity(double density, double dynamic_viscosity) {
    return dynamic_viscosity / density;
}

double atmosphere_get_mach_number(double velocity, double sound_speed) {
    return velocity / sound_speed;
}

double atmosphere_get_reynolds_number(double density, double velocity, double length, double viscosity) {
    return density * velocity * length / viscosity;
}

double atmosphere_get_dynamic_pressure(double density, double velocity) {
    return 0.5 * density * velocity * velocity;
}

#endif
