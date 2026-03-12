#ifndef PHYSICS_HEAT_TRANSFER_H
#define PHYSICS_HEAT_TRANSFER_H

#include <math.h>

#define SIGMA 5.670374419e-8

typedef struct {
    double heat_flux;
    double wall_temp;
    double stagnation_temp;
    double recovery_factor;
    double h_convective;
} HeatTransferData;

typedef struct {
    double Mach;
    double T_inf;
    double p_inf;
    double rho_inf;
    double Pr;
    double mu;
    double Cp;
    double gamma;
    double L;
    double k;
} FlowProperties;

void computeStagnationTemperature(HeatTransferData *ht, FlowProperties *flow);
void computeHeatFlux(HeatTransferData *ht, FlowProperties *flow, double T_wall);
void computeConvectiveCoeff(HeatTransferData *ht, FlowProperties *flow);
double computeRecoveryFactor(double Pr, double Mach);
void computeHeatTransfer(HeatTransferData *ht, FlowProperties *flow, double T_wall);

#endif
