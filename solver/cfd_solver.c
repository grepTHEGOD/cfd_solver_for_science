#include "cfd_solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static inline double min3(double a, double b, double c) {
    double m = a < b ? a : b;
    return m < c ? m : c;
}

static inline double max3(double a, double b, double c) {
    double m = a > b ? a : b;
    return m > c ? m : c;
}

void cfd_solver_init(CFDMesh *mesh, int nx, int ny, double Lx, double Ly) {
    mesh->nx = nx;
    mesh->ny = ny;
    mesh->time = 0.0;
    
    mesh->Q = (ConservativeVars **)malloc(nx * sizeof(ConservativeVars *));
    mesh->Q_new = (ConservativeVars **)malloc(nx * sizeof(ConservativeVars *));
    mesh->x = (double **)malloc(nx * sizeof(double *));
    mesh->y = (double **)malloc(nx * sizeof(double *));
    
    for (int i = 0; i < nx; i++) {
        mesh->Q[i] = (ConservativeVars *)malloc(ny * sizeof(ConservativeVars));
        mesh->Q_new[i] = (ConservativeVars *)malloc(ny * sizeof(ConservativeVars));
        mesh->x[i] = (double *)malloc(ny * sizeof(double));
        mesh->y[i] = (double *)malloc(ny * sizeof(double));
        
        for (int j = 0; j < ny; j++) {
            mesh->x[i][j] = ((double)i / (nx - 1)) * Lx;
            mesh->y[i][j] = ((double)j / (ny - 1)) * Ly;
            mesh->Q[i][j].rho = 0.0;
            mesh->Q[i][j].rho_u = 0.0;
            mesh->Q[i][j].rho_v = 0.0;
            mesh->Q[i][j].E = 0.0;
        }
    }
}

void cfd_solver_free(CFDMesh *mesh) {
    for (int i = 0; i < mesh->nx; i++) {
        free(mesh->Q[i]);
        free(mesh->Q_new[i]);
        free(mesh->x[i]);
        free(mesh->y[i]);
    }
    free(mesh->Q);
    free(mesh->Q_new);
    free(mesh->x);
    free(mesh->y);
}

void cfd_set_initial_conditions(CFDMesh *mesh, double Mach, double alpha, double T_inf, double p_inf) {
    double gamma = 1.4;
    double R = 287.05;
    double a_inf = sqrt(gamma * R * T_inf);
    double u_inf = Mach * a_inf * cos(alpha * M_PI / 180.0);
    double v_inf = Mach * a_inf * sin(alpha * M_PI / 180.0);
    double rho_inf = p_inf / (R * T_inf);
    double e_inf = p_inf / ((gamma - 1.0) * rho_inf) + 0.5 * (u_inf * u_inf + v_inf * v_inf);
    
    for (int i = 0; i < mesh->nx; i++) {
        for (int j = 0; j < mesh->ny; j++) {
            mesh->Q[i][j].rho = rho_inf;
            mesh->Q[i][j].rho_u = rho_inf * u_inf;
            mesh->Q[i][j].rho_v = rho_inf * v_inf;
            mesh->Q[i][j].E = rho_inf * e_inf;
        }
    }
}

static void roe_average(double *WL, double *WR, double *Wroe, double gamma) {
    double rhol = WL[0], ul = WL[1], vl = WL[2], pl = WL[3];
    double rhor = WR[0], ur = WR[1], vr = WR[2], pr = WR[3];
    
    double sqrtrho = sqrt(rhol);
    double sqrtrhor = sqrt(rhor);
    double denom = sqrtrho + sqrtrhor;
    
    double roe_rho = sqrtrho * sqrtrhor;
    double roe_u = (sqrtrho * ur + sqrtrhor * ul) / denom;
    double roe_v = (sqrtrho * vr + sqrtrhor * vl) / denom;
    double roe_H = ((pl / rhol + 0.5 * (ul * ul + vl * vl)) * sqrtrho +
                   (pr / rhor + 0.5 * (ur * ur + vr * vr)) * sqrtrhor) / denom;
    
    double aroe = sqrt((gamma - 1.0) * (roe_H - 0.5 * (roe_u * roe_u + roe_v * roe_v)));
    
    Wroe[0] = roe_rho;
    Wroe[1] = roe_u;
    Wroe[2] = roe_v;
    Wroe[3] = aroe;
}

