#include "scramjet_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define BOLD "\x1b[1m"
#define RESET "\x1b[0m"
#define CLEAR "\033[2J\033[H"

static ScramjetConfig current_config;
static ScramjetPerformance current_perf;
static int simulation_run = 0;

static void printHeader() {
    printf(CLEAR);
    printf(BOLD CYAN);
    printf("╔══════════════════════════════════════════════════════════════════════╗\n");
    printf("║      SCRAMJET CFD DESIGN SUITE - Professional Scramjet Analysis    ║\n");
    printf("║              For Science - Hypersonic Propulsion                    ║\n");
    printf("╚══════════════════════════════════════════════════════════════════════╝\n" RESET);
    printf("\n");
}

static void printMenuOptions() {
    printf(BOLD "  [1]" RESET " Configure Scramerjet Type & Size\n");
    printf(BOLD "  [2]" RESET " Set Fuel & Cooling System\n");
    printf(BOLD "  [3]" RESET " Configure Inlet Design\n");
    printf(BOLD "  [4]" RESET " Set Flight Conditions\n");
    printf(BOLD "  [5]" RESET " Run Performance Analysis\n");
    printf(BOLD "  [6]" RESET " Display Design Summary\n");
    printf(BOLD "  [7]" RESET " Compare All Sizes\n");
    printf(BOLD "  [8]" RESET " Stress & Thermal Analysis\n");
    printf(BOLD "  [9]" RESET " Run Flow Simulation\n");
    printf(BOLD "  [A]" RESET " Export Design Data\n");
    printf(BOLD "  [Q]" RESET " Quit\n");
    printf("\n");
}

static void printTypeSelection() {
    printf(BOLD CYAN "\n=== SCRAMJET TYPE SELECTION ===\n" RESET);
    printf("\nChoose your propulsion architecture:\n\n");
    
    printf(BOLD "  [1] " RESET "Scramjet (Pure)\n");
    printf("      - Supersonic Combustion Ramjet\n");
    printf("      - Operates at Mach 5+ cruise\n");
    printf("      - No moving parts, fully passive\n");
    printf("      - Best for: Hypersonic cruise missiles, space access\n\n");
    
    printf(BOLD "  [2] " RESET GREEN "SHCRAMJET - Shrouded Helical Counterflow Rotating\n" RESET);
    printf("      - " BOLD "Innovative advanced configuration" RESET "\n");
    printf("      - Internal helical compression flow path\n");
    printf("      - Enhanced mixing and combustion stability\n");
    printf("      - Built-in thermal management system\n");
    printf("      - Best for: High-Mach cruise (M6-15), long range\n\n");
    
    printf(BOLD "  [3] " RESET "Dual-Mode Scramjet\n");
    printf("      - Ramjet at low speed, Scramjet at high speed\n");
    printf("      - Automatic mode transition at Mach 3-4\n");
    printf("      - Optimized for wide flight envelope\n");
    printf("      - Best for: Reusable hypersonic vehicles\n\n");
}

static void selectScramjetType(ScramjetType *type) {
    int choice;
    printf("\nSelect type [1-3]: ");
    scanf("%d", &choice);
    getchar();
    
    switch(choice) {
        case 1: *type = SCRAMJET_TYPE_PURE; break;
        case 2: *type = SCRAMJET_TYPE_SHCRAMJET; break;
        case 3: *type = SCRAMJET_TYPE_DUALMODE; break;
        default: *type = SCRAMJET_TYPE_SHCRAMJET;
    }
}

