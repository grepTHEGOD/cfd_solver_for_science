#include "atmosphere.h"

static double satVaporPressure(double T) {
    return 610.78 * exp(17.27 * (T - 273.15) / (T - 273.15 + 237.3));
}

void atmosphereISA(AtmosphericProperties *atm, double altitude_m) {
    atm->altitude = altitude_m;
    
    if (altitude_m < H_TROPO) {
        atm->temperature = T0 - LAPSE_RATE * altitude_m;
        double exponent = G / (R_GAS * LAPSE_RATE);
        atm->pressure = P0 * pow(1.0 - (LAPSE_RATE * altitude_m) / T0, exponent);
        atm->density = atm->pressure / (R_GAS * atm->temperature);
    } else {
        atm->temperature = T0 - LAPSE_RATE * H_TROPO;
        atm->pressure = P0 * pow(1.0 - (LAPSE_RATE * H_TROPO) / T0, G / (R_GAS * LAPSE_RATE)) * 
                        exp(-G * (altitude_m - H_TROPO) / (R_GAS * atm->temperature));
        atm->density = atm->pressure / (R_GAS * atm->temperature);
    }
    
    atm->speed_of_sound = sqrt(1.4 * R_GAS * atm->temperature);
    
    double mu0 = 1.716e-5;
    double S = 110.4;
    atm->dynamic_viscosity = mu0 * pow(atm->temperature / T0, 1.5) * (T0 + S) / (atm->temperature + S);
    
    atm->kinematic_viscosity = atm->dynamic_viscosity / atm->density;
}

void atmosphereStandard(AtmosphericProperties *atm, double V, double altitude_m) {
    atmosphereISA(atm, altitude_m);
    
    atm->Mach_number = V / atm->speed_of_sound;
    
    double L = 1.0;
    atm->Reynolds_number = (atm->density * V * L) / atm->dynamic_viscosity;
}

double celsiusToKelvin(double celsius) {
    return celsius + 273.15;
}

double kelvinToCelsius(double kelvin) {
    return kelvin - 273.15;
}

double feetToMeters(double feet) {
    return feet * 0.3048;
}

double metersToFeet(double meters) {
    return meters / 0.3048;
}
