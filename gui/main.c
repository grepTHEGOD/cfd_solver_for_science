#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ncurses.h>
#include "../core/physics_bridge.h"

#define SLIDER_COUNT 11

typedef struct {
    float* value;
    float min_val;
    float max_val;
    float step;
    const char* label;
} Slider;

float g_velocity = 50.0f;
float g_altitude = 1000.0f;
float g_distance = 5000.0f;
float g_air_resistance = 0.3f;
float g_mass = 1000.0f;
float g_gravity = 9.81f;
float g_wing_area = 20.0f;
float g_drag_coeff = 0.3f;
float g_lift_coeff = 0.5f;
float g_angle_of_attack = 5.0f;
float g_initial_altitude = 0.0f;
int g_use_feet = 0;

C_FlightResult g_result;
bool g_result_valid = false;
bool g_model_loaded = false;
C_ModelProperties g_model_props;

Slider sliders[SLIDER_COUNT] = {
    {&g_velocity, 0.0f, 500.0f, 1.0f, "Velocity (m/s)"},
    {&g_altitude, 0.0f, 50000.0f, 10.0f, "Altitude"},
    {&g_distance, 0.0f, 100000.0f, 100.0f, "Distance"},
    {&g_air_resistance, 0.0f, 1.0f, 0.01f, "Air Resistance"},
    {&g_mass, 1.0f, 100000.0f, 10.0f, "Mass (kg)"},
    {&g_gravity, 1.0f, 20.0f, 0.1f, "Gravity (m/s2)"},
    {&g_wing_area, 0.1f, 500.0f, 0.5f, "Wing Area (m2)"},
    {&g_drag_coeff, 0.01f, 2.0f, 0.01f, "Drag Coefficient"},
    {&g_lift_coeff, 0.0f, 2.0f, 0.01f, "Lift Coefficient"},
    {&g_angle_of_attack, -30.0f, 30.0f, 0.5f, "Angle of Attack"},
    {&g_initial_altitude, 0.0f, 50000.0f, 10.0f, "Initial Altitude"}
};

void calculate_results() {
    double alt_m = g_use_feet ? c_feet_to_meters(g_altitude) : g_altitude;
    double init_alt_m = g_use_feet ? c_feet_to_meters(g_initial_altitude) : g_initial_altitude;
    
    if (g_model_loaded) {
        c_compute_model_aerodynamics(g_velocity, alt_m, g_angle_of_attack);
        g_model_props = c_get_model_properties();
        g_drag_coeff = g_model_props.drag_coefficient;
        g_lift_coeff = g_model_props.lift_coefficient;
    }
    
    g_result = c_compute_flight(g_velocity, alt_m + init_alt_m, g_distance, 
                                 g_mass, g_gravity, g_drag_coeff, g_lift_coeff, 
                                 g_wing_area, g_air_resistance);
    g_result_valid = true;
}

void render_slider(int y, Slider* s) {
    float range = s->max_val - s->min_val;
    float normalized = (*s->value - s->min_val) / range;
    int bar_width = 30;
    int fill = (int)(normalized * bar_width);
    
    char bar[64] = {0};
    snprintf(bar, sizeof(bar), "[");
    for (int i = 0; i < bar_width; i++) {
        if (i < fill) strcat(bar, "=");
        else if (i == fill) strcat(bar, "|");
        else strcat(bar, " ");
    }
    strcat(bar, "]");
    
    mvprintw(y, 2, "%-20s %s %10.2f", s->label, bar, *s->value);
}

void render_value(int y, int x, const char* label, const char* value) {
    mvprintw(y, x, "%-18s: %s", label, value);
}