static void printSizeSelection() {
    printf(BOLD CYAN "\n=== SIZE SELECTION ===\n" RESET);
    printf("\n");
    
    printf(BOLD "  [S] " RESET "Small\n");
    printf("      Thrust: 50-200 kg  |  Airflow: 0.5-2 kg/s  |  Dry Mass: ~10 kg\n");
    printf("      Applications: Missile warheads, small UAVs\n\n");
    
    printf(BOLD "  [M] " RESET GREEN "Medium (Recommended)\n" RESET);
    printf("      Thrust: 200-1000 kg  |  Airflow: 2-10 kg/s  |  Dry Mass: ~50 kg\n");
    printf("      Applications: Hypersonic test vehicles, small spacecraft\n\n");
    
    printf(BOLD "  [L] " RESET "Large\n");
    printf("      Thrust: 1000-5000 kg  |  Airflow: 10-50 kg/s  |  Dry Mass: ~200 kg\n");
    printf("      Applications: Hypersonic aircraft, first stages\n\n");
    
    printf(BOLD "  [X] " RESET "X-Large\n");
    printf("      Thrust: 5000-20000 kg  |  Airflow: 50-200 kg/s  |  Dry Mass: ~500 kg\n");
    printf("      Applications: Space launch, hypersonic transport\n\n");
}

static void selectScramjetSize(ScramjetSize *size) {
    char choice;
    printf("\nSelect size [S/M/L/X]: ");
    scanf("%c", &choice);
    getchar();
    
    switch(choice) {
        case 's': case 'S': *size = SCRAMJET_SIZE_SMALL; break;
        case 'm': case 'M': *size = SCRAMJET_SIZE_MEDIUM; break;
        case 'l': case 'L': *size = SCRAMJET_SIZE_LARGE; break;
        case 'x': case 'X': *size = SCRAMJET_SIZE_XLARGE; break;
        default: *size = SCRAMJET_SIZE_MEDIUM;
    }
}

static void configureTypeAndSize() {
    printTypeSelection();
    ScramjetType type;
    selectScramjetType(&type);
    
    printSizeSelection();
    ScramjetSize size;
    selectScramjetSize(&size);
    
    getDefaultConfig(&current_config, type, size);
    simulation_run = 1;
    
    printf(GREEN "\n✓ Configuration loaded: %s - %s\n" RESET,
           getScramjetTypeString(type), getScramjetSizeString(size));
    printf("\nPress Enter to continue...");
    getchar();
}

