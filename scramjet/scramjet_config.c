#include "scramjet_config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846
#define G 9.81
#define R_AIR 287.05
#define P_ATM_SEA 101325.0
#define T_ATM_SEA 288.15
#define GAMMA_AIR 1.4
#define CP_AIR 1004.5
#define Q_FUEL_H2 120.0e6
#define Q_FUEL_CH4 55.5e6
#define Q_FUEL_C2H4 47.0e6

ScramjetType getScramjetTypeFromString(const char *type_str) {
    if (strcmp(type_str, "scramjet") == 0 || strcmp(type_str, "pure") == 0)
        return SCRAMJET_TYPE_PURE;
    if (strcmp(type_str, "shcramjet") == 0 || strcmp(type_str, "shc") == 0)
        return SCRAMJET_TYPE_SHCRAMJET;
    if (strcmp(type_str, "dualmode") == 0 || strcmp(type_str, "dual") == 0)
        return SCRAMJET_TYPE_DUALMODE;
    return SCRAMJET_TYPE_PURE;
}

ScramjetSize getScramjetSizeFromString(const char *size_str) {
    if (strcmp(size_str, "small") == 0 || strcmp(size_str, "sm") == 0)
        return SCRAMJET_SIZE_SMALL;
    if (strcmp(size_str, "medium") == 0 || strcmp(size_str, "med") == 0 || strcmp(size_str, "md") == 0)
        return SCRAMJET_SIZE_MEDIUM;
    if (strcmp(size_str, "large") == 0 || strcmp(size_str, "lg") == 0)
        return SCRAMJET_SIZE_LARGE;
    if (strcmp(size_str, "xlarge") == 0 || strcmp(size_str, "xl") == 0 || strcmp(size_str, "huge") == 0)
        return SCRAMJET_SIZE_XLARGE;
    return SCRAMJET_SIZE_MEDIUM;
}

ScramjetFuel getScramjetFuelFromString(const char *fuel_str) {
    if (strcmp(fuel_str, "hydrogen") == 0 || strcmp(fuel_str, "h2") == 0)
        return FUEL_HYDROGEN;
    if (strcmp(fuel_str, "methane") == 0 || strcmp(fuel_str, "ch4") == 0)
        return FUEL_METHANE;
    if (strcmp(fuel_str, "ethylene") == 0 || strcmp(fuel_str, "c2h4") == 0)
        return FUEL_ETHYLENE;
    return FUEL_HYDROGEN;
}

ScramjetCooling getScramjetCoolingFromString(const char *cooling_str) {
    if (strcmp(cooling_str, "air") == 0 || strcmp(cooling_str, "bleed") == 0)
        return COOLING_AIR;
    if (strcmp(cooling_str, "regenerative") == 0 || strcmp(cooling_str, "regen") == 0)
        return COOLING_REGENERATIVE;
    if (strcmp(cooling_str, "film") == 0)
        return COOLING_FILM;
    if (strcmp(cooling_str, "transpiration") == 0 || strcmp(cooling_str, "porous") == 0)
        return COOLING_TRANSPIRATION;
    return COOLING_REGENERATIVE;
}

ScramjetInletType getInletTypeFromString(const char *inlet_str) {
    if (strcmp(inlet_str, "2d wedge") == 0 || strcmp(inlet_str, "wedge") == 0)
        return INLET_2D_WEDGE;
    if (strcmp(inlet_str, "3d conical") == 0 || strcmp(inlet_str, "conical") == 0)
        return INLET_3D_CONICAL;
    if (strcmp(inlet_str, "2d digital") == 0 || strcmp(inlet_str, "digital") == 0)
        return INLET_2D_DIGITAL;
    if (strcmp(inlet_str, "3d isometric") == 0 || strcmp(inlet_str, "isometric") == 0)
        return INLET_3D_ISOMETRIC;
    return INLET_2D_WEDGE;
}

const char *getScramjetTypeString(ScramjetType type) {
    switch(type) {
        case SCRAMJET_TYPE_PURE: return "Scramjet (Pure)";
        case SCRAMJET_TYPE_SHCRAMJET: return "SHCRAMJET";
        case SCRAMJET_TYPE_DUALMODE: return "Dual-Mode Scramjet";
        default: return "Unknown";
    }
}

