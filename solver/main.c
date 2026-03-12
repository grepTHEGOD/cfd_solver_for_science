#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "cfd_solver.h"

#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"

void print_banner() {
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════════════╗\n");
    printf("  ║          CFD SOLVER FOR SCIENCE - VERSION 2.0                ║\n");
    printf("  ║     2D Compressible Navier-Stokes Flow Solver                ║\n");
    printf("  ╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

void print_menu() {
    printf("  Available test cases:\n");
    printf("  ─────────────────────────────────────────────────\n");
    printf("  1.  Subsonic flow over flat plate (M=0.3, α=0°)\n");
    printf("  2.  Transonic flow (M=0.8, α=0°)\n");
    printf("  3.  Supersonic flow (M=2.0, α=0°)\n");
    printf("  4.  Supersonic flow with angle of attack (M=2.0, α=5°)\n");
    printf("  5.  Hypersonic flow (M=5.0, α=10°)\n");
    printf("  6.  Custom parameters\n");
    printf("  0.  Exit\n");
    printf("  ─────────────────────────────────────────────────\n\n");
}

void run_simulation(int nx, int ny, double Mach, double alpha, double T_inf, double p_inf, int max_iter) {
    CFDMesh mesh;
    SolverConfig config;
    
    double Lx = 1.0;
    double Ly = 1.0;
    
    config.nx = nx;
    config.ny = ny;
    config.dx = Lx / (nx - 1);
    config.dy = Ly / (ny - 1);
    config.gamma = 1.4;
    config.Pr = 0.71;
    config.mu = 1.8e-5;
    config.dt = 1e-6;
    config.max_iter = max_iter;
    config.residual_tolerance = 1e-6;
    config.boundary_type = 0;
    
    cfd_solver_init(&mesh, nx, ny, Lx, Ly);
    
    printf("\n  Initial conditions:\n");
    printf("    Mach number: %.2f\n", Mach);
    printf("    Angle of attack: %.2f°\n", alpha);
    printf("    Freestream temperature: %.2f K\n", T_inf);
    printf("    Freestream pressure: %.0f Pa\n", p_inf);
    
    cfd_set_initial_conditions(&mesh, Mach, alpha, T_inf, p_inf);
    
    int result = cfd_solve(&mesh, &config);
    
    if (result == 0) {
        printf("\n  " GREEN "✓ Simulation completed successfully!" RESET "\n");
    } else {
        printf("\n  " RED "✗ Simulation did not converge" RESET "\n");
    }
    
    cfd_solver_free(&mesh);
}

int main(int argc, char *argv[]) {
    int choice;
    int nx = 100, ny = 100;
    int max_iter = 5000;
    
    double Mach = 0.3;
    double alpha = 0.0;
    double T_inf = 300.0;
    double p_inf = 101325.0;
    
    print_banner();
    
    if (argc > 1) {
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            printf("  Usage: %s [options]\n\n", argv[0]);
            printf("  Options:\n");
            printf("    --mesh N          Grid size NxN (default: 100)\n");
            printf("    --iter N          Maximum iterations (default: 5000)\n");
            printf("    --mach M          Mach number\n");
            printf("    --alpha A         Angle of attack (degrees)\n");
            printf("    --temp T          Freestream temperature (K)\n");
            printf("    --pressure P      Freestream pressure (Pa)\n");
            printf("    --help, -h        Show this help\n");
            return 0;
        }
        
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "--mesh") == 0 && i + 1 < argc) {
                nx = atoi(argv[++i]);
                ny = nx;
            } else if (strcmp(argv[i], "--iter") == 0 && i + 1 < argc) {
                max_iter = atoi(argv[++i]);
            } else if (strcmp(argv[i], "--mach") == 0 && i + 1 < argc) {
                Mach = atof(argv[++i]);
            } else if (strcmp(argv[i], "--alpha") == 0 && i + 1 < argc) {
                alpha = atof(argv[++i]);
            } else if (strcmp(argv[i], "--temp") == 0 && i + 1 < argc) {
                T_inf = atof(argv[++i]);
            } else if (strcmp(argv[i], "--pressure") == 0 && i + 1 < argc) {
                p_inf = atof(argv[++i]);
            }
        }
        
        run_simulation(nx, ny, Mach, alpha, T_inf, p_inf, max_iter);
        return 0;
    }
    
    while (1) {
        print_menu();
        printf("  Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("  Invalid input.\n");
            return 1;
        }
        
        switch (choice) {
            case 1:
                printf("\n  Running: Subsonic flow over flat plate\n");
                run_simulation(nx, ny, 0.3, 0.0, 300.0, 101325.0, max_iter);
                break;
            case 2:
                printf("\n  Running: Transonic flow\n");
                run_simulation(nx, ny, 0.8, 0.0, 300.0, 101325.0, max_iter);
                break;
            case 3:
                printf("\n  Running: Supersonic flow\n");
                run_simulation(nx, ny, 2.0, 0.0, 300.0, 101325.0, max_iter);
                break;
            case 4:
                printf("\n  Running: Supersonic flow with AoA\n");
                run_simulation(nx, ny, 2.0, 5.0, 300.0, 101325.0, max_iter);
                break;
            case 5:
                printf("\n  Running: Hypersonic flow\n");
                run_simulation(nx, ny, 5.0, 10.0, 300.0, 101325.0, max_iter);
                break;
            case 6:
                printf("\n  Custom parameters:\n");
                printf("    Grid size (NxN): ");
                scanf("%d", &nx);
                ny = nx;
                printf("    Mach number: ");
                scanf("%lf", &Mach);
                printf("    Angle of attack (deg): ");
                scanf("%lf", &alpha);
                printf("    Freestream temperature (K): ");
                scanf("%lf", &T_inf);
                printf("    Freestream pressure (Pa): ");
                scanf("%lf", &p_inf);
                run_simulation(nx, ny, Mach, alpha, T_inf, p_inf, max_iter);
                break;
            case 0:
                printf("\n  Thank you for using CFD Solver!\n\n");
                return 0;
            default:
                printf("  Invalid choice. Please try again.\n");
        }
    }
    
    return 0;
}
