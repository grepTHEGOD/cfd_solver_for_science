#ifndef PHYSICS_FORCES_H
#define PHYSICS_FORCES_H

#include <math.h>

typedef struct {
    double lift;
    double drag;
    double side_force;
    double moment_x;
    double moment_y;
    double moment_z;
    double Cl;
    double Cd;
    double Cmy;
} AerodynamicForces;

typedef struct {
    double Mach;
    double alpha;
    double beta;
    double Cl_alpha;
    double Cd0;
    double k;
    double e;
    double AR;
} AeroCoefficients;

void computeLiftDrag(AerodynamicForces *forces, double q, double S, double alpha, AeroCoefficients *coeffs);
void computeForcesFromCoeffs(AerodynamicForces *forces, double q, double S);
void computeLiftCoeff(double *Cl, double alpha, AeroCoefficients *coeffs);
void computeDragCoeff(double *Cd, double alpha, AeroCoefficients *coeffs);
double computeDynamicPressure(double rho, double V);

#endif
