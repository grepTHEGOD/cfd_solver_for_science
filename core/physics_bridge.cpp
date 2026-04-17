#include "physics_bridge.h"
#include "physics_engine.hpp"
#include "model_loader.hpp"
#include <cmath>
#include <memory>

static std::unique_ptr<ModelLoader::Mesh> g_mesh;
static ModelLoader::MeshProperties g_model_props;
static double g_model_drag = 0.3;
static double g_model_lift = 0.5;

extern "C" {

C_AtmosphericProperties c_atmosphere_isa(double altitude_m) {
    Physics::AtmosphericProperties atm = Physics::PhysicsEngine::atmosphereISA(altitude_m);
    C_AtmosphericProperties result;
    result.altitude = atm.altitude;
    result.temperature = atm.temperature;
    result.pressure = atm.pressure;
    result.density = atm.density;
    result.speed_of_sound = atm.speed_of_sound;
    result.dynamic_viscosity = atm.dynamic_viscosity;
    result.kinematic_viscosity = atm.kinematic_viscosity;
    result.Reynolds_number = atm.Reynolds_number;
    result.Mach_number = atm.Mach_number;
    return result;
}

C_AtmosphericProperties c_atmosphere_standard(double V, double altitude_m) {
    Physics::AtmosphericProperties atm = Physics::PhysicsEngine::atmosphereStandard(V, altitude_m);
    C_AtmosphericProperties result;
    result.altitude = atm.altitude;
    result.temperature = atm.temperature;
    result.pressure = atm.pressure;
    result.density = atm.density;
    result.speed_of_sound = atm.speed_of_sound;
    result.dynamic_viscosity = atm.dynamic_viscosity;
    result.kinematic_viscosity = atm.kinematic_viscosity;
    result.Reynolds_number = atm.Reynolds_number;
    result.Mach_number = atm.Mach_number;
    return result;
}

double c_celsius_to_kelvin(double celsius) {
    return Physics::PhysicsEngine::celsiusToKelvin(celsius);
}

double c_kelvin_to_celsius(double kelvin) {
    return Physics::PhysicsEngine::kelvinToCelsius(kelvin);
}

double c_feet_to_meters(double feet) {
    return Physics::PhysicsEngine::feetToMeters(feet);
}

double c_meters_to_feet(double meters) {
    return Physics::PhysicsEngine::metersToFeet(meters);
}

C_AerodynamicForces c_compute_aerodynamics(double velocity, double altitude, double angle_of_attack) {
    Physics::AtmosphericProperties atm = Physics::PhysicsEngine::atmosphereStandard(velocity, altitude);
    double q = Physics::PhysicsEngine::computeDynamicPressure(atm.density, velocity);
    
    Physics::AeroCoefficients coeffs;
    coeffs.Mach = atm.Mach_number;
    coeffs.alpha = angle_of_attack;
    coeffs.beta = 0.0;
    coeffs.Cl_alpha = 2.0 * M_PI;
    coeffs.Cd0 = 0.02;
    coeffs.k = 1.0;
    coeffs.e = 0.8;
    coeffs.AR = 8.0;
    
    Physics::AerodynamicForces forces;
    Physics::PhysicsEngine::computeLiftDrag(forces, q, 1.0, angle_of_attack, coeffs);
    
    C_AerodynamicForces result;
    result.lift = forces.lift;
    result.drag = forces.drag;
    result.side_force = forces.side_force;
    result.Cl = forces.Cl;
    result.Cd = forces.Cd;
    return result;
}

C_FlightResult c_compute_flight(double velocity, double altitude, double distance,
                                 double mass, double gravity, double drag_coeff, 
                                 double lift_coeff, double wing_area, double air_resistance) {
    Physics::FlightResult result = Physics::PhysicsEngine::computeFlight(
        velocity, altitude, distance, mass, gravity, drag_coeff, lift_coeff, wing_area, air_resistance
    );
    
    C_FlightResult c_result;
    c_result.distance_traveled = result.distance_traveled;
    c_result.time_of_flight = result.time_of_flight;
    c_result.final_velocity = result.final_velocity;
    c_result.max_altitude = result.max_altitude;
    c_result.drag_force = result.drag_force;
    c_result.lift_force = result.lift_force;
    c_result.kinetic_energy = result.kinetic_energy;
    c_result.potential_energy = result.potential_energy;
    return c_result;
}

double c_compute_drag_force(double velocity, double altitude, double drag_coeff, double area) {
    Physics::AtmosphericProperties atm = Physics::PhysicsEngine::atmosphereStandard(velocity, altitude);
    return Physics::PhysicsEngine::computeDragForce(velocity, atm.density, drag_coeff, area);
}

double c_compute_lift_force(double velocity, double altitude, double lift_coeff, double area) {
    Physics::AtmosphericProperties atm = Physics::PhysicsEngine::atmosphereStandard(velocity, altitude);
    return Physics::PhysicsEngine::computeLiftForce(velocity, atm.density, lift_coeff, area);
}

double c_compute_kinetic_energy(double mass, double velocity) {
    return Physics::PhysicsEngine::computeKineticEnergy(mass, velocity);
}

double c_compute_potential_energy(double mass, double altitude, double gravity) {
    return Physics::PhysicsEngine::computePotentialEnergy(mass, altitude, gravity);
}

int c_load_model(const char* filename) {
    g_mesh = std::make_unique<ModelLoader::Mesh>();
    if (!ModelLoader::ModelLoader::loadOBJ(std::string(filename), *g_mesh)) {
        g_mesh.reset();
        return 0;
    }
    g_model_props = ModelLoader::ModelLoader::computeProperties(*g_mesh);
    return 1;
}

C_ModelProperties c_get_model_properties() {
    C_ModelProperties props;
    props.loaded = g_mesh ? 1 : 0;
    props.vertex_count = g_model_props.vertex_count;
    props.face_count = g_model_props.face_count;
    props.surface_area = g_model_props.surface_area;
    props.volume = g_model_props.volume;
    props.bounding_box_x = g_model_props.bounding_box_x;
    props.bounding_box_y = g_model_props.bounding_box_y;
    props.bounding_box_z = g_model_props.bounding_box_z;
    props.planform_area = g_model_props.planform_area;
    props.aspect_ratio = g_model_props.aspect_ratio;
    props.fineness_ratio = g_model_props.fineness_ratio;
    props.wetted_area = g_model_props.wetted_area;
    props.drag_coefficient = g_model_drag;
    props.lift_coefficient = g_model_lift;
    return props;
}

void c_compute_model_aerodynamics(double velocity, double altitude, double angle_of_attack) {
    if (!g_mesh) return;
    g_model_drag = ModelLoader::ModelLoader::computeDragCoefficient(*g_mesh, velocity, altitude);
    g_model_lift = ModelLoader::ModelLoader::computeLiftCoefficient(*g_mesh, angle_of_attack);
}

void c_free_model() {
    g_mesh.reset();
}

}