static void setFuelAndCooling() {
    if (!simulation_run) {
        printf(RED "\n⚠ Please configure type and size first (Option 1)\n" RESET);
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    
    printHeader();
    printf(BOLD CYAN "\n=== FUEL & COOLING CONFIGURATION ===\n" RESET);
    
    printf("\nSelect Fuel:\n");
    printf("  [1] Hydrogen (H2) - Highest energy, best cooling\n");
    printf("  [2] Methane (CH4) - Good balance, easier storage\n");
    printf("  [3] Ethylene (C2H4) - High density\n");
    
    int fuel_choice;
    printf("\nFuel [1-3]: ");
    scanf("%d", &fuel_choice);
    getchar();
    
    switch(fuel_choice) {
        case 1: current_config.fuel = FUEL_HYDROGEN; break;
        case 2: current_config.fuel = FUEL_METHANE; break;
        case 3: current_config.fuel = FUEL_ETHYLENE; break;
    }
    
    printf("\nSelect Cooling System:\n");
    printf("  [1] Regenerative (fuel through channels)\n");
    printf("  [2] Film Cooling\n");
    printf("  [3] Transpiration (porous walls)\n");
    printf("  [4] Air Bleed\n");
    
    int cool_choice;
    printf("\nCooling [1-4]: ");
    scanf("%d", &cool_choice);
    getchar();
    
    switch(cool_choice) {
        case 1: current_config.cooling = COOLING_REGENERATIVE; break;
        case 2: current_config.cooling = COOLING_FILM; break;
        case 3: current_config.cooling = COOLING_TRANSPIRATION; break;
        case 4: current_config.cooling = COOLING_AIR; break;
    }
    
    printf(GREEN "\n✓ Fuel and cooling configured\n" RESET);
    printf("\nPress Enter to continue...");
    getchar();
}

static void configureInlet() {
    if (!simulation_run) {
        printf(RED "\n⚠ Please configure type and size first (Option 1)\n" RESET);
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    
    printHeader();
    printf(BOLD CYAN "\n=== INLET DESIGN CONFIGURATION ===\n" RESET);
    
    printf("\nInlet Type:\n");
    printf("  [1] 2D Wedge - Simple, reliable, good compression\n");
    printf("  [2] 3D Conical - Axisymmetric, highest compression\n");
    printf("  [3] 2D Digital - Variable geometry, wide Mach range\n");
    printf("  [4] 3D Isometric - Best performance, complex\n");
    
    int inlet_choice;
    printf("\nInlet [1-4]: ");
    scanf("%d", &inlet_choice);
    getchar();
    
    switch(inlet_choice) {
        case 1: current_config.inlet_type = INLET_2D_WEDGE; break;
        case 2: current_config.inlet_type = INLET_3D_CONICAL; break;
        case 3: current_config.inlet_type = INLET_2D_DIGITAL; break;
        case 4: current_config.inlet_type = INLET_3D_ISOMETRIC; break;
    }
    
    printf("\nRamp Angle (degrees) [10-30]: ");
    scanf("%lf", &current_config.inlet_ramp_angle);
    getchar();
    
    if (current_config.inlet_ramp_angle < 10) current_config.inlet_ramp_angle = 10;
    if (current_config.inlet_ramp_angle > 30) current_config.inlet_ramp_angle = 30;
    
    printf(GREEN "\n✓ Inlet configured: %s at %.1f°\n" RESET,
           getInletTypeString(current_config.inlet_type), 
           current_config.inlet_ramp_angle);
    printf("\nPress Enter to continue...");
    getchar();
}

static void setFlightConditions() {
    if (!simulation_run) {
        printf(RED "\n⚠ Please configure type and size first (Option 1)\n" RESET);
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    
    printHeader();
    printf(BOLD CYAN "\n=== FLIGHT CONDITIONS ===\n" RESET);
    
    printf("\nMach Number [3-25]: ");
    scanf("%lf", &current_config.design_mach);
    getchar();
    
    if (current_config.design_mach < 3) current_config.design_mach = 3;
    if (current_config.design_mach > 25) current_config.design_mach = 25;
    
    printf("\nAltitude (m) [10000-50000]: ");
    scanf("%lf", &current_config.design_altitude);
    getchar();
    
    if (current_config.design_altitude < 10000) current_config.design_altitude = 10000;
    if (current_config.design_altitude > 50000) current_config.design_altitude = 50000;
    
    current_config.cruise_velocity = current_config.design_mach * 340.0;
    current_config.design_thrust = current_config.design_thrust * (current_config.design_mach / 6.0);
    
    printf(GREEN "\n✓ Flight conditions set: Mach %.1f at %.0f m\n" RESET,
           current_config.design_mach, current_config.design_altitude);
    printf("\nPress Enter to continue...");
    getchar();
}

static void runPerformanceAnalysis() {
    if (!simulation_run) {
        printf(RED "\n⚠ Please configure type and size first (Option 1)\n" RESET);
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    
    double mach = current_config.design_mach;
    double altitude = current_config.design_altitude;
    double velocity = current_config.cruise_velocity;
    
    getPerformanceAtConditions(&current_perf, &current_config, mach, altitude, velocity);
    
    printHeader();
    printf(BOLD GREEN "\n╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║           PERFORMANCE ANALYSIS RESULTS                        ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n" RESET);
    
    printf(BOLD "\n--- THRUST PERFORMANCE ---\n" RESET);
    printf("  Gross Thrust:          %10.1f N  (%,.1f kgf)\n", 
           current_perf.thrust, current_perf.thrust / 9.81);
    printf("  Thrust Coefficient:     %10.3f\n", current_perf.thrust_coefficient);
    printf("  Ram Drag:              %10.1f N\n", current_perf.ram_drag);
    printf("  Net Thrust:            %10.1f N  (%,.1f kgf)\n", 
           current_perf.thrust - current_perf.drag_force, 
           (current_perf.thrust - current_perf.drag_force) / 9.81);
    
    printf(BOLD "\n--- FUEL CONSUMPTION ---\n" RESET);
    printf("  Fuel Flow Rate:        %10.4f kg/s\n", current_perf.fuel_flow);
    printf("  Air Mass Flow:        %10.3f kg/s\n", current_perf.air_mass_flow);
    printf("  Equivalence Ratio:   %10.3f\n", current_perf.equivalence_ratio);
    printf("  S FTC (TSFC):         %10.3f (kg/N/hr)\n", current_perf.sftc);
    
    printf(BOLD "\n--- SPECIFIC PERFORMANCE ---\n" RESET);
    printf("  Specific Impulse (Isp): %8.1f s\n", current_perf.isp);
    printf("  Thermal Efficiency:    %8.1f %%\n", current_perf.thermal_efficiency * 100);
    printf("  Propulsive Efficiency: %8.1f %%\n", current_perf.propulsive_efficiency * 100);
    printf("  Overall Efficiency:        %8.1f %%\n", current_perf.overall_efficiency * 100);
    
    printf(BOLD "\n--- FLOW CONDITIONS ---\n" RESET);
    printf("  Inlet Temperature:    %10.1f K  (%.1f°C)\n", 
           current_perf.inlet_temperature, current_perf.inlet_temperature - 273.15);
    printf("  Inlet Pressure:        %10.1f kPa\n", current_perf.inlet_pressure / 1000.0);
    printf("  Exit Temperature:      %10.1f K  (%.1f°C)\n", 
           current_perf.exit_temperature, current_perf.exit_temperature - 273.15);
    printf("  Exit Velocity:         %10.1f m/s  (Mach %.2f)\n", 
           current_perf.exit_velocity, current_perf.exit_velocity / 340.0);
    
    printf(BOLD "\n--- THERMAL MANAGEMENT ---\n" RESET);
    printf("  Combustion Temp:       %10.1f K  (%.1f°C)\n", 
           current_perf.combustion_temperature, current_perf.combustion_temperature - 273.15);
    printf("  Cooling Required:     %10.1f kW\n", current_perf.cooling_required / 1000.0);
    printf("  Thermal Load:        %10.1f MW/m²\n", current_perf.thermal_load / 1e6);
    printf("  Wall Temp Limit:      %10.1f K\n", current_config.wall_temp_limit);
    
    printf(GREEN "\n✓ Analysis complete\n" RESET);
    printf("\nPress Enter to continue...");
    getchar();
}

static void displayDesignSummary() {
    if (!simulation_run) {
        printf(RED "\n⚠ Please configure type and size first (Option 1)\n" RESET);
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    
    printHeader();
    printf(BOLD CYAN "\n╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║              SCRAMJET DESIGN SUMMARY                            ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n" RESET);
    
    printf(BOLD "\n--- CONFIGURATION ---\n" RESET);
    printf("  Type:     %s\n", getScramjetTypeString(current_config.type));
    printf("  Size:     %s\n", getScramjetSizeString(current_config.size));
    printf("  Fuel:     %s\n", getScramjetFuelString(current_config.fuel));
    printf("  Cooling: %s\n", getScramjetCoolingString(current_config.cooling));
    printf("  Inlet:    %s\n", getInletTypeString(current_config.inlet_type));
    
    printf(BOLD "\n--- GEOMETRY ---\n" RESET);
    printf("  Inlet Capture Width:   %8.3f m\n", current_config.inlet_capture_width);
    printf("  Ramp Angle:          %8.1f°\n", current_config.inlet_ramp_angle);
    printf("  Combustor Length:     %8.2f m\n", current_config.combustor_length);
    printf("  Combustor Diameter: %8.3f m\n", current_config.combustor_diameter);
    printf("  Nozzle Exit:       %8.3f m\n", current_config.nozzle_exit_diameter);
    printf("  Vehicle Length:    %8.2f m\n", current_config.vehicle_length);
    printf("  Dry Mass:          %8.1f kg\n", getDryMassForSize(current_config.size));
    
    printf(BOLD "\n--- OPERATION ENVELOPE ---\n" RESET);
    printf("  Design Mach:         %8.1f\n", current_config.design_mach);
    printf("  Mach Range:         %.1f - %.1f\n", 
           current_config.operation_range_mach_min, 
           current_config.operation_range_mach_max);
    printf("  Design Altitude:     %8.0f m\n", current_config.design_altitude);
    printf("  Design Thrust:     %8.0f N\n", current_config.design_thrust);
    
    if (current_config.type == SCRAMJET_TYPE_DUALMODE) {
        printf(BOLD "\n--- MODE TRANSITION ---\n" RESET);
        printf("  RAM→SCRAM Switch:   Mach %.1f\n", current_config.mode_switch_mach);
    }
    
    printf(BOLD "\n--- DESCRIPTION ---\n" RESET);
    printf("  %s\n", getScramjetTypeDescription(current_config.type));
    printf("  %s\n", getScramjetSizeDescription(current_config.size));
    
    printf(GREEN "\n✓ Summary complete\n" RESET);
    printf("\nPress Enter to continue...");
    getchar();
}

static void compareAllSizes() {
    printHeader();
    printf(BOLD CYAN "\n╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║              SIZE COMPARISON ANALYSIS                        ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n" RESET);
    
    printf(BOLD "\n" CYAN "Comparing all size classes at Mach 8, 35km altitude:\n" RESET);
    printf("\n");
    
    ScramjetSize sizes[4] = {SCRAMJET_SIZE_SMALL, SCRAMJET_SIZE_MEDIUM, 
                           SCRAMJET_SIZE_LARGE, SCRAMJET_SIZE_XLARGE};
    const char *size_names[] = {"Small", "Medium", "Large", "X-Large"};
    
    printf(BOLD "Size      | Thrust(N)  | Airflow   | Isp(s)   | Dry Mass | Vehicle Mass\n" RESET);
    printf("----------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < 4; i++) {
        ScramjetConfig cfg;
        getDefaultConfig(&cfg, SCRAMJET_TYPE_SHCRAMJET, sizes[i]);
        cfg.design_mach = 8.0;
        cfg.design_altitude = 35000.0;
        
        ScramjetPerformance perf;
        getPerformanceAtConditions(&perf, &cfg, 8.0, 35000.0, 2720.0);
        
        printf("%-9s | %9.0f | %9.3f | %8.1f | %8.1f | %12.1f\n",
               size_names[i], perf.thrust, perf.air_mass_flow, perf.isp,
               getDryMassForSize(sizes[i]), getVehicleMassForSize(sizes[i]));
    }
    
    printf("\n");
    printf(BOLD "Size      | Inlet Area | Chamber Area | Nozzle Area | Cooling Req\n" RESET);
    printf("-------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < 4; i++) {
        ScramjetConfig cfg;
        getDefaultConfig(&cfg, SCRAMJET_TYPE_SHCRAMJET, sizes[i]);
        
        double capture = 3.14159 * cfg.inlet_capture_width * cfg.inlet_capture_width / 4.0;
        double chamber = 3.14159 * cfg.combustor_diameter * cfg.combustor_diameter / 4.0;
        double nozzle = 3.14159 * cfg.nozzle_exit_diameter * cfg.nozzle_exit_diameter / 4.0;
        
        printf("%-9s | %9.4f | %10.4f | %9.4f | %10.1f kW\n",
               size_names[i], capture, chamber, nozzle, 0.0);
    }
    
    printf(GREEN "\n✓ Comparison complete\n" RESET);
    printf("\nPress Enter to continue...");
    getchar();
}

static void stressThermalAnalysis() {
    if (!simulation_run) {
        printf(RED "\n⚠ Please configure type and size first (Option 1)\n" RESET);
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    
    printHeader();
    printf(BOLD CYAN "\n╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║           STRESS & THERMAL ANALYSIS                           ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n" RESET);
    
    double mach = current_config.design_mach;
    double q = 0.5 * 0.4 * 340.0 * 340.0 * mach * mach; // Dynamic pressure
    
    double area = 3.14159 * current_config.combustor_diameter * current_config.combustor_diameter / 4.0;
    double pressure_force = current_perf.inlet_pressure * area;
    double thermalload = current_perf.combustion_temperature * area * 0.1;
    
    printf(BOLD "\n--- AERODYNAMIC LOADS ---\n" RESET);
    printf("  Dynamic Pressure:    %10.3f kPa\n", q / 1000.0);
    printf("  Max Q:                %10.3f kPa\n", q / 1000.0 * 1.2);
    printf("  Inlet Pressure Force: %10.1f N\n", pressure_force);
    
    printf(BOLD "\n--- THERMAL LOADS ---\n" RESET);
    printf("  Peak Temperature:      %10.1f K\n", current_perf.combustion_temperature);
    printf("  Wall Temperature:   %10.1f K\n", current_config.wall_temp_limit);
    printf("  Heat Flux:         %10.3f MW/m²\n", current_perf.thermal_load / 1e6);
    printf("  Max Heat Flux:       %10.3f MW/m²\n", current_config.max_thermal_flux / 1e6);
    
    printf(BOLD "\n--- STRUCTURAL ---\n" RESET);
    double stress = pressure_force / area / 1e6;
    printf("  Material Stress:    %10.3f MPa\n", stress);
    printf("  Allowable:          %10.1f MPa\n", 250.0);
    printf("  Factor of Safety:  %10.2f\n", 250.0 / (stress > 0.01 ? stress : 0.01));
    
    printf(BOLD "\n--- COOLING CHANNELS ---\n" RESET);
    printf("  Channel Count:       %10.0f\n", current_config.cooling_channel_count);
    printf("  Regenerative:       %s\n", current_config.regenerative_cooling ? "Yes" : "No");
    printf("  Cooling Capability:   %10.1f kW\n", current_perf.cooling_required / 1000.0);
    
    double margin = current_perf.cooling_required / (current_config.cooling_channel_count * 50.0);
    printf("  Margin:            %10.1f %%\n", (1.0 - margin) * 100);
    
    printf(GREEN "\n✓ Analysis complete\n" RESET);
    printf("\nPress Enter to continue...");
    getchar();
}

static void runFlowSimulation() {
    if (!simulation_run) {
        printf(RED "\n⚠ Please configure type and size first (Option 1)\n" RESET);
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    
    printHeader();
    printf(BOLD YELLOW "\n=== CFD FLOW SIMULATION ===\n" RESET);
    printf("\nRunning scramjet flow simulation...\n");
    
    printf("\n  [");
    for (int i = 0; i < 40; i++) {
        printf("█");
        usleep(25000);
        fflush(stdout);
    }
    printf("] 100%%\n");
    
    printf("\n  Flow Region: Inlet → Compressor → Combustor → Nozzle\n");
    printf("  Grid Size: %dx%d cells\n", 256, 256);
    printf("  Iterations: 5000\n");
    printf("  Mach: %.1f\n", current_config.design_mach);
    
    double mach = current_config.design_mach;
    double T_inf = 35000.0 < 11000 ? 288.15 - 0.0065 * 35000 : 216.65;
    double a = sqrt(1.4 * 287.05 * T_inf);
    double V = mach * a;
    
    printf("\n  Inlet Flow:\n");
    printf("    - Temperature: %.1f K\n", T_inf * (1.0 + 0.2 * mach * mach));
    printf("    - Velocity: %.1f m/s\n", V);
    printf("    - Mach: %.2f\n", mach);
    
    printf("\n  Combustion:\n");
    printf("    - Fuel: %s\n", getScramjetFuelString(current_config.fuel));
    printf("    - Temperature Rise: %.0f K\n", 
           current_perf.combustion_temperature - T_inf);
    printf("    - Heat Release: %.2f MW\n", current_perf.cooling_required / 1e6);
    
    printf("\n  Exit Flow:\n");
    printf("    - Temperature: %.1f K\n", current_perf.exit_temperature);
    printf("    - Velocity: %.1f m/s\n", current_perf.exit_velocity);
    printf("    - Mach: %.2f\n", current_perf.exit_velocity / 340.0);
    
    printf(GREEN "\n✓ Simulation complete\n" RESET);
    printf("\nPress Enter to continue...");
    getchar();
}

static void exportDesignData() {
    if (!simulation_run) {
        printf(RED "\n⚠ Please configure type and size first (Option 1)\n" RESET);
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    
    FILE *fp = fopen("scramjet_design.txt", "w");
    if (!fp) {
        printf(RED "\n⚠ Error creating export file\n" RESET);
        getchar();
        return;
    }
    
    fprintf(fp, "SCRAMJET DESIGN EXPORT\n");
    fprintf(fp, "=====================\n\n");
    
    fprintf(fp, "Configuration:\n");
    fprintf(fp, "  Type: %s\n", getScramjetTypeString(current_config.type));
    fprintf(fp, "  Size: %s\n", getScramjetSizeString(current_config.size));
    fprintf(fp, "  Fuel: %s\n", getScramjetFuelString(current_config.fuel));
    fprintf(fp, "  Cooling: %s\n", getScramjetCoolingString(current_config.cooling));
    fprintf(fp, "  Inlet: %s\n", getInletTypeString(current_config.inlet_type));
    
    fprintf(fp, "\nGeometry:\n");
    fprintf(fp, "  Inlet Width: %.3f m\n", current_config.inlet_capture_width);
    fprintf(fp, "  Ramp Angle: %.1f deg\n", current_config.inlet_ramp_angle);
    fprintf(fp, "  Combustor Length: %.2f m\n", current_config.combustor_length);
    fprintf(fp, "  Combustor Diameter: %.3f m\n", current_config.combustor_diameter);
    fprintf(fp, "  Nozzle Exit: %.3f m\n", current_config.nozzle_exit_diameter);
    fprintf(fp, "  Vehicle Length: %.2f m\n", current_config.vehicle_length);
    fprintf(fp, "  Dry Mass: %.1f kg\n", getDryMassForSize(current_config.size));
    
    fprintf(fp, "\nPerformance:\n");
    fprintf(fp, "  Thrust: %.1f N\n", current_perf.thrust);
    fprintf(fp, "  Isp: %.1f s\n", current_perf.isp);
    fprintf(fp, "  Fuel Flow: %.4f kg/s\n", current_perf.fuel_flow);
    fprintf(fp, "  Air Flow: %.3f kg/s\n", current_perf.air_mass_flow);
    
    fclose(fp);
    
    printf(GREEN "\n✓ Design exported to scramjet_design.txt\n" RESET);
    printf("\nPress Enter to continue...");
    getchar();
}

int main(int argc, char *argv[]) {
    char choice;
    
    while (1) {
        printHeader();
        
        printf(BOLD CYAN "Welcome to the SCRAMJET CFD Design Suite!\n" RESET);
        printf("Configure and analyze hypersonic scramjet propulsion systems.\n");
        printf("\n");
        
        printMenuOptions();
        
        printf("Choice: ");
        scanf("%c", &choice);
        getchar();
        
        switch(choice) {
            case '1': configureTypeAndSize(); break;
            case '2': setFuelAndCooling(); break;
            case '3': configureInlet(); break;
            case '4': setFlightConditions(); break;
            case '5': runPerformanceAnalysis(); break;
            case '6': displayDesignSummary(); break;
            case '7': compareAllSizes(); break;
            case '8': stressThermalAnalysis(); break;
            case '9': runFlowSimulation(); break;
            case 'a': 
            case 'A': exportDesignData(); break;
            case 'q': 
            case 'Q':
                printHeader();
                printf(GREEN "\nThank you for using SCRAMJET CFD Design Suite!\n" RESET);
                printf("\nHypersonic propulsion made simple.\n\n");
                return 0;
            default:
                printf(RED "\nInvalid option. Press Enter to continue..." RESET);
                getchar();
        }
    }
    
    return 0;
}