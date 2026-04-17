#ifndef PHYSICS_BRIDGE_H
#define PHYSICS_BRIDGE_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    double altitude;
    double temperature;
    double pressure;
    double density;
    double speed_of_sound;
    double dynamic_viscosity;
    double kinematic_viscosity;
    double Reynolds_number;
    double Mach_number;
} C_AtmosphericProperties;

typedef struct {
    double lift;
    double drag;
    double side_force;
    double Cl;
    double Cd;
} C_AerodynamicForces;

typedef struct {
    double velocity;
    double altitude;
    double distance;
    double air_resistance;
    double mass;
    double gravity;
    double wing_area;
    double drag_coefficient;
    double lift_coefficient;
    int use_feet;
} C_PhysicsParams;

typedef struct {
    double distance_traveled;
    double time_of_flight;
    double final_velocity;
    double max_altitude;
    double drag_force;
    double lift_force;
    double kinetic_energy;
    double potential_energy;
} C_FlightResult;

C_AtmosphericProperties c_atmosphere_isa(double altitude_m);
C_AtmosphericProperties c_atmosphere_standard(double V, double altitude_m);
double c_celsius_to_kelvin(double celsius);
double c_kelvin_to_celsius(double kelvin);
double c_feet_to_meters(double feet);
double c_meters_to_feet(double meters);

C_AerodynamicForces c_compute_aerodynamics(double velocity, double altitude, double angle_of_attack);
C_FlightResult c_compute_flight(double velocity, double altitude, double distance,
                                 double mass, double gravity, double drag_coeff, 
                                 double lift_coeff, double wing_area, double air_resistance);

double c_compute_drag_force(double velocity, double altitude, double drag_coeff, double area);
double c_compute_lift_force(double velocity, double altitude, double lift_coeff, double area);
double c_compute_kinetic_energy(double mass, double velocity);
double c_compute_potential_energy(double mass, double altitude, double gravity);

typedef struct {
    int loaded;
    int vertex_count;
    int face_count;
    double surface_area;
    double volume;
    double bounding_box_x;
    double bounding_box_y;
    double bounding_box_z;
    double planform_area;
    double aspect_ratio;
    double fineness_ratio;
    double wetted_area;
    double drag_coefficient;
    double lift_coefficient;
} C_ModelProperties;

int c_load_model(const char* filename);
C_ModelProperties c_get_model_properties();
void c_compute_model_aerodynamics(double velocity, double altitude, double angle_of_attack);
void c_free_model();

#ifdef __cplusplus
}
#endif

#endif