int main(int argc, char* argv[]) {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_BLACK);
    
    calculate_results();
    
    int selected = 0;
    int ch;
    
    while ((ch = getch()) != KEY_F(1) && ch != 27) {
        if (ch == KEY_UP) {
            selected = (selected - 1 + SLIDER_COUNT + 1) % (SLIDER_COUNT + 1);
        } else if (ch == KEY_DOWN) {
            selected = (selected + 1) % (SLIDER_COUNT + 1);
        } else if (ch == KEY_LEFT) {
            if (selected < SLIDER_COUNT) {
                float new_val = *sliders[selected].value - sliders[selected].step;
                if (new_val >= sliders[selected].min_val) {
                    *sliders[selected].value = new_val;
                    calculate_results();
                }
            } else if (selected == SLIDER_COUNT) {
                g_use_feet = !g_use_feet;
                calculate_results();
            }
        } else if (ch == KEY_RIGHT) {
            if (selected < SLIDER_COUNT) {
                float new_val = *sliders[selected].value + sliders[selected].step;
                if (new_val <= sliders[selected].max_val) {
                    *sliders[selected].value = new_val;
                    calculate_results();
                }
            } else if (selected == SLIDER_COUNT) {
                g_use_feet = !g_use_feet;
                calculate_results();
            }
        }
        
        clear();
        
        attron(A_BOLD | COLOR_PAIR(1));
        mvprintw(0, 2, "=== CFD PHYSICS SIMULATOR ===");
        attroff(A_BOLD | COLOR_PAIR(1));
        
        mvprintw(1, 2, "Use Arrow Keys to navigate, Left/Right to adjust values, ESC to quit");
        
        attron(A_BOLD | COLOR_PAIR(1));
        mvprintw(3, 2, "--- FLIGHT PARAMETERS ---");
        attroff(A_BOLD | COLOR_PAIR(1));
        
        for (int i = 0; i < SLIDER_COUNT; i++) {
            if (i == selected) attron(A_REVERSE);
            render_slider(4 + i, &sliders[i]);
            if (i == selected) attroff(A_REVERSE);
        }
        
        int toggle_y = 4 + SLIDER_COUNT + 1;
        if (selected == SLIDER_COUNT) attron(A_REVERSE);
        mvprintw(toggle_y, 2, "[%c] Use Feet/Metres", g_use_feet ? 'x' : ' ');
        if (selected == SLIDER_COUNT) attroff(A_REVERSE);
        
        int right_col = 50;
        
        attron(A_BOLD | COLOR_PAIR(1));
        mvprintw(3, right_col, "--- FLIGHT RESULTS ---");
        attroff(A_BOLD | COLOR_PAIR(1));
        
        if (g_result_valid) {
            char buf[32];
            
            snprintf(buf, sizeof(buf), "%.2f s", g_result.time_of_flight);
            attron(COLOR_PAIR(2));
            render_value(5, right_col, "Time of Flight", buf);
            attroff(COLOR_PAIR(2));
            
            snprintf(buf, sizeof(buf), "%.2f m/s", g_result.final_velocity);
            render_value(6, right_col, "Final Velocity", buf);
            
            snprintf(buf, sizeof(buf), "%.0f m", g_result.distance_traveled);
            render_value(7, right_col, "Distance", buf);
            
            snprintf(buf, sizeof(buf), "%.2f N", g_result.drag_force);
            attron(COLOR_PAIR(4));
            render_value(9, right_col, "Drag Force", buf);
            attroff(COLOR_PAIR(4));
            
            snprintf(buf, sizeof(buf), "%.2f N", g_result.lift_force);
            attron(COLOR_PAIR(2));
            render_value(10, right_col, "Lift Force", buf);
            attroff(COLOR_PAIR(2));
            
            snprintf(buf, sizeof(buf), "%.0f J", g_result.kinetic_energy);
            render_value(11, right_col, "Kinetic Energy", buf);
            
            snprintf(buf, sizeof(buf), "%.0f J", g_result.potential_energy);
            render_value(12, right_col, "Potential Energy", buf);
        }
        
        double alt_m = g_use_feet ? c_feet_to_meters(g_altitude) : g_altitude;
        C_AtmosphericProperties atm = c_atmosphere_standard(g_velocity, alt_m);
        
        attron(A_BOLD | COLOR_PAIR(1));
        mvprintw(14, right_col, "--- ATMOSPHERE ---");
        attroff(A_BOLD | COLOR_PAIR(1));
        
        char buf[32];
        snprintf(buf, sizeof(buf), "%.2f K", atm.temperature);
        render_value(16, right_col, "Temperature", buf);
        
        snprintf(buf, sizeof(buf), "%.0f Pa", atm.pressure);
        render_value(17, right_col, "Pressure", buf);
        
        snprintf(buf, sizeof(buf), "%.4f kg/m3", atm.density);
        render_value(18, right_col, "Density", buf);
        
        snprintf(buf, sizeof(buf), "%.1f m/s", atm.speed_of_sound);
        render_value(19, right_col, "Speed of Sound", buf);
        
        snprintf(buf, sizeof(buf), "%.4f", atm.Mach_number);
        render_value(20, right_col, "Mach Number", buf);
        
        C_AerodynamicForces aero = c_compute_aerodynamics(g_velocity, alt_m, g_angle_of_attack);
        
        attron(A_BOLD | COLOR_PAIR(1));
        mvprintw(21, right_col, "--- AERODYNAMICS ---");
        attroff(A_BOLD | COLOR_PAIR(1));
        
        snprintf(buf, sizeof(buf), "%.4f", aero.Cl);
        attron(COLOR_PAIR(3));
        render_value(23, right_col, "Lift Coeff", buf);
        attroff(COLOR_PAIR(3));
        
        snprintf(buf, sizeof(buf), "%.4f", aero.Cd);
        attron(COLOR_PAIR(4));
        render_value(24, right_col, "Drag Coeff", buf);
        attroff(COLOR_PAIR(4));
        
        if (g_model_loaded) {
            attron(A_BOLD | COLOR_PAIR(2));
            mvprintw(25, right_col, "--- 3D MODEL LOADED ---");
            attroff(A_BOLD | COLOR_PAIR(2));
            
            snprintf(buf, sizeof(buf), "%d", g_model_props.vertex_count);
            render_value(26, right_col, "Vertices", buf);
            
            snprintf(buf, sizeof(buf), "%d", g_model_props.face_count);
            render_value(27, right_col, "Faces", buf);
        }
        
        refresh();
    }
    
    endwin();
    return 0;
}