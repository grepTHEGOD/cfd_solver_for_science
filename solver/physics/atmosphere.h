#ifndef PHYSICS_ATMOSPHERE_H
#define PHYSICS_ATMOSPHERE_H

#include <math.h>

#define R_GAS 287.05
#define G 9.80665
#define P0 101325.0
#define T0 288.15
#define RHO0 1.225
#define LAPSE_RATE 0.0065
#define H_TROPO 11000.0

typedef struct {
    double altitude;
    double temperature;
    double pressure;
    double density;
    double speed_of_sound;
    double dynamic_viscosity;
    double kinematic_viscosity;
    double Reynolds_number;
    double Mach_number;
} AtmosphericProperties;

void atmosphereISA(AtmosphericProperties *atm, double altitude_m);
void atmosphereStandard(AtmosphericProperties *atm, double V, double altitude_m);
double celsiusToKelvin(double celsius);
double kelvinToCelsius(double kelvin);
double feetToMeters(double feet);
double metersToFeet(double meters);

#endif
