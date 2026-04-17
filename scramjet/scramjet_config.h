#ifndef SCRAMJET_CONFIG_H
#define SCRAMJET_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    SCRAMJET_TYPE_PURE,
    SCRAMJET_TYPE_SHCRAMJET,
    SCRAMJET_TYPE_DUALMODE
} ScramjetType;

typedef enum {
    SCRAMJET_SIZE_SMALL,
    SCRAMJET_SIZE_MEDIUM,
    SCRAMJET_SIZE_LARGE,
    SCRAMJET_SIZE_XLARGE
} ScramjetSize;

typedef enum {
    FUEL_HYDROGEN,
    FUEL_METHANE,
    FUEL_ETHYLENE,
    FUEL_PREFABRICATED
} ScramjetFuel;

typedef enum {
    COOLING_AIR,
    COOLING_REGENERATIVE,
    COOLING_FILM,
    COOLING_TRANSPIRATION
} ScramjetCooling;

typedef enum {
    INLET_2D_WEDGE,
    INLET_3D_CONICAL,
    INLET_2D_DIGITAL,
    INLET_3D_ISOMETRIC
} ScramjetInletType;

typedef struct {
    double captured_mass_flow;
    double external_mass_flow;
    double capture_ratio;
    double inlet_area;
    double throat_area;
    double diffuser_ratio;
    double shock_angle;
    double oblique_shock_angle;
    double total_pressure_loss;
    double temperature_rise;
} ScramjetInlet;

typedef struct {
    double chamber_length;
    double chamber_area;
    double chamber_equivalence_ratio;
    double fuel_flow_rate;
    double air_mass_flow;
    double peak_temperature;
    double heat_release;
    double combustion_efficiency;
    double burnout_percent;
    double residence_time;
    double fuel_air_ratio;
} ScramjetCombustor;

typedef struct {
    double exit_area;
    double throat_area;
    double expansion_ratio;
    double exit_pressure;
    double exit_temperature;
    double exit_velocity;
    double thrust;
    double specific_impulse;
    double isp;
    double thrust_coefficient;
    double nozzle_efficiency;
} ScramjetNozzle;

typedef struct {
    double inlet_capture_area;
    double max_air_flow;
    double design_mach;
    double min_mach;
    double cruise_altitude;
    double cruise_velocity;
    double max_thrust;
    double dry_mass;
    double vehicle_mass;
    double design_altitude;
    double design_thrust;
    void *inlet;
    void *combustor;
    void *nozzle;
} ScramjetGeometry;

typedef struct {
    double thrust;
    double thrust_coefficient;
    double fuel_flow;
    double sftc;
    double specific_impulse;
    double isp;

    double thermal_efficiency;
    double propulsive_efficiency;
    double overall_efficiency;

    double air_mass_flow;
    double fuel_mass_flow;
    double equivalence_ratio;
    double oa_ratios[3];

    double inlet_temperature;
    double inlet_pressure;
    double inlet_density;
    double exit_temperature;
    double exit_pressure;
    double exit_velocity;

    double drag_force;
    double ram_drag;
    double friction_drag;

    double total_pressure_ratio;
    double combustion_temperature;
    double cooling_required;
    double thermal_load;
} ScramjetPerformance;

typedef struct {
    ScramjetType type;
    ScramjetSize size;
    ScramjetFuel fuel;
    ScramjetCooling cooling;
    ScramjetInletType inlet_type;

    double design_altitude;
    double design_mach;
    double design_thrust;
    double operation_range_mach_min;
    double operation_range_mach_max;

    double vehicle_mass;
    double vehicle_length;
    double max_g_load;
    double integration_volume;

    double inlet_length;
    double inlet_capture_width;
    double inlet_ramp_angle;
    double combustor_length;
    double combustor_diameter;
    double nozzle_length;
    double nozzle_exit_diameter;
    double cruise_velocity;

    bool autoignition_fuel;
    bool scram_to_ram_transition;
    double mode_switch_mach;

    double wall_temp_limit;
    double max_thermal_flux;
    double cooling_channel_count;
    double regenerative_cooling;

    bool structural_analysis;
    bool thermal_analysis;
    bool structural_hfat;
} ScramjetConfig;

ScramjetType getScramjetTypeFromString(const char *type_str);
ScramjetSize getScramjetSizeFromString(const char *size_str);
ScramjetFuel getScramjetFuelFromString(const char *fuel_str);
ScramjetCooling getScramjetCoolingFromString(const char *cooling_str);
ScramjetInletType getInletTypeFromString(const char *inlet_str);

const char *getScramjetTypeString(ScramjetType type);
const char *getScramjetSizeString(ScramjetSize size);
const char *getScramjetFuelString(ScramjetFuel fuel);
const char *getScramjetCoolingString(ScramjetCooling cooling);
const char *getInletTypeString(ScramjetInletType inlet);

void getDefaultConfig(ScramjetConfig *config, ScramjetType type, ScramjetSize size);
void getGeometryForConfig(ScramjetGeometry *geo, ScramjetConfig *config);
void getPerformanceAtConditions(ScramjetPerformance *perf, ScramjetConfig *config, 
                               double mach, double altitude, double velocity);

const char *getScramjetTypeDescription(ScramjetType type);
const char *getScramjetSizeDescription(ScramjetSize size);
const char *getFuelDescription(ScramjetFuel fuel);
const char *getCoolingDescription(ScramjetCooling cooling);
const char *getInletTypeDescription(ScramjetInletType inlet);

double getDesignThrustForSize(ScramjetSize size);
double getMaxAirflowForSize(ScramjetSize size);
double getDryMassForSize(ScramjetSize size);
double getVehicleMassForSize(ScramjetSize size);

void computeInletPerformance(ScramjetInlet *inlet, double mach, double altitude, 
                            double capture_area, ScramjetInletType inlet_type);
void computeCombustorPerformance(ScramjetCombustor *comb, double mach, double altitude,
                                 double air_flow, ScramjetFuel fuel, double equivalence);
void computeNozzlePerformance(ScramjetNozzle *nozzle, double chamber_pressure, 
                               double chamber_temp, double throat_area, 
                               double exit_pressure, double gamma);

#endif