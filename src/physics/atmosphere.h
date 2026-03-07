#ifndef ATMOSPHERE_H
#define ATMOSPHERE_H

#include <math.h>

typedef struct {
    double altitude;
    double pressure;
    double temperature;
    double density;
    double viscosity;
    double sound_speed;
    double dynamic_viscosity;
    double kinematic_viscosity;
    double mach_number;
    double reynolds_number;
    double dynamic_pressure;
} AtmosphereData;

typedef struct {
    double airspeed;
    double altitude;
    double angle_of_attack;
    double pressure;
    double temperature;
    double turbulence_intensity;
    double viscosity;
    double characteristic_length;
} SimulationParams;

typedef struct {
    int altitude_unit;
    int speed_unit;
    int temp_unit;
    int pressure_unit;
} UnitSettings;

void atmosphere_update(AtmosphereData *atm, double altitude);
double atmosphere_get_pressure(double altitude);
double atmosphere_get_temperature(double altitude);
double atmosphere_get_density(double altitude);
double atmosphere_get_sound_speed(double temperature);
double atmosphere_get_dynamic_viscosity(double temperature);
double atmosphere_get_kinematic_viscosity(double density, double dynamic_viscosity);
double atmosphere_get_mach_number(double velocity, double sound_speed);
double atmosphere_get_reynolds_number(double density, double velocity, double length, double viscosity);
double atmosphere_get_dynamic_pressure(double density, double velocity);

#endif
