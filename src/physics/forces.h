#ifndef FORCES_H
#define FORCES_H

#include <math.h>

typedef struct {
    double lift;
    double drag;
    double side_force;
    double lift_coefficient;
    double drag_coefficient;
    double side_coefficient;
    double pitch_moment;
    double roll_moment;
    double yaw_moment;
    double cl_alpha;
    double cd0;
    double k;
    double oswald;
} ForceResult;

typedef struct {
    double velocity;
    double density;
    double area;
    double chord;
    double span;
    double aoa;
    double mach;
    double reynolds;
    double cl;
    double cd;
    double cm;
} AerodynamicForces;

void forces_compute(AerodynamicForces *af, ForceResult *result);
double forces_get_lift_coefficient(double aoa, double cl_alpha, double cd0, double k);
double forces_get_drag_coefficient(double aoa, double cd0, double k, double cl);
double forces_get_moment_coefficient(double aoa, double cm_alpha);
double forces_get_dynamic_pressure(double density, double velocity);
double forces_get_lift(double q, double area, double cl);
double forces_get_drag(double q, double area, double cd);

#endif