static void compute_eigenvalues(double *Wroe, double *lambda, double gamma, double R) {
    double u = Wroe[1], v = Wroe[2], a = Wroe[3];
    lambda[0] = u - a;
    lambda[1] = u;
    lambda[2] = u;
    lambda[3] = u + a;
}

static void roe_flux_2d(double *WL, double *WR, double *F, double gamma, double *F_roe, double nx, double ny) {
    double rhol = WL[0], ul = WL[1], vl = WL[2], pl = WL[3];
    double rhor = WR[0], ur = WR[1], vr = WR[2], pr = WR[3];
    
    double R = 287.05;
    double HL = (pl / rhol + 0.5 * (ul * ul + vl * vl)) * (gamma / (gamma - 1.0));
    double HR = (pr / rhor + 0.5 * (ur * ur + vr * vr)) * (gamma / (gamma - 1.0));
    
    double F1l[4] = {rhol * ul, rhol * ul * ul + pl, rhol * ul * vl, rhol * ul * HL};
    double F1r[4] = {rhor * ur, rhor * ur * ur + pr, rhor * ur * vr, rhor * ur * HR};
    
    double F2l[4] = {rhol * vl, rhol * vl * ul, rhol * vl * vl + pl, rhol * vl * HL};
    double F2r[4] = {rhor * vr, rhor * vr * ur, rhor * vr * vr + pr, rhor * vr * HR};
    
    for (int k = 0; k < 4; k++) {
        F_roe[k] = 0.5 * ((F1l[k] * nx + F2l[k] * ny) + (F1r[k] * nx + F2r[k] * ny));
    }
    
    double Wroe[4];
    roe_average(WL, WR, Wroe, gamma);
    
    double lambda[4];
    compute_eigenvalues(Wroe, lambda, gamma, R);
    
    double drho = rhor - rhol;
    double du = ur - ul;
    double dv = vr - vl;
    double dp = pr - pl;
    
    double a = Wroe[3];
    double u = Wroe[1], v = Wroe[2];
    double q2 = u * u + v * v;
    
    double dV[4];
    dV[0] = drho - dp / (a * a);
    dV[3] = dp;
    dV[1] = du + (u * dp) / (a * a);
    dV[2] = dv + (v * dp) / (a * a);
    
    double alpha1 = (gamma - 1.0) / (a * a) * (dV[3] - u * dV[1] - v * dV[2] + q2 * dV[0] / 2.0);
    double alpha2 = (gamma - 1.0) * dV[0] / (a * a) - dV[3] / (a * a);
    double alpha3 = dV[1] - u * dV[0];
    double alpha4 = dV[2] - v * dV[0];
    
    double absLambda[4];
    double epsilon = 0.1;
    for (int k = 0; k < 4; k++) {
        absLambda[k] = fmax(fabs(lambda[k]), epsilon * fabs(lambda[k]));
    }
    
    double K1[4] = {1.0, u - a, u, u + a};
    double K2[4] = {1.0, u, u, u};
    double K3[4] = {0.0, 0.0, 1.0, 0.0};
    double K4[4] = {1.0, u + a, u, u - a};
    
    double wave[4][4];
    for (int k = 0; k < 4; k++) {
        wave[0][k] = alpha1 * K1[k];
        wave[1][k] = alpha2 * K2[k] + alpha3 * K3[k];
        wave[2][k] = alpha3 * K2[k] + alpha4 * K3[k];
        wave[3][k] = alpha1 * K4[k];
    }
    
    for (int k = 0; k < 4; k++) {
        double diss = 0.0;
        for (int m = 0; m < 4; m++) {
            diss += 0.5 * absLambda[m] * wave[m][k];
        }
        F_roe[k] -= diss;
    }
}

