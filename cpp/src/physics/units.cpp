#include <physics/units.hh>

namespace cfd {

float Units::convert_speed(float value, SpeedUnit from, SpeedUnit to) {
    float ms = 0.0f;
    
    switch (from) {
        case SpeedUnit::MPH: ms = value * MPH_TO_MS; break;
        case SpeedUnit::M_S: ms = value; break;
        case SpeedUnit::KNOTS: ms = value * KNOT_TO_MS; break;
        case SpeedUnit::MACH: ms = value * 343.0f; break;
    }
    
    switch (to) {
        case SpeedUnit::MPH: return ms * MS_TO_MPH;
        case SpeedUnit::M_S: return ms;
        case SpeedUnit::KNOTS: return ms / KNOT_TO_MS;
        case SpeedUnit::MACH: return ms / 343.0f;
    }
    
    return value;
}

float Units::convert_length(float value, LengthUnit from, LengthUnit to) {
    float meters = 0.0f;
    
    switch (from) {
        case LengthUnit::METERS: meters = value; break;
        case LengthUnit::FEET: meters = value * FT_TO_M; break;
        case LengthUnit::KILOMETERS: meters = value * KM_TO_M; break;
        case LengthUnit::MILES: meters = value * MI_TO_M; break;
    }
    
    switch (to) {
        case LengthUnit::METERS: return meters;
        case LengthUnit::FEET: return meters * M_TO_FT;
        case LengthUnit::KILOMETERS: return meters / KM_TO_M;
        case LengthUnit::MILES: return meters / MI_TO_M;
    }
    
    return value;
}

float Units::convert_pressure(float value, UnitSystem from, UnitSystem to) {
    float pascals = (from == UnitSystem::SI) ? value : value * PSI_TO_PA;
    return (to == UnitSystem::SI) ? pascals : pascals / PSI_TO_PA;
}

float Units::convert_temperature(float value, bool to_kelvin) {
    return to_kelvin ? value + 273.15f : value - 273.15f;
}

float Units::convert_area(float value, UnitSystem from, UnitSystem to) {
    float sq_meters = (from == UnitSystem::SI) ? value : value * FT_TO_M * FT_TO_M;
    return (to == UnitSystem::SI) ? sq_meters : sq_meters / (FT_TO_M * FT_TO_M);
}

float Units::convert_force(float value, UnitSystem from, UnitSystem to) {
    float newtons = (from == UnitSystem::SI) ? value : value * LBF_TO_N;
    return (to == UnitSystem::SI) ? newtons : newtons / LBF_TO_N;
}

float Units::convert_density(float value, UnitSystem from, UnitSystem to) {
    return value;
}

std::string Units::speed_unit_string(SpeedUnit unit) {
    switch (unit) {
        case SpeedUnit::MPH: return "mph";
        case SpeedUnit::MACH: return "Mach";
        case SpeedUnit::M_S: return "m/s";
        case SpeedUnit::KNOTS: return "kts";
    }
    return "";
}

std::string Units::length_unit_string(LengthUnit unit) {
    switch (unit) {
        case LengthUnit::METERS: return "m";
        case LengthUnit::FEET: return "ft";
        case LengthUnit::KILOMETERS: return "km";
        case LengthUnit::MILES: return "mi";
    }
    return "";
}

}