const char *getScramjetSizeString(ScramjetSize size) {
    switch(size) {
        case SCRAMJET_SIZE_SMALL: return "Small";
        case SCRAMJET_SIZE_MEDIUM: return "Medium";
        case SCRAMJET_SIZE_LARGE: return "Large";
        case SCRAMJET_SIZE_XLARGE: return "X-Large";
        default: return "Unknown";
    }
}

const char *getScramjetFuelString(ScramjetFuel fuel) {
    switch(fuel) {
        case FUEL_HYDROGEN: return "Hydrogen";
        case FUEL_METHANE: return "Methane";
        case FUEL_ETHYLENE: return "Ethylene";
        case FUEL_PREFABRICATED: return "Pre-Fabricated";
        default: return "Unknown";
    }
}

const char *getScramjetCoolingString(ScramjetCooling cooling) {
    switch(cooling) {
        case COOLING_AIR: return "Air Bleed";
        case COOLING_REGENERATIVE: return "Regenerative";
        case COOLING_FILM: return "Film Cooling";
        case COOLING_TRANSPIRATION: return "Transpiration";
        default: return "Unknown";
    }
}

const char *getInletTypeString(ScramjetInletType inlet) {
    switch(inlet) {
        case INLET_2D_WEDGE: return "2D Wedge";
        case INLET_3D_CONICAL: return "3D Conical";
        case INLET_2D_DIGITAL: return "2D Digital";
        case INLET_3D_ISOMETRIC: return "3D Isometric";
        default: return "Unknown";
    }
}

const char *getScramjetTypeDescription(ScramjetType type) {
    switch(type) {
        case SCRAMJET_TYPE_PURE:
            return "Pure scramjet: Supersonic Combustion Ramjet. Operates at Mach 5+";
        case SCRAMJET_TYPE_SHCRAMJET:
            return "SHCRAMJET: Shrouded Helical Counterflow Rotating Advanced Ramjet. "
                   "Innovative design with internal compression and helical flow.";
        case SCRAMJET_TYPE_DUALMODE:
            return "Dual-Mode: Operates as ramjet at low Mach, transitions to scramjet at high Mach.";
        default: return "Unknown";
    }
}

const char *getScramjetSizeDescription(ScramjetSize size) {
    switch(size) {
        case SCRAMJET_SIZE_SMALL:
            return "Small: 50-200 kg thrust, 0.5-2 kg/s air flow, ~10 kg dry mass";
        case SCRAMJET_SIZE_MEDIUM:
            return "Medium: 200-1000 kg thrust, 2-10 kg/s air flow, ~50 kg dry mass";
        case SCRAMJET_SIZE_LARGE:
            return "Large: 1000-5000 kg thrust, 10-50 kg/s air flow, ~200 kg dry mass";
        case SCRAMJET_SIZE_XLARGE:
            return "X-Large: 5000-20000 kg thrust, 50-200 kg/s air flow, ~500 kg dry mass";
        default: return "Unknown";
    }
}

const char *getFuelDescription(ScramjetFuel fuel) {
    switch(fuel) {
        case FUEL_HYDROGEN:
            return "Hydrogen: Highest specific energy (120 MJ/kg), excellent cooling, requires cryogenic storage";
        case FUEL_METHANE:
            return "Methane: Moderate energy (55.5 MJ/kg), easier storage, regenerative cooling compatible";
        case FUEL_ETHYLENE:
            return "Ethylene: High density, good cooling, but soot formation issues";
        case FUEL_PREFABRICATED:
            return "Pre-fabricated: Custom fuel mixture for specific mission requirements";
        default: return "Unknown";
    }
}

const char *getCoolingDescription(ScramjetCooling cooling) {
    switch(cooling) {
        case COOLING_AIR:
            return "Air Bleed: Simple but inefficient, extracts air from compressor";
        case COOLING_REGENERATIVE:
            return "Regenerative: Flows fuel through channels before injection, best for H2";
        case COOLING_FILM:
            return "Film Cooling: Bleeds cool gas along walls, good for thermal protection";
        case COOLING_TRANSPIRATION:
            return "Transpiration: Porous walls with coolant flow, most effective but complex";
        default: return "Unknown";
    }
}

