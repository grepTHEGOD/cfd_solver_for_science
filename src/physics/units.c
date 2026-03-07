#ifndef UNITS_C
#define UNITS_C

#include "units.h"

double units_convert_speed(double value, SpeedUnit from, SpeedUnit to) {
    double value_ms = 0.0;
    
    switch (from) {
        case UNIT_M_S: value_ms = value; break;
        case UNIT_KMH: value_ms = value / 3.6; break;
        case UNIT_MPH: value_ms = value * 0.44704; break;
        case UNIT_KNOTS: value_ms = value * 0.514444; break;
        case UNIT_MACH: value_ms = value * 343.0; break;
    }
    
    switch (to) {
        case UNIT_M_S: return value_ms;
        case UNIT_KMH: return value_ms * 3.6;
        case UNIT_MPH: return value_ms / 0.44704;
        case UNIT_KNOTS: return value_ms / 0.514444;
        case UNIT_MACH: return value_ms / 343.0;
    }
    
    return value_ms;
}

double units_convert_distance(double value, DistanceUnit from, DistanceUnit to) {
    double value_m = 0.0;
    
    switch (from) {
        case UNIT_METERS: value_m = value; break;
        case UNIT_FEET: value_m = value * 0.3048; break;
        case UNIT_KM: value_m = value * 1000.0; break;
        case UNIT_MILES: value_m = value * 1609.344; break;
    }
    
    switch (to) {
        case UNIT_METERS: return value_m;
        case UNIT_FEET: return value_m / 0.3048;
        case UNIT_KM: return value_m / 1000.0;
        case UNIT_MILES: return value_m / 1609.344;
    }
    
    return value_m;
}

double units_convert_pressure(double value, PressureUnit from, PressureUnit to) {
    double value_pa = 0.0;
    
    switch (from) {
        case UNIT_PASCAL: value_pa = value; break;
        case UNIT_KPA: value_pa = value * 1000.0; break;
        case UNIT_PSI: value_pa = value * 6894.76; break;
        case UNIT_ATM: value_pa = value * 101325.0; break;
        case UNIT_BAR: value_pa = value * 100000.0; break;
    }
    
    switch (to) {
        case UNIT_PASCAL: return value_pa;
        case UNIT_KPA: return value_pa / 1000.0;
        case UNIT_PSI: return value_pa / 6894.76;
        case UNIT_ATM: return value_pa / 101325.0;
        case UNIT_BAR: return value_pa / 100000.0;
    }
    
    return value_pa;
}

double units_convert_temperature(double value, TemperatureUnit from, TemperatureUnit to) {
    double value_k = 0.0;
    
    switch (from) {
        case UNIT_KELVIN: value_k = value; break;
        case UNIT_CELSIUS: value_k = value + 273.15; break;
        case UNIT_FAHRENHEIT: value_k = (value - 32.0) * 5.0 / 9.0 + 273.15; break;
    }
    
    switch (to) {
        case UNIT_KELVIN: return value_k;
        case UNIT_CELSIUS: return value_k - 273.15;
        case UNIT_FAHRENHEIT: return (value_k - 273.15) * 9.0 / 5.0 + 32.0;
    }
    
    return value_k;
}

#endif
