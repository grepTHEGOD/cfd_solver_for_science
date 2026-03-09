#pragma once

#include <cmath>
#include <algorithm>

#include <app/scene.hh>

namespace cfd {

constexpr float R_AIR = 287.05287f;
constexpr float GAMMA = 1.4f;
constexpr float SEA_LEVEL_TEMP = 288.15f;
constexpr float LAPSE_RATE_TROPO = -0.0065f;
constexpr float TROPO_PAUSE = 11000.0f;
constexpr float STRATO_START = 20000.0f;

class Atmosphere {
public:
    Atmosphere() = default;
    
    AtmosphericConditions compute(float altitude_meters) const;
    
    float speed_of_sound(float temperature_k) const;
    float pressure_at_altitude(float altitude_meters) const;
    float temperature_at_altitude(float altitude_meters) const;
    float density_from_pressure(float pressure, float temperature) const;
    float viscosity(float temperature_k) const;
    float reynolds_number(float velocity, float length, float density, float viscosity) const;
    float dynamic_pressure(float density, float velocity) const;
    float mach_from_velocity(float velocity, float speed_of_sound) const;
    float velocity_from_mach(float mach, float speed_of_sound) const;
    
    static float feet_to_meters(float feet);
    static float meters_to_feet(float meters);
    static float mph_to_mps(float mph);
    static float mps_to_mph(float mps);
    static float knots_to_mps(float knots);
};

}
