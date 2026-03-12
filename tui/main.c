#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <ctype.h>

#define RESET      "\033[0m"
#define BOLD       "\033[1m"
#define DIM        "\033[2m"
#define CYAN       "\033[36m"
#define GREEN      "\033[32m"
#define YELLOW     "\033[33m"
#define RED        "\033[31m"
#define BLUE       "\033[34m"
#define MAGENTA    "\033[35m"
#define WHITE      "\033[37m"
#define BG_BLUE    "\033[44m"
#define BG_CYAN    "\033[46m"

void clearScreen() {
    printf("\033[2J\033[H");
    fflush(stdout);
}

void printHeader() {
    printf(BG_BLUE WHITE BOLD "\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║           CFD SOLVER FOR SCIENCE - CONTROL CENTER            ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf(RESET "\n");
}

void printStatus(const char* msg, int success) {
    if (success) {
        printf(GREEN "  [✓] %s\n" RESET, msg);
    } else {
        printf(RED "  [✗] %s\n" RESET, msg);
    }
}

void printInfo(const char* msg) {
    printf(BLUE "  [ℹ] %s\n" RESET, msg);
}

void loadingAnimation(const char* msg) {
    printf(YELLOW "  %s " RESET, msg);
    fflush(stdout);
    const char spin[] = "⠋⠙⠹⠸⠼⠴⠦⠧⠇⠏";
    for (int i = 0; i < 20; i++) {
        printf("\b%c", spin[i % 10]);
        fflush(stdout);
        usleep(50000);
    }
    printf("\n");
}

void pressEnterToContinue() {
    printf(YELLOW "\n  Press Enter to continue..." RESET);
    while (getchar() != '\n');
}

int getMenuChoice(int min, int max) {
    char input[32];
    int choice = -1;
    
    while (choice < min || choice > max) {
        printf(BOLD "\n  Enter your choice [" RESET YELLOW "%d-%d" RESET BOLD "]: " RESET, min, max);
        if (fgets(input, sizeof(input), stdin) != NULL) {
            choice = atoi(input);
            if (choice < min || choice > max) {
                printStatus("Invalid option. Please try again.", 0);
            }
        }
    }
    return choice;
}

void trim_newline(char *s) {
    s[strcspn(s, "\n")] = '\0';
}

void launchModule(const char *module, const char *path) {
    loadingAnimation((char *)module);
    pid_t pid = fork();
    if (pid == 0) {
        execl(path, module, NULL);
        exit(1);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printStatus((char *)module, 1);
        } else {
            printStatus((char *)module, 0);
        }
    }
}

void runSolver() {
    clearScreen();
    printHeader();
    
    printf(BOLD "\n  ═══════════════════════════ RUN CFD SOLVER ═══════════════════\n\n" RESET);
    
    printf("  Initializing solver parameters...\n\n");
    
    int choice;
    printf("  Select test case:\n");
    printf("    1. Subsonic flow (M=0.3)\n");
    printf("    2. Transonic flow (M=0.8)\n");
    printf("    3. Supersonic flow (M=2.0)\n");
    printf("    4. Hypersonic flow (M=5.0)\n");
    printf("    5. Custom parameters\n");
    printf("\n  Enter choice: ");
    scanf("%d", &choice);
    
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "./solver/cfd_solver");
    
    switch(choice) {
        case 1: strcat(cmd, " --mach 0.3 --iter 2000"); break;
        case 2: strcat(cmd, " --mach 0.8 --iter 3000"); break;
        case 3: strcat(cmd, " --mach 2.0 --iter 5000"); break;
        case 4: strcat(cmd, " --mach 5.0 --iter 5000"); break;
        case 5:
            printf("  Enter Mach number: ");
            double mach;
            scanf("%lf", &mach);
            snprintf(cmd, sizeof(cmd), "./solver/cfd_solver --mach %.2f", mach);
            break;
        default:
            strcat(cmd, " --mach 0.3 --iter 2000");
    }
    
    printf("\n");
    loadingAnimation("  Running CFD simulation");
    printf("\n");
    
    system(cmd);
    
    pressEnterToContinue();
}

void visualizeResults() {
    clearScreen();
    printHeader();
    
    printf(BOLD "\n  ═════════════════════════ VISUALIZATION ════════════════════\n\n" RESET);
    
    printf("  Launching 3D visualization module...\n");
    printf("  Note: Requires X11 display server\n\n");
    
    launchModule("Visualization", "./visualization/visualization");
    
    pressEnterToContinue();
}

void showAbout() {
    clearScreen();
    printHeader();
    
    printf(BOLD "\n  ════════════════════════════ ABOUT ═════════════════════════\n\n" RESET);
    printf("  " BOLD "CFD Solver for Science" RESET "\n");
    printf("  Version: 2.0.0\n\n");
    printf("  A comprehensive computational fluid dynamics package\n");
    printf("  designed for aerodynamic analysis and design.\n\n");
    printf(BOLD "  Features:" RESET "\n");
    printf("    • 2D Compressible Navier-Stokes solver\n");
    printf("    • Roe flux splitting for inviscid fluxes\n");
    printf("    • Multiple test cases (subsonic to hypersonic)\n");
    printf("    • Real-time flow visualization (raylib)\n");
    printf("    • Aerodynamic force coefficients\n");
    printf("    • Heat transfer analysis\n\n");
    
    printf(BOLD "  Build:" RESET "\n");
    printf("    mkdir build && cd build\n");
    printf("    cmake ..\n");
    printf("    make\n\n");
    
    pressEnterToContinue();
}

void displayMainMenu() {
    clearScreen();
    printHeader();
    
    const char* menu = 
        GREEN "  ┌────────────────────────────────────────────────────────────────┐\n"
        "  │                        MAIN MENU                                │\n"
        "  ├────────────────────────────────────────────────────────────────┤\n"
        "  │  " YELLOW "1." RESET "  Run CFD Solver       - Execute flow simulation          │\n"
        "  │  " YELLOW "2." RESET "  Visualize Results    - Open 3D flow visualization     │\n"
        "  │  " YELLOW "3." RESET "  About                - Project information             │\n"
        "  │  " YELLOW "0." RESET "  Exit                 - Quit application                 │\n"
        "  └────────────────────────────────────────────────────────────────┘\n" RESET;
    
    printf("%s", menu);
}

int main() {
    int choice;
    
    loadingAnimation("  Initializing CFD Solver");
    printStatus("System ready", 1);
    sleep(1);
    
    while (1) {
        displayMainMenu();
        choice = getMenuChoice(0, 3);
        
        switch (choice) {
            case 1:
                runSolver();
                break;
            case 2:
                visualizeResults();
                break;
            case 3:
                showAbout();
                break;
            case 0:
                clearScreen();
                printf(BG_CYAN WHITE BOLD);
                printf("\n  ╔══════════════════════════════════════════════════════╗\n");
                printf("  ║          Thank you for using CFD Solver!              ║\n");
                printf("  ║              Have a great day.                        ║\n");
                printf("  ╚══════════════════════════════════════════════════════╝\n");
                printf(RESET "\n");
                return 0;
        }
    }
    
    return 0;
}