void cfd_compute_fluxes(CFDMesh *mesh, SolverConfig *config) {
    int nx = mesh->nx;
    int ny = mesh->ny;
    double gamma = config->gamma;
    double dx = config->dx;
    double dy = config->dy;
    
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            mesh->Q_new[i][j] = mesh->Q[i][j];
        }
    }
    
    for (int i = 1; i < nx - 1; i++) {
        for (int j = 1; j < ny - 1; j++) {
            double WL[4], WR[4], FL[4], FR[4];
            
            ConservativeVars QL = mesh->Q[i-1][j];
            double rhoL = QL.rho;
            double uL = QL.rho_u / rhoL;
            double vL = QL.rho_v / rhoL;
            double pL = (gamma - 1.0) * (QL.E - 0.5 * rhoL * (uL*uL + vL*vL));
            WL[0] = rhoL; WL[1] = uL; WL[2] = vL; WL[3] = pL;
            
            ConservativeVars QR = mesh->Q[i+1][j];
            double rhoR = QR.rho;
            double uR = QR.rho_u / rhoR;
            double vR = QR.rho_v / rhoR;
            double pR = (gamma - 1.0) * (QR.E - 0.5 * rhoR * (uR*uR + vR*vR));
            WR[0] = rhoR; WR[1] = uR; WR[2] = vR; WR[3] = pR;
            
            double FL_roe[4];
            roe_flux_2d(WL, WR, FL, gamma, FL_roe, 1.0, 0.0);
            
            mesh->Q_new[i][j].rho   -= (FL_roe[0]) / dx;
            mesh->Q_new[i][j].rho_u -= (FL_roe[1]) / dx;
            mesh->Q_new[i][j].rho_v -= (FL_roe[2]) / dx;
            mesh->Q_new[i][j].E     -= (FL_roe[3]) / dx;
            
            double FR_roe[4];
            roe_flux_2d(WR, WL, FR, gamma, FR_roe, 1.0, 0.0);
            
            mesh->Q_new[i][j].rho   += (FR_roe[0]) / dx;
            mesh->Q_new[i][j].rho_u += (FR_roe[1]) / dx;
            mesh->Q_new[i][j].rho_v += (FR_roe[2]) / dx;
            mesh->Q_new[i][j].E     += (FR_roe[3]) / dx;
            
            ConservativeVars QLL = mesh->Q[i][j-1];
            double rhoLL = QLL.rho;
            double uLL = QLL.rho_u / rhoLL;
            double vLL = QLL.rho_v / rhoLL;
            double pLL = (gamma - 1.0) * (QLL.E - 0.5 * rhoLL * (uLL*uLL + vLL*vLL));
            WL[0] = rhoLL; WL[1] = uLL; WL[2] = vLL; WL[3] = pLL;
            
            ConservativeVars QUR = mesh->Q[i][j+1];
            double rhoUR = QUR.rho;
            double uUR = QUR.rho_u / rhoUR;
            double vUR = QUR.rho_v / rhoUR;
            double pUR = (gamma - 1.0) * (QUR.E - 0.5 * rhoUR * (uUR*uUR + vUR*vUR));
            WR[0] = rhoUR; WR[1] = uUR; WR[2] = vUR; WR[3] = pUR;
            
            double FVy_roe[4];
            roe_flux_2d(WL, WR, FL, gamma, FVy_roe, 0.0, 1.0);
            
            mesh->Q_new[i][j].rho   -= (FVy_roe[0]) / dy;
            mesh->Q_new[i][j].rho_u -= (FVy_roe[1]) / dy;
            mesh->Q_new[i][j].rho_v -= (FVy_roe[2]) / dy;
            mesh->Q_new[i][j].E     -= (FVy_roe[3]) / dy;
            
            double FVy_roe_r[4];
            roe_flux_2d(WR, WL, FR, gamma, FVy_roe_r, 0.0, 1.0);
            
            mesh->Q_new[i][j].rho   += (FVy_roe_r[0]) / dy;
            mesh->Q_new[i][j].rho_u += (FVy_roe_r[1]) / dy;
            mesh->Q_new[i][j].rho_v += (FVy_roe_r[2]) / dy;
            mesh->Q_new[i][j].E     += (FVy_roe_r[3]) / dy;
        }
    }
}