const char *getInletTypeDescription(ScramjetInletType inlet) {
    switch(inlet) {
        case INLET_2D_WEDGE:
            return "2D Wedge: Simple compression ramp, good for axisymmetric vehicles";
        case INLET_3D_CONICAL:
            return "3D Conical: Axisymmetric with sharp nose, highest compression";
        case INLET_2D_DIGITAL:
            return "2D Digital: Variable geometry, optimizes capture over wide Mach range";
        case INLET_3D_ISOMETRIC:
            return "3D Isometric: 3D flow path, complex but best performance";
        default: return "Unknown";
    }
}

double getDesignThrustForSize(ScramjetSize size) {
    switch(size) {
        case SCRAMJET_SIZE_SMALL: return 150.0;
        case SCRAMJET_SIZE_MEDIUM: return 500.0;
        case SCRAMJET_SIZE_LARGE: return 2500.0;
        case SCRAMJET_SIZE_XLARGE: return 10000.0;
        default: return 500.0;
    }
}

double getMaxAirflowForSize(ScramjetSize size) {
    switch(size) {
        case SCRAMJET_SIZE_SMALL: return 1.0;
        case SCRAMJET_SIZE_MEDIUM: return 5.0;
        case SCRAMJET_SIZE_LARGE: return 25.0;
        case SCRAMJET_SIZE_XLARGE: return 100.0;
        default: return 5.0;
    }
}

double getDryMassForSize(ScramjetSize size) {
    switch(size) {
        case SCRAMJET_SIZE_SMALL: return 10.0;
        case SCRAMJET_SIZE_MEDIUM: return 50.0;
        case SCRAMJET_SIZE_LARGE: return 200.0;
        case SCRAMJET_SIZE_XLARGE: return 500.0;
        default: return 50.0;
    }
}

double getVehicleMassForSize(ScramjetSize size) {
    switch(size) {
        case SCRAMJET_SIZE_SMALL: return 50.0;
        case SCRAMJET_SIZE_MEDIUM: return 200.0;
        case SCRAMJET_SIZE_LARGE: return 1000.0;
        case SCRAMJET_SIZE_XLARGE: return 3000.0;
        default: return 200.0;
    }
}

static double getAtmosphericPressure(double altitude) {
    if (altitude < 11000.0) {
        double T = T_ATM_SEA - 0.0065 * altitude;
        return P_ATM_SEA * pow(T / T_ATM_SEA, 5.2561);
    } else {
        double T = 216.65;
        double h = altitude - 11000.0;
        return 22632.0 * exp(-h / 7400.0);
    }
}

static double getAtmosphericTemperature(double altitude) {
    if (altitude < 11000.0) {
        return T_ATM_SEA - 0.0065 * altitude;
    } else {
        return 216.65;
    }
}

static double getAtmosphericDensity(double altitude) {
    double P = getAtmosphericPressure(altitude);
    double T = getAtmosphericTemperature(altitude);
    return P / (R_AIR * T);
}

static double getSpeedOfSound(double T) {
    return sqrt(GAMMA_AIR * R_AIR * T);
}

