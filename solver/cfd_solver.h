#ifndef CFD_SOLVER_H
#define CFD_SOLVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_GRID_X 512
#define MAX_GRID_Y 512
#define MAX_ITERATIONS 10000
#define CFL_NUMBER 0.4

typedef struct {
    double rho;
    double rho_u;
    double rho_v;
    double E;
} ConservativeVars;

typedef struct {
    double u;
    double v;
    double p;
    double T;
    double rho;
} PrimitiveVars;

typedef struct {
    int nx;
    int ny;
    double dx;
    double dy;
    double gamma;
    double Pr;
    double mu;
    double dt;
    int max_iter;
    double residual_tolerance;
    int boundary_type;
} SolverConfig;

typedef struct {
    ConservativeVars **Q;
    ConservativeVars **Q_new;
    double **x;
    double **y;
    int nx;
    int ny;
    double time;
} CFDMesh;

void cfd_solver_init(CFDMesh *mesh, int nx, int ny, double Lx, double Ly);
void cfd_solver_free(CFDMesh *mesh);
void cfd_set_initial_conditions(CFDMesh *mesh, double Mach, double alpha, double T_inf, double p_inf);
void cfd_compute_fluxes(CFDMesh *mesh, SolverConfig *config);
void cfd_time_step(CFDMesh *mesh, SolverConfig *config);
double cfd_compute_residual(CFDMesh *mesh, ConservativeVars **Q_old);
void cfd_apply_boundary_conditions(CFDMesh *mesh, SolverConfig *config);
int cfd_solve(CFDMesh *mesh, SolverConfig *config);
void cfd_get_primitive(CFDMesh *mesh, int i, int j, PrimitiveVars *prim);
void cfd_compute_forces(CFDMesh *mesh, SolverConfig *config, double *lift, double *drag);
void cfd_compute_heat_flux(CFDMesh *mesh, double *q_heat, double *T_wall);

#endif
