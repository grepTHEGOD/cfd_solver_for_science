#ifndef FORCES_C
#define FORCES_C

#include "forces.h"
#include <math.h>

void forces_compute(AerodynamicForces *af, ForceResult *result) {
    if (!af || !result) return;
    
    double aoa = af->aoa;
    double cl_alpha = 2.0 * 3.14159 * af->span / (af->chord * 2.0);
    double cd0 = 0.02;
    double k = 0.04;
    double oswald = 0.8;
    
    result->cl_alpha = cl_alpha;
    result->cd0 = cd0;
    result->k = k;
    result->oswald = oswald;
    
    double cl = forces_get_lift_coefficient(aoa, cl_alpha, cd0, k);
    double cd = forces_get_drag_coefficient(aoa, cd0, k, cl);
    double cm = forces_get_moment_coefficient(aoa, -0.5);
    
    af->cl = cl;
    af->cd = cd;
    af->cm = cm;
    
    result->lift_coefficient = cl;
    result->drag_coefficient = cd;
    result->pitch_moment = cm;
    
    result->lift = 0.0;
    result->drag = 0.0;
    result->side_force = 0.0;
    result->side_coefficient = 0.0;
    result->roll_moment = 0.0;
    result->yaw_moment = 0.0;
}

double forces_get_lift_coefficient(double aoa, double cl_alpha, double cd0, double k) {
    double aoa_rad = aoa * 3.14159 / 180.0;
    double cl = cl_alpha * aoa_rad;
    
    if (aoa > 15.0) {
        cl = cl * cos(aoa_rad * 0.5);
    }
    
    return cl;
}

double forces_get_drag_coefficient(double aoa, double cd0, double k, double cl) {
    double cd = cd0 + k * cl * cl;
    return cd;
}

double forces_get_moment_coefficient(double aoa, double cm_alpha) {
    return cm_alpha * aoa * 3.14159 / 180.0;
}

double forces_get_dynamic_pressure(double density, double velocity) {
    return 0.5 * density * velocity * velocity;
}

double forces_get_lift(double q, double area, double cl) {
    return q * area * cl;
}

double forces_get_drag(double q, double area, double cd) {
    return q * area * cd;
}

#endif