void getDefaultConfig(ScramjetConfig *config, ScramjetType type, ScramjetSize size) {
    config->type = type;
    config->size = size;
    config->fuel = FUEL_HYDROGEN;
    config->cooling = COOLING_REGENERATIVE;
    config->inlet_type = INLET_2D_WEDGE;

    switch(size) {
        case SCRAMJET_SIZE_SMALL:
            config->design_altitude = 25000.0;
            config->design_mach = 6.0;
            config->design_thrust = 150.0;
            config->operation_range_mach_min = 4.0;
            config->operation_range_mach_max = 10.0;
            config->vehicle_mass = 50.0;
            config->vehicle_length = 2.0;
            config->max_g_load = 10.0;
            config->inlet_capture_width = 0.3;
            config->inlet_ramp_angle = 15.0;
            config->combustor_length = 1.0;
            config->combustor_diameter = 0.2;
            config->nozzle_exit_diameter = 0.3;
            break;
        case SCRAMJET_SIZE_MEDIUM:
            config->design_altitude = 30000.0;
            config->design_mach = 7.0;
            config->design_thrust = 500.0;
            config->operation_range_mach_min = 4.0;
            config->operation_range_mach_max = 12.0;
            config->vehicle_mass = 200.0;
            config->vehicle_length = 3.5;
            config->max_g_load = 8.0;
            config->inlet_capture_width = 0.5;
            config->inlet_ramp_angle = 18.0;
            config->combustor_length = 1.5;
            config->combustor_diameter = 0.35;
            config->nozzle_exit_diameter = 0.5;
            break;
        case SCRAMJET_SIZE_LARGE:
            config->design_altitude = 35000.0;
            config->design_mach = 8.0;
            config->design_thrust = 2500.0;
            config->operation_range_mach_min = 4.0;
            config->operation_range_mach_max = 15.0;
            config->vehicle_mass = 1000.0;
            config->vehicle_length = 5.0;
            config->max_g_load = 6.0;
            config->inlet_capture_width = 0.8;
            config->inlet_ramp_angle = 20.0;
            config->combustor_length = 2.5;
            config->combustor_diameter = 0.6;
            config->nozzle_exit_diameter = 0.8;
            break;
        case SCRAMJET_SIZE_XLARGE:
            config->design_altitude = 40000.0;
            config->design_mach = 10.0;
            config->design_thrust = 10000.0;
            config->operation_range_mach_min = 5.0;
            config->operation_range_mach_max = 20.0;
            config->vehicle_mass = 3000.0;
            config->vehicle_length = 8.0;
            config->max_g_load = 5.0;
            config->inlet_capture_width = 1.5;
            config->inlet_ramp_angle = 25.0;
            config->combustor_length = 4.0;
            config->combustor_diameter = 1.0;
            config->nozzle_exit_diameter = 1.5;
            break;
    }

    config->autoignition_fuel = true;
    config->scram_to_ram_transition = (type == SCRAMJET_TYPE_DUALMODE);
    config->mode_switch_mach = 3.5;

    config->wall_temp_limit = 1500.0;
    config->max_thermal_flux = 5.0e6;
    config->cooling_channel_count = 10.0;
    config->regenerative_cooling = true;

    config->structural_analysis = true;
    config->thermal_analysis = true;
    config->structural_hfat = true;
}

void getGeometryForConfig(ScramjetGeometry *geo, ScramjetConfig *config) {
    double capture_diameter = config->inlet_capture_width;
    geo->inlet_capture_area = PI * capture_diameter * capture_diameter / 4.0;
    geo->max_air_flow = getMaxAirflowForSize(config->size);
    geo->design_mach = config->design_mach;
    geo->min_mach = config->operation_range_mach_min;
    geo->design_altitude = config->design_altitude;
    geo->cruise_velocity = config->design_mach * getSpeedOfSound(config->design_altitude);
    geo->design_thrust = config->design_thrust;
    geo->dry_mass = getDryMassForSize(config->size);
    geo->vehicle_mass = config->vehicle_mass;

    geo->inlet = NULL;
    geo->combustor = NULL;
    geo->nozzle = NULL;
}

