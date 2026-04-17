#ifndef PHYSICS_ENGINE_HPP
#define PHYSICS_ENGINE_HPP

#include <cmath>
#include <vector>

namespace Physics {

struct AtmosphericProperties {
    double altitude;
    double temperature;
    double pressure;
    double density;
    double speed_of_sound;
    double dynamic_viscosity;
    double kinematic_viscosity;
    double Reynolds_number;
    double Mach_number;
};

struct AerodynamicForces {
    double lift;
    double drag;
    double side_force;
    double moment_x;
    double moment_y;
    double moment_z;
    double Cl;
    double Cd;
};

struct AeroCoefficients {
    double Mach;
    double alpha;
    double beta;
    double Cl_alpha;
    double Cd0;
    double k;
    double e;
    double AR;
};

struct FlightResult {
    double distance_traveled;
    double time_of_flight;
    double final_velocity;
    double max_altitude;
    double drag_force;
    double lift_force;
    double kinetic_energy;
    double potential_energy;
};

struct PhysicsParams {
    double velocity;          // m/s
    double altitude;          // meters
    double distance;          // meters
    double air_resistance;    // coefficient (0-1)
    double mass;              // kg
    double gravity;           // m/s^2
    double wing_area;         // m^2
    double drag_coefficient;
    double lift_coefficient;
    bool use_feet;            // true = feet, false = meters
};

const double R_GAS = 287.05;
const double G = 9.80665;
const double P0 = 101325.0;
const double T0 = 288.15;
const double RHO0 = 1.225;
const double LAPSE_RATE = 0.0065;
const double H_TROPO = 11000.0;

class PhysicsEngine {
public:
    static AtmosphericProperties atmosphereISA(double altitude_m);
    static AtmosphericProperties atmosphereStandard(double V, double altitude_m);
    static double celsiusToKelvin(double celsius);
    static double kelvinToCelsius(double kelvin);
    static double feetToMeters(double feet);
    static double metersToFeet(double meters);

    static void computeLiftCoeff(double& Cl, double alpha, const AeroCoefficients& coeffs);
    static void computeDragCoeff(double& Cd, double alpha, const AeroCoefficients& coeffs);
    static void computeLiftDrag(AerodynamicForces& forces, double q, double S, double alpha, const AeroCoefficients& coeffs);
    static double computeDynamicPressure(double rho, double V);
    
    static FlightResult computeFlight(double initial_velocity, double altitude, double distance,
                                       double mass, double gravity, double drag_coeff, 
                                       double lift_coeff, double wing_area, double air_resistance);
    
    static double computeDragForce(double velocity, double density, double drag_coeff, double area);
    static double computeLiftForce(double velocity, double density, double lift_coeff, double area);
    static double computeReynoldsNumber(double velocity, double length, double viscosity);
    static double computeMachNumber(double velocity, double speed_of_sound);
    
    static double computeKineticEnergy(double mass, double velocity);
    static double computePotentialEnergy(double mass, double altitude, double gravity);
    
    static double linearInterpolation(double x, const std::vector<double>& xData, const std::vector<double>& yData);
    static double computeAirDensityAtAltitude(double altitude);
    static double computeSpeedOfSound(double temperature);
    static double computeDynamicViscosity(double temperature);
};

}

#endif