#include "forces.h"

void computeLiftCoeff(double *Cl, double alpha, AeroCoefficients *coeffs) {
    double alpha_rad = alpha * M_PI / 180.0;
    double alpha0 = 0.0;
    double alpha_stall = 15.0 * M_PI / 180.0;
    
    if (fabs(alpha) < alpha_stall) {
        *Cl = coeffs->Cl_alpha * (alpha_rad - alpha0);
    } else {
        double Cl_max = coeffs->Cl_alpha * alpha_stall;
        double stall_factor = exp(-pow((alpha_rad - alpha_stall) / (5.0 * M_PI / 180.0), 2));
        *Cl = Cl_max * stall_factor;
    }
}

void computeDragCoeff(double *Cd, double alpha, AeroCoefficients *coeffs) {
    double alpha_rad = alpha * M_PI / 180.0;
    double Cd_induced = coeffs->k * pow(coeffs->Cl_alpha * alpha_rad, 2) / (M_PI * coeffs->e * coeffs->AR);
    *Cd = coeffs->Cd0 + Cd_induced;
}

void computeLiftDrag(AerodynamicForces *forces, double q, double S, double alpha, AeroCoefficients *coeffs) {
    computeLiftCoeff(&forces->Cl, alpha, coeffs);
    computeDragCoeff(&forces->Cd, alpha, coeffs);
    
    forces->lift = forces->Cl * q * S;
    forces->drag = forces->Cd * q * S;
    forces->side_force = 0.0;
    forces->moment_y = 0.0;
}

void computeForcesFromCoeffs(AerodynamicForces *forces, double q, double S) {
    forces->lift = forces->Cl * q * S;
    forces->drag = forces->Cd * q * S;
    forces->side_force = 0.0;
}

double computeDynamicPressure(double rho, double V) {
    return 0.5 * rho * V * V;
}
