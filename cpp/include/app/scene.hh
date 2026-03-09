#pragma once

#include <memory>
#include <vector>
#include <string>
#include <array>
#include <functional>
#include <optional>
#include <chrono>

#include <GLFW/glfw3.h>

#include <utils/math.hh>
#include <utils/logger.hh>

namespace cfd {

struct SimulationParameters {
    float airspeed = 150.0f;
    float altitude = 0.0f;
    float distance = 1000.0f;
    float fuel = 5000.0f;
    float angle_of_attack = 0.0f;
    
    bool speed_in_mach = false;
    bool altitude_in_meters = false;
    
    float reference_area = 1.0f;
    float reference_length = 1.0f;
};

struct AtmosphericConditions {
    float pressure;
    float temperature;
    float density;
    float viscosity;
    float speed_of_sound;
    float mach_limit;
};

struct AerodynamicForces {
    float lift = 0.0f;
    float drag = 0.0f;
    float side = 0.0f;
    float lift_coefficient = 0.0f;
    float drag_coefficient = 0.0f;
    float side_coefficient = 0.0f;
};

struct HeatTransferResults {
    float heat_flux = 0.0f;
    float surface_temperature = 0.0f;
    float stagnation_temperature = 0.0f;
    float thermal_load = 0.0f;
};

struct DerivedValues {
    float mach_number = 0.0f;
    float reynolds_number = 0.0f;
    float dynamic_pressure = 0.0f;
    float air_density = 0.0f;
    float speed_of_sound = 0.0f;
};

struct ScramjetState {
    float inlet_capture_ratio = 0.0f;
    float combustion_temperature = 0.0f;
    float thrust = 0.0f;
    float specific_impulse = 0.0f;
    float thermal_efficiency = 0.0f;
    float propulsive_efficiency = 0.0f;
    float overall_efficiency = 0.0f;
};

}
