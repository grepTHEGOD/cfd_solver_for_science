#include "physics_engine.hpp"

namespace Physics {

AtmosphericProperties PhysicsEngine::atmosphereISA(double altitude_m) {
    AtmosphericProperties atm;
    atm.altitude = altitude_m;
    
    if (altitude_m < H_TROPO) {
        atm.temperature = T0 - LAPSE_RATE * altitude_m;
        double exponent = G / (R_GAS * LAPSE_RATE);
        atm.pressure = P0 * std::pow(1.0 - (LAPSE_RATE * altitude_m) / T0, exponent);
        atm.density = atm.pressure / (R_GAS * atm.temperature);
    } else {
        atm.temperature = T0 - LAPSE_RATE * H_TROPO;
        double p_tropo = P0 * std::pow(1.0 - (LAPSE_RATE * H_TROPO) / T0, G / (R_GAS * LAPSE_RATE));
        atm.pressure = p_tropo * std::exp(-G * (altitude_m - H_TROPO) / (R_GAS * atm.temperature));
        atm.density = atm.pressure / (R_GAS * atm.temperature);
    }
    
    atm.speed_of_sound = std::sqrt(1.4 * R_GAS * atm.temperature);
    
    double mu0 = 1.716e-5;
    double S = 110.4;
    atm.dynamic_viscosity = mu0 * std::pow(atm.temperature / T0, 1.5) * (T0 + S) / (atm.temperature + S);
    
    atm.kinematic_viscosity = atm.dynamic_viscosity / atm.density;
    
    return atm;
}

AtmosphericProperties PhysicsEngine::atmosphereStandard(double V, double altitude_m) {
    AtmosphericProperties atm = atmosphereISA(altitude_m);
    atm.Mach_number = V / atm.speed_of_sound;
    
    double L = 1.0;
    atm.Reynolds_number = (atm.density * V * L) / atm.dynamic_viscosity;
    
    return atm;
}

double PhysicsEngine::celsiusToKelvin(double celsius) {
    return celsius + 273.15;
}

double PhysicsEngine::kelvinToCelsius(double kelvin) {
    return kelvin - 273.15;
}

double PhysicsEngine::feetToMeters(double feet) {
    return feet * 0.3048;
}

double PhysicsEngine::metersToFeet(double meters) {
    return meters / 0.3048;
}

void PhysicsEngine::computeLiftCoeff(double& Cl, double alpha, const AeroCoefficients& coeffs) {
    double alpha_rad = alpha * M_PI / 180.0;
    double alpha0 = 0.0;
    double alpha_stall = 15.0 * M_PI / 180.0;
    
    if (std::fabs(alpha) < alpha_stall) {
        Cl = coeffs.Cl_alpha * (alpha_rad - alpha0);
    } else {
        double Cl_max = coeffs.Cl_alpha * alpha_stall;
        double stall_factor = std::exp(-std::pow((alpha_rad - alpha_stall) / (5.0 * M_PI / 180.0), 2));
        Cl = Cl_max * stall_factor;
    }
}

void PhysicsEngine::computeDragCoeff(double& Cd, double alpha, const AeroCoefficients& coeffs) {
    double alpha_rad = alpha * M_PI / 180.0;
    double Cd_induced = coeffs.k * std::pow(coeffs.Cl_alpha * alpha_rad, 2) / (M_PI * coeffs.e * coeffs.AR);
    Cd = coeffs.Cd0 + Cd_induced;
}

void PhysicsEngine::computeLiftDrag(AerodynamicForces& forces, double q, double S, double alpha, const AeroCoefficients& coeffs) {
    computeLiftCoeff(forces.Cl, alpha, coeffs);
    computeDragCoeff(forces.Cd, alpha, coeffs);
    
    forces.lift = forces.Cl * q * S;
    forces.drag = forces.Cd * q * S;
    forces.side_force = 0.0;
    forces.moment_y = 0.0;
}

double PhysicsEngine::computeDynamicPressure(double rho, double V) {
    return 0.5 * rho * V * V;
}

FlightResult PhysicsEngine::computeFlight(double initial_velocity, double altitude, double distance,
                                            double mass, double gravity, double drag_coeff, 
                                            double lift_coeff, double wing_area, double air_resistance) {
    FlightResult result;
    
    AtmosphericProperties atm = atmosphereStandard(initial_velocity, altitude);
    double rho = atm.density;
    
    double velocity = initial_velocity;
    double current_altitude = altitude;
    double current_distance = 0.0;
    double dt = 0.01;
    double max_altitude = altitude;
    double time = 0.0;
    
    air_resistance = std::max(0.0, std::min(1.0, air_resistance));
    double drag_multiplier = 1.0 + air_resistance * 2.0;
    
    while (current_distance < distance && velocity > 0.1) {
        double q = computeDynamicPressure(rho, velocity);
        
        double drag = drag_coeff * q * wing_area * drag_multiplier;
        double lift_val = lift_coeff * q * wing_area;
        (void)lift_val;
        
        double acceleration = -drag / mass;
        
        if (current_altitude > 0) {
            acceleration -= gravity;
        }
        
        velocity += acceleration * dt;
        if (velocity < 0) velocity = 0;
        
        current_distance += velocity * dt;
        
        if (current_distance > max_altitude * 10) {
            AtmosphericProperties new_atm = atmosphereStandard(velocity, current_altitude);
            rho = new_atm.density;
        }
        
        time += dt;
        
        if (time > 10000) break;
    }
    
    result.distance_traveled = current_distance;
    result.time_of_flight = time;
    result.final_velocity = velocity;
    result.max_altitude = max_altitude;
    result.drag_force = drag_coeff * computeDynamicPressure(rho, velocity) * wing_area * drag_multiplier;
    result.lift_force = lift_coeff * computeDynamicPressure(rho, velocity) * wing_area;
    result.kinetic_energy = computeKineticEnergy(mass, velocity);
    result.potential_energy = computePotentialEnergy(mass, current_altitude, gravity);
    
    return result;
}

double PhysicsEngine::computeDragForce(double velocity, double density, double drag_coeff, double area) {
    return 0.5 * density * velocity * velocity * drag_coeff * area;
}

double PhysicsEngine::computeLiftForce(double velocity, double density, double lift_coeff, double area) {
    return 0.5 * density * velocity * velocity * lift_coeff * area;
}

double PhysicsEngine::computeReynoldsNumber(double velocity, double length, double viscosity) {
    return (velocity * length) / viscosity;
}

double PhysicsEngine::computeMachNumber(double velocity, double speed_of_sound) {
    return velocity / speed_of_sound;
}

double PhysicsEngine::computeKineticEnergy(double mass, double velocity) {
    return 0.5 * mass * velocity * velocity;
}

double PhysicsEngine::computePotentialEnergy(double mass, double altitude, double gravity) {
    return mass * gravity * altitude;
}

double PhysicsEngine::linearInterpolation(double x, const std::vector<double>& xData, const std::vector<double>& yData) {
    if (xData.size() != yData.size() || xData.empty()) return 0.0;
    
    if (x <= xData.front()) return yData.front();
    if (x >= xData.back()) return yData.back();
    
    for (size_t i = 0; i < xData.size() - 1; i++) {
        if (x >= xData[i] && x <= xData[i + 1]) {
            double t = (x - xData[i]) / (xData[i + 1] - xData[i]);
            return yData[i] + t * (yData[i + 1] - yData[i]);
        }
    }
    
    return yData.back();
}

double PhysicsEngine::computeAirDensityAtAltitude(double altitude) {
    AtmosphericProperties atm = atmosphereISA(altitude);
    return atm.density;
}

double PhysicsEngine::computeSpeedOfSound(double temperature) {
    return std::sqrt(1.4 * R_GAS * temperature);
}

double PhysicsEngine::computeDynamicViscosity(double temperature) {
    double mu0 = 1.716e-5;
    double S = 110.4;
    return mu0 * std::pow(temperature / T0, 1.5) * (T0 + S) / (temperature + S);
}

}