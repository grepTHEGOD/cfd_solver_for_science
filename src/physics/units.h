#ifndef UNITS_H
#define UNITS_H

typedef enum {
    UNIT_M_S,
    UNIT_KMH,
    UNIT_MPH,
    UNIT_KNOTS,
    UNIT_MACH
} SpeedUnit;

typedef enum {
    UNIT_METERS,
    UNIT_FEET,
    UNIT_KM,
    UNIT_MILES
} DistanceUnit;

typedef enum {
    UNIT_PASCAL,
    UNIT_KPA,
    UNIT_PSI,
    UNIT_ATM,
    UNIT_BAR
} PressureUnit;

typedef enum {
    UNIT_KELVIN,
    UNIT_CELSIUS,
    UNIT_FAHRENHEIT
} TemperatureUnit;

double units_convert_speed(double value, SpeedUnit from, SpeedUnit to);
double units_convert_distance(double value, DistanceUnit from, DistanceUnit to);
double units_convert_pressure(double value, PressureUnit from, PressureUnit to);
double units_convert_temperature(double value, TemperatureUnit from, TemperatureUnit to);

#endif
