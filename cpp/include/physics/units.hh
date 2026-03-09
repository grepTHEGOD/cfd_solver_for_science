#pragma once

#include <string>

namespace cfd {

enum class UnitSystem {
    SI,
    IMPERIAL
};

enum class SpeedUnit {
    MPH,
    MACH,
    M_S,
    KNOTS
};

enum class LengthUnit {
    METERS,
    FEET,
    KILOMETERS,
    MILES
};

class Units {
public:
    Units() = default;
    
    static float convert_speed(float value, SpeedUnit from, SpeedUnit to);
    static float convert_length(float value, LengthUnit from, LengthUnit to);
    static float convert_pressure(float value, UnitSystem from, UnitSystem to);
    static float convert_temperature(float value, bool to_kelvin);
    static float convert_area(float value, UnitSystem from, UnitSystem to);
    static float convert_force(float value, UnitSystem from, UnitSystem to);
    static float convert_density(float value, UnitSystem from, UnitSystem to);
    
    static std::string speed_unit_string(SpeedUnit unit);
    static std::string length_unit_string(LengthUnit unit);
    
    static constexpr float MPH_TO_MS = 0.44704f;
    static constexpr float MS_TO_MPH = 2.23694f;
    static constexpr float FT_TO_M = 0.3048f;
    static constexpr float M_TO_FT = 3.28084f;
    static constexpr float KNOT_TO_MS = 0.514444f;
    static constexpr float MI_TO_M = 1609.34f;
    static constexpr float KM_TO_M = 1000.0f;
    static constexpr float LBF_TO_N = 4.44822f;
    static constexpr float PSI_TO_PA = 6894.76f;
};

}