void computeInletPerformance(ScramjetInlet *inlet, double mach, double altitude, 
                               double capture_area, ScramjetInletType inlet_type) {
    double a = getSpeedOfSound(getAtmosphericTemperature(altitude));
    double V = mach * a;
    double rho = getAtmosphericDensity(altitude);
    double P = getAtmosphericPressure(altitude);

    inlet->captured_mass_flow = rho * V * capture_area;

    double theta = 0.0;
    double beta = 0.0;

    switch(inlet_type) {
        case INLET_2D_WEDGE:
            theta = 15.0 * PI / 180.0;
            break;
        case INLET_3D_CONICAL:
            theta = 20.0 * PI / 180.0;
            break;
        case INLET_2D_DIGITAL:
            theta = 12.0 * PI / 180.0;
            break;
        case INLET_3D_ISOMETRIC:
            theta = 18.0 * PI / 180.0;
            break;
    }

    double M1 = mach;
    double M2 = sqrt((1.0 + (GAMMA_AIR-1.0)/2.0 * M1*M1) / (GAMMA_AIR * M1*M1 - (GAMMA_AIR-1.0)/2.0));
    M2 = (M1 > 1.0) ? M2 : M1 * 0.9;

    if (theta > 0.0 && M1 > 1.0) {
        double mu = asin(1.0 / M1);
        beta = mu + theta * 0.5;
    }

    inlet->shock_angle = beta;
    inlet->oblique_shock_angle = theta;
    inlet->inlet_area = capture_area;
    inlet->throat_area = capture_area * 0.7;
    inlet->diffuser_ratio = capture_area / inlet->throat_area;

    double T_ratio = (1.0 + (GAMMA_AIR-1.0)/2.0 * M1*M1) / (1.0 + (GAMMA_AIR-1.0)/2.0 * M2*M2);
    inlet->temperature_rise = T_ratio;

    if (M2 > 0.0) {
        inlet->total_pressure_loss = pow((GAMMA_AIR+1.0)*M1*M1 / (2.0 + (GAMMA_AIR-1.0)*M1*M1), GAMMA_AIR/(GAMMA_AIR-1.0)) *
                                   pow((GAMMA_AIR-1.0)/(2.0*GAMMA_AIR), 1.0/(GAMMA_AIR-1.0));
        inlet->total_pressure_loss = 1.0 - inlet->total_pressure_loss;
    } else {
        inlet->total_pressure_loss = 0.3;
    }

    inlet->capture_ratio = 0.85;
    inlet->external_mass_flow = inlet->captured_mass_flow * (1.0 - inlet->capture_ratio);
}

void computeCombustorPerformance(ScramjetCombustor *comb, double mach, double altitude,
                                 double air_flow, ScramjetFuel fuel, double equivalence) {
    double T = getAtmosphericTemperature(altitude);
    double P = getAtmosphericPressure(altitude);

    double inlet_T = T * (1.0 + 0.2 * mach * mach);
    double inlet_P = P * (1.0 + 0.2 * mach * mach);

    comb->air_mass_flow = air_flow;
    comb->chamber_area = 0.1;
    comb->chamber_length = 1.5;
    comb->residence_time = 0.003;

    double fuel_energy = 0.0;
    switch(fuel) {
        case FUEL_HYDROGEN: fuel_energy = Q_FUEL_H2; break;
        case FUEL_METHANE: fuel_energy = Q_FUEL_CH4; break;
        case FUEL_ETHYLENE: fuel_energy = Q_FUEL_C2H4; break;
        default: fuel_energy = Q_FUEL_H2;
    }

    double sta = equivalence * 1.0;
    comb->fuel_air_ratio = sta * 0.029;
    comb->fuel_flow_rate = comb->fuel_air_ratio * air_flow;
    comb->heat_release = comb->fuel_flow_rate * fuel_energy;

    double Cp = CP_AIR;
    double T_exit = inlet_T + comb->heat_release / (air_flow * Cp);

    comb->peak_temperature = T_exit;
    comb->combustion_efficiency = 0.92;
    comb->burnout_percent = 95.0;
    comb->chamber_equivalence_ratio = equivalence;
}

void computeNozzlePerformance(ScramjetNozzle *nozzle, double chamber_pressure, 
                             double chamber_temp, double throat_area, 
                             double exit_pressure, double gamma) {
    double A_star = throat_area;
    double P0 = chamber_pressure;
    double T0 = chamber_temp;

    double A_exit = A_star * 3.0;
    double area_ratio = A_exit / A_star;

    double pe_p0 = pow(2.0 / (gamma + 1.0), gamma / (gamma - 1.0));
    double Me = sqrt(2.0 / (gamma - 1.0) * (pow(1.0/pe_p0 * 1.0/area_ratio, (gamma-1.0)/gamma) - 1.0));

    if (Me < 1.0) Me = 2.5;

    nozzle->exit_area = A_exit;
    nozzle->throat_area = A_star;
    nozzle->expansion_ratio = area_ratio;

    double T_ratio = 1.0 + (gamma - 1.0) / 2.0 * Me * Me;
    nozzle->exit_temperature = T0 / T_ratio;
    nozzle->exit_pressure = P0 * pow(2.0 / (gamma + 1.0) / T_ratio, gamma / (gamma - 1.0));

    double a_exit = sqrt(gamma * R_AIR * nozzle->exit_temperature);
    nozzle->exit_velocity = Me * a_exit;

    nozzle->thrust = (nozzle->exit_pressure - exit_pressure) * A_exit + 
                    nozzle->exit_velocity * 0.5 * nozzle->exit_area * 0.5;
    nozzle->thrust = nozzle->thrust > 0 ? nozzle->thrust : 1000.0;

    nozzle->isp = nozzle->thrust / (0.5 * 9.81);
    nozzle->specific_impulse = nozzle->isp;
    nozzle->thrust_coefficient = 1.5;
    nozzle->nozzle_efficiency = 0.92;
}