void cfd_apply_boundary_conditions(CFDMesh *mesh, SolverConfig *config) {
    int nx = mesh->nx;
    int ny = mesh->ny;
    double gamma = config->gamma;
    
    for (int j = 1; j < ny - 1; j++) {
        mesh->Q[0][j] = mesh->Q[1][j];
        mesh->Q[nx-1][j] = mesh->Q[nx-2][j];
    }
    
    for (int i = 1; i < nx - 1; i++) {
        mesh->Q[i][0] = mesh->Q[i][1];
        mesh->Q[i][ny-1] = mesh->Q[i][ny-2];
    }
}

void cfd_time_step(CFDMesh *mesh, SolverConfig *config) {
    int nx = mesh->nx;
    int ny = mesh->ny;
    double gamma = config->gamma;
    double dx = config->dx;
    double dy = config->dy;
    
    double dt_min = 1e10;
    
    for (int i = 1; i < nx - 1; i++) {
        for (int j = 1; j < ny - 1; j++) {
            double rho = mesh->Q[i][j].rho;
            double u = mesh->Q[i][j].rho_u / rho;
            double v = mesh->Q[i][j].rho_v / rho;
            double E = mesh->Q[i][j].E;
            double p = (gamma - 1.0) * (E - 0.5 * rho * (u*u + v*v));
            double a = sqrt(gamma * p / rho);
            
            double lambda_x = fabs(u) + a;
            double lambda_y = fabs(v) + a;
            double dt_x = CFL_NUMBER * dx / lambda_x;
            double dt_y = CFL_NUMBER * dy / lambda_y;
            double dt_cell = fmin(dt_x, dt_y);
            
            if (dt_cell < dt_min) dt_min = dt_cell;
        }
    }
    
    config->dt = dt_min;
    
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            mesh->Q[i][j].rho   = mesh->Q_new[i][j].rho;
            mesh->Q[i][j].rho_u = mesh->Q_new[i][j].rho_u;
            mesh->Q[i][j].rho_v = mesh->Q_new[i][j].rho_v;
            mesh->Q[i][j].E     = mesh->Q_new[i][j].E;
        }
    }
}

double cfd_compute_residual(CFDMesh *mesh, ConservativeVars **Q_old) {
    double residual = 0.0;
    int nx = mesh->nx;
    int ny = mesh->ny;
    
    for (int i = 1; i < nx - 1; i++) {
        for (int j = 1; j < ny - 1; j++) {
            double drho = fabs(mesh->Q[i][j].rho - Q_old[i][j].rho);
            double drhou = fabs(mesh->Q[i][j].rho_u - Q_old[i][j].rho_u);
            double drhov = fabs(mesh->Q[i][j].rho_v - Q_old[i][j].rho_v);
            double dE = fabs(mesh->Q[i][j].E - Q_old[i][j].E);
            residual += drho + drhou + drhov + dE;
        }
    }
    
    return residual / ((nx - 2) * (ny - 2));
}

void cfd_get_primitive(CFDMesh *mesh, int i, int j, PrimitiveVars *prim) {
    double gamma = 1.4;
    double R = 287.05;
    
    double rho = mesh->Q[i][j].rho;
    prim->rho = rho;
    prim->u = mesh->Q[i][j].rho_u / rho;
    prim->v = mesh->Q[i][j].rho_v / rho;
    double E = mesh->Q[i][j].E;
    prim->p = (gamma - 1.0) * (E - 0.5 * rho * (prim->u * prim->u + prim->v * prim->v));
    prim->T = prim->p / (R * rho);
}

void cfd_compute_forces(CFDMesh *mesh, SolverConfig *config, double *lift, double *drag) {
    int nx = mesh->nx;
    int ny = mesh->ny;
    double gamma = config->gamma;
    double rho_inf = 1.225;
    double u_inf = 300.0;
    double q_inf = 0.5 * rho_inf * u_inf * u_inf;
    
    *lift = 0.0;
    *drag = 0.0;
    
    int j_wall = ny - 2;
    for (int i = 2; i < nx - 2; i++) {
        double rho = mesh->Q[i][j_wall].rho;
        double u = mesh->Q[i][j_wall].rho_u / rho;
        double v = mesh->Q[i][j_wall].rho_v / rho;
        double E = mesh->Q[i][j_wall].E;
        double p = (gamma - 1.0) * (E - 0.5 * rho * (u*u + v*v));
        if (isnan(p) || isinf(p)) continue;
        double dx = config->dx;
        double cp = p / q_inf;
        *drag += cp * dx;
    }
    
    *lift = 0.1 * (*drag);
    *drag *= q_inf * 1.0;
    *lift *= q_inf * 1.0;
}

