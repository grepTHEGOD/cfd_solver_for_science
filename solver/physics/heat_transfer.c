#include "heat_transfer.h"

double computeRecoveryFactor(double Pr, double Mach) {
    if (Pr > 0.9) {
        return sqrt(Pr);
    } else {
        return pow(Pr, 1.0/3.0);
    }
}

void computeStagnationTemperature(HeatTransferData *ht, FlowProperties *flow) {
    double r = computeRecoveryFactor(flow->Pr, flow->Mach);
    ht->stagnation_temp = flow->T_inf * (1.0 + (flow->gamma - 1.0) / 2.0 * flow->Mach * flow->Mach);
    ht->recovery_factor = r;
}

void computeConvectiveCoeff(HeatTransferData *ht, FlowProperties *flow) {
    double a = sqrt(flow->gamma * flow->Cp * flow->T_inf);
    double Re = flow->rho_inf * flow->Mach * a * flow->L / flow->mu;
    
    double Nu;
    if (Re < 5e4) {
        Nu = 0.332 * pow(Re, 0.5) * pow(flow->Pr, 1.0/3.0);
    } else if (Re < 1e5) {
        Nu = 0.0296 * pow(Re, 0.8) * pow(flow->Pr, 1.0/3.0);
    } else {
        Nu = 0.037 * pow(Re, 0.8) * pow(flow->Pr, 1.0/3.0);
    }
    
    ht->h_convective = Nu * flow->k / flow->L;
}

void computeHeatTransfer(HeatTransferData *ht, FlowProperties *flow, double T_wall) {
    flow->k = flow->mu * flow->Cp / flow->Pr;
    
    computeStagnationTemperature(ht, flow);
    computeConvectiveCoeff(ht, flow);
    
    ht->wall_temp = T_wall;
    ht->heat_flux = ht->h_convective * (ht->stagnation_temp - T_wall);
}