void getPerformanceAtConditions(ScramjetPerformance *perf, ScramjetConfig *config, 
                                 double mach, double altitude, double velocity) {
    double T = getAtmosphericTemperature(altitude);
    double rho = getAtmosphericDensity(altitude);
    double P = getAtmosphericPressure(altitude);
    double a = getSpeedOfSound(T);
    double V = mach * a;

    double capture_area = PI * config->inlet_capture_width * config->inlet_capture_width / 4.0;
    
    ScramjetInlet inlet_data;
    computeInletPerformance(&inlet_data, mach, altitude, capture_area, config->inlet_type);

    double air_flow = rho * V * capture_area * inlet_data.capture_ratio;

    ScramjetCombustor comb_data;
    computeCombustorPerformance(&comb_data, mach, altitude, air_flow, config->fuel, 0.8);

    double throat_area = capture_area * 0.6;
    ScramjetNozzle nozzle_data;
    computeNozzlePerformance(&nozzle_data, P * 2.0, comb_data.peak_temperature, throat_area, P, GAMMA_AIR);

    double q = 0.5 * rho * V * V;
    double drag = q * 0.5 * (config->inlet_capture_width * config->inlet_capture_width * PI / 4.0);

    perf->thrust = nozzle_data.thrust - drag;
    perf->thrust = perf->thrust > 0 ? perf->thrust : config->design_thrust;
    perf->fuel_flow = comb_data.fuel_flow_rate;
    perf->thrust_coefficient = perf->thrust / (q * 0.1);
    perf->sftc = (perf->fuel_flow > 0) ? perf->fuel_flow / perf->thrust * 1000.0 : 0.0;
    perf->isp = (perf->fuel_flow > 0) ? perf->thrust / (perf->fuel_flow * G) : 3000.0;
    perf->specific_impulse = perf->isp;

    perf->thermal_efficiency = 0.35;
    perf->propulsive_efficiency = 0.55;
    perf->overall_efficiency = perf->thermal_efficiency * perf->propulsive_efficiency;

    perf->air_mass_flow = air_flow;
    perf->fuel_mass_flow = comb_data.fuel_flow_rate;
    perf->equivalence_ratio = comb_data.chamber_equivalence_ratio;
    perf->oa_ratios[0] = inlet_data.total_pressure_loss;
    perf->oa_ratios[1] = comb_data.combustion_efficiency;
    perf->oa_ratios[2] = nozzle_data.nozzle_efficiency;

    perf->inlet_temperature = inlet_data.temperature_rise * T;
    perf->inlet_pressure = P * (1.0 + 0.2 * mach * mach);
    perf->inlet_density = rho * 1.5;
    perf->exit_temperature = nozzle_data.exit_temperature;
    perf->exit_pressure = nozzle_data.exit_pressure;
    perf->exit_velocity = nozzle_data.exit_velocity;

    perf->drag_force = drag;
    perf->ram_drag = inlet_data.total_pressure_loss * air_flow * V;
    perf->friction_drag = drag * 0.1;

    perf->total_pressure_ratio = 1.0 - inlet_data.total_pressure_loss;
    perf->combustion_temperature = comb_data.peak_temperature;
    perf->cooling_required = comb_data.heat_release * 0.3;
    perf->thermal_load = comb_data.heat_release / (config->combustor_length * PI * config->combustor_diameter);
}