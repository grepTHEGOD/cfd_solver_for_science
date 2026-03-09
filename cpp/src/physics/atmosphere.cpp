#include <physics/atmosphere.hh>
#include <utils/math.hh>

namespace cfd {

AtmosphericConditions Atmosphere::compute(float altitude_meters) const {
    AtmosphericConditions conditions;
    
    conditions.temperature = temperature_at_altitude(altitude_meters);
    conditions.pressure = pressure_at_altitude(altitude_meters);
    conditions.density = density_from_pressure(conditions.pressure, conditions.temperature);
    conditions.viscosity = viscosity(conditions.temperature);
    conditions.speed_of_sound = speed_of_sound(conditions.temperature);
    conditions.mach_limit = conditions.speed_of_sound > 0 ? 10.0f : 0.0f;
    
    return conditions;
}

float Atmosphere::speed_of_sound(float temperature_k) const {
    return std::sqrt(GAMMA * R_AIR * temperature_k);
}

float Atmosphere::pressure_at_altitude(float altitude_meters) const {
    if (altitude_meters <= TROPO_PAUSE) {
        float temp_ratio = 1.0f + (LAPSE_RATE_TROPO / SEA_LEVEL_TEMP) * altitude_meters;
        return SEA_LEVEL_PRESSURE * std::pow(temp_ratio, -GRAVITY / (LAPSE_RATE_TROPO * R_AIR));
    } else {
        float temp_at_tropo = temperature_at_altitude(TROPO_PAUSE);
        float pressure_at_tropo = SEA_LEVEL_PRESSURE * std::pow(1.0f + LAPSE_RATE_TROPO * TROPO_PAUSE / SEA_LEVEL_TEMP, 
                                                                  -GRAVITY / (LAPSE_RATE_TROPO * R_AIR));
        return pressure_at_tropo * std::exp(-GRAVITY * (altitude_meters - TROPO_PAUSE) / (R_AIR * temp_at_tropo));
    }
}

float Atmosphere::temperature_at_altitude(float altitude_meters) const {
    if (altitude_meters <= TROPO_PAUSE) {
        return SEA_LEVEL_TEMP + LAPSE_RATE_TROPO * altitude_meters;
    } else if (altitude_meters <= STRATO_START) {
        return temperature_at_altitude(TROPO_PAUSE);
    } else {
        return temperature_at_altitude(TROPO_PAUSE) + 0.001f * (altitude_meters - STRATO_START);
    }
}

float Atmosphere::density_from_pressure(float pressure, float temperature) const {
    return pressure / (R_AIR * temperature);
}

float Atmosphere::viscosity(float temperature_k) const {
    constexpr float SUTHERLAND_CONSTANT = 110.4f;
    constexpr float VISCOSITY_REF = 1.716e-5f;
    float temp_ratio = temperature_k / SEA_LEVEL_TEMP;
    return VISCOSITY_REF * std::pow(temp_ratio, 1.5f) * (SEA_LEVEL_TEMP + SUTHERLAND_CONSTANT) / (temperature_k + SUTHERLAND_CONSTANT);
}

float Atmosphere::reynolds_number(float velocity, float length, float density, float viscosity) const {
    if (viscosity <= 0.0f) return 0.0f;
    return density * velocity * length / viscosity;
}

float Atmosphere::dynamic_pressure(float density, float velocity) const {
    return 0.5f * density * velocity * velocity;
}

float Atmosphere::mach_from_velocity(float velocity, float speed_of_sound) const {
    if (speed_of_sound <= 0.0f) return 0.0f;
    return velocity / speed_of_sound;
}

float Atmosphere::velocity_from_mach(float mach, float speed_of_sound) const {
    return mach * speed_of_sound;
}

float Atmosphere::feet_to_meters(float feet) {
    return feet * FT_TO_M;
}

float Atmosphere::meters_to_feet(float meters) {
    return meters * M_TO_FT;
}

float Atmosphere::mph_to_mps(float mph) {
    return mph * MPH_TO_MS;
}

float Atmosphere::mps_to_mph(float mps) {
    return mps * MS_TO_MPH;
}

float Atmosphere::knots_to_mps(float knots) {
    return knots * KNOT_TO_MS;
}

}