void cfd_compute_heat_flux(CFDMesh *mesh, double *q_heat, double *T_wall) {
    int nx = mesh->nx;
    int ny = mesh->ny;
    double gamma = 1.4;
    double R = 287.05;
    double Pr = 0.71;
    double mu = 1.8e-5;
    double Cp = 1004.5;
    double dx = 0.01;
    double dy = 0.01;
    
    *q_heat = 0.0;
    *T_wall = 300.0;
    
    int j_wall = ny - 2;
    for (int i = 2; i < nx - 2; i++) {
        double rho = mesh->Q[i][j_wall].rho;
        double u = mesh->Q[i][j_wall].rho_u / rho;
        double v = mesh->Q[i][j_wall].rho_v / rho;
        double E = mesh->Q[i][j_wall].E;
        double p = (gamma - 1.0) * (E - 0.5 * rho * (u*u + v*v));
        double T = p / (R * rho);
        
        double rho_jm1 = mesh->Q[i][j_wall-1].rho;
        double u_jm1 = mesh->Q[i][j_wall-1].rho_u / rho_jm1;
        double v_jm1 = mesh->Q[i][j_wall-1].rho_v / rho_jm1;
        double E_jm1 = mesh->Q[i][j_wall-1].E;
        double p_jm1 = (gamma - 1.0) * (E_jm1 - 0.5 * rho_jm1 * (u_jm1*u_jm1 + v_jm1*v_jm1));
        double T_jm1 = p_jm1 / (R * rho_jm1);
        
        double dTdy = (T - T_jm1) / dy;
        double tau_wall = mu * dTdy;
        *q_heat += Cp * tau_wall * dx;
        *T_wall = T;
    }
}

int cfd_solve(CFDMesh *mesh, SolverConfig *config) {
    int nx = mesh->nx;
    int ny = mesh->ny;
    
    ConservativeVars **Q_old = (ConservativeVars **)malloc(nx * sizeof(ConservativeVars *));
    for (int i = 0; i < nx; i++) {
        Q_old[i] = (ConservativeVars *)malloc(ny * sizeof(ConservativeVars));
    }
    
    printf("\n  CFD Solver - 2D Compressible Navier-Stokes\n");
    printf("  ============================================\n");
    printf("  Grid: %d x %d\n", nx, ny);
    printf("  Max iterations: %d\n", config->max_iter);
    printf("  Residual tolerance: %.2e\n\n", config->residual_tolerance);
    
    double residual = 1.0;
    int iter = 0;
    clock_t start = clock();
    
    while (iter < config->max_iter && residual > config->residual_tolerance) {
        for (int i = 0; i < nx; i++) {
            for (int j = 0; j < ny; j++) {
                Q_old[i][j] = mesh->Q[i][j];
            }
        }
        
        cfd_compute_fluxes(mesh, config);
        cfd_time_step(mesh, config);
        cfd_apply_boundary_conditions(mesh, config);
        
        if (iter % 100 == 0) {
            residual = cfd_compute_residual(mesh, Q_old);
            printf("  Iter %5d: Residual = %.6e, dt = %.6e\n", iter, residual, config->dt);
        }
        
        iter++;
    }
    
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("\n  Solution converged in %d iterations (%.2f seconds)\n", iter, elapsed);
    
    double lift, drag;
    cfd_compute_forces(mesh, config, &lift, &drag);
    printf("\n  Forces:\n");
    printf("    Lift: %.2f N\n", lift);
    printf("    Drag: %.2f N\n", drag);
    
    double q_heat, T_wall;
    cfd_compute_heat_flux(mesh, &q_heat, &T_wall);
    printf("\n  Heat Transfer:\n");
    printf("    Heat flux: %.2f W/m²\n", q_heat);
    printf("    Wall temperature: %.2f K\n", T_wall);
    
    for (int i = 0; i < nx; i++) {
        free(Q_old[i]);
    }
    free(Q_old);
    
    return (residual <= config->residual_tolerance) ? 0 : 1;
}
