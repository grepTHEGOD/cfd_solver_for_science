#ifndef CONTROL_PANEL_C
#define CONTROL_PANEL_C

#include "control_panel.h"
#include "physics/units.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ControlPanel* control_panel_create(Rectangle bounds) {
    ControlPanel *cp = (ControlPanel*)calloc(1, sizeof(ControlPanel));
    if (!cp) return NULL;
    
    cp->bounds = bounds;
    
    cp->airspeed = 150.0f;
    cp->altitude = 0.0f;
    cp->distance = 1000.0f;
    cp->fuel = 5000.0f;
    cp->angle_of_attack = 0.0f;
    cp->temperature = 288.15f;
    cp->pressure = 101.325f;
    cp->turbulence = 5.0f;
    cp->viscosity = 1.8e-5f;
    cp->characteristic_length = 1.0f;
    
    cp->speed_unit = UNIT_MPH;
    cp->altitude_unit = UNIT_ALT_FEET;
    
    cp->mach_number = 0.2f;
    cp->reynolds_number = 2.5e6f;
    cp->density = 1.225f;
    cp->dynamic_pressure = 13725.0f;
    cp->sound_speed = 343.0f;
    
    float panel_x = bounds.x + 10;
    float panel_y = bounds.y + 40;
    float slider_width = bounds.width - 120;
    float row_height = 45;
    
    cp->airspeed_slider = gui_slider(
        (Rectangle){panel_x, panel_y, slider_width, 30},
        150.0f, 0.0f, 2000.0f, 1.0f,
        "Airspeed", cp->speed_unit == UNIT_MPH ? "mph" : "Mach"
    );
    
    cp->altitude_slider = gui_slider(
        (Rectangle){panel_x, panel_y + row_height, slider_width, 30},
        0.0f, 0.0f, 100000.0f, 100.0f,
        "Altitude", cp->altitude_unit == UNIT_ALT_FEET ? "ft" : "m"
    );
    
    cp->distance_slider = gui_slider(
        (Rectangle){panel_x, panel_y + row_height * 2, slider_width, 30},
        1000.0f, 0.0f, 50000.0f, 100.0f,
        "Distance", "km"
    );
    
    cp->fuel_slider = gui_slider(
        (Rectangle){panel_x, panel_y + row_height * 3, slider_width, 30},
        5000.0f, 0.0f, 100000.0f, 100.0f,
        "Fuel", "kg"
    );
    
    cp->aoa_slider = gui_slider(
        (Rectangle){panel_x, panel_y + row_height * 4, slider_width, 30},
        0.0f, -45.0f, 45.0f, 0.5f,
        "Angle of Attack", "deg"
    );
    
    bool speed_is_mph = (cp->speed_unit == UNIT_MPH);
    bool alt_is_ft = (cp->altitude_unit == UNIT_ALT_FEET);
    
    cp->speed_toggle = gui_toggle(
        (Rectangle){panel_x + slider_width + 10, panel_y, 100, 30},
        &speed_is_mph, "Speed", "mph", "Mach"
    );
    
    cp->altitude_toggle = gui_toggle(
        (Rectangle){panel_x + slider_width + 10, panel_y + row_height, 100, 30},
        &alt_is_ft, "Altitude", "ft", "m"
    );
    
    cp->speed_unit = speed_is_mph ? UNIT_MPH : UNIT_MACH;
    cp->altitude_unit = alt_is_ft ? UNIT_ALT_FEET : UNIT_ALT_METERS;
    
    return cp;
}

void control_panel_destroy(ControlPanel *cp) {
    if (!cp) return;
    free(cp);
}

void control_panel_update(ControlPanel *cp) {
    if (!cp) return;
    
    if (gui_slider_draw(&cp->airspeed_slider)) {
        cp->airspeed = cp->airspeed_slider.value;
    }
    
    if (gui_slider_draw(&cp->altitude_slider)) {
        cp->altitude = cp->altitude_slider.value;
    }
    
    if (gui_slider_draw(&cp->distance_slider)) {
        cp->distance = cp->distance_slider.value;
    }
    
    if (gui_slider_draw(&cp->fuel_slider)) {
        cp->fuel = cp->fuel_slider.value;
    }
    
    if (gui_slider_draw(&cp->aoa_slider)) {
        cp->angle_of_attack = cp->aoa_slider.value;
    }
    
    gui_toggle_draw(&cp->speed_toggle);
    gui_toggle_draw(&cp->altitude_toggle);
    
    cp->speed_unit = cp->speed_toggle.value ? UNIT_MPH : UNIT_MACH;
    cp->altitude_unit = cp->altitude_toggle.value ? UNIT_ALT_FEET : UNIT_ALT_METERS;
    
    cp->airspeed_slider.unit = cp->speed_unit == UNIT_MPH ? "mph" : "Mach";
    cp->altitude_slider.unit = cp->altitude_unit == UNIT_ALT_FEET ? "ft" : "m";
}

void control_panel_draw(ControlPanel *cp) {
    if (!cp) return;
    
    DrawRectangleRec(cp->bounds, GUI_COLOR_PANEL);
    DrawRectangleLinesEx(cp->bounds, 1, GUI_COLOR_BORDER);
    
    DrawText("Simulation Parameters", (int)(cp->bounds.x + 15), (int)(cp->bounds.y + 12), 16, GUI_COLOR_TEXT);
    
    float panel_x = cp->bounds.x + 10;
    float panel_y = cp->bounds.y + 40;
    float slider_width = cp->bounds.width - 120;
    float row_height = 45;
    
    gui_slider_draw(&cp->airspeed_slider);
    gui_slider_draw(&cp->altitude_slider);
    gui_slider_draw(&cp->distance_slider);
    gui_slider_draw(&cp->fuel_slider);
    gui_slider_draw(&cp->aoa_slider);
    
    gui_toggle_draw(&cp->speed_toggle);
    gui_toggle_draw(&cp->altitude_toggle);
    
    float derived_y = panel_y + row_height * 5;
    char buf[64];
    
    snprintf(buf, sizeof(buf), "Mach: %.3f", cp->mach_number);
    DrawText(buf, (int)(panel_x), (int)derived_y, 14, GUI_COLOR_ACCENT);
    
    snprintf(buf, sizeof(buf), "Reynolds: %.2e", cp->reynolds_number);
    DrawText(buf, (int)(panel_x + 180), (int)derived_y, 14, GUI_COLOR_ACCENT);
    
    snprintf(buf, sizeof(buf), "Density: %.3f kg/m³", cp->density);
    DrawText(buf, (int)(panel_x + 400), (int)derived_y, 14, GUI_COLOR_ACCENT);
    
    snprintf(buf, sizeof(buf), "Dyn. Press: %.0f Pa", cp->dynamic_pressure);
    DrawText(buf, (int)(panel_x + 600), (int)derived_y, 14, GUI_COLOR_ACCENT);
    
    snprintf(buf, sizeof(buf), "Sound Speed: %.1f m/s", cp->sound_speed);
    DrawText(buf, (int)(panel_x + 800), (int)derived_y, 14, GUI_COLOR_ACCENT);
}

void control_panel_get_values(ControlPanel *cp, float *airspeed, float *altitude, float *distance, float *fuel, float *aoa) {
    if (!cp) return;
    if (airspeed) *airspeed = cp->airspeed;
    if (altitude) *altitude = cp->altitude;
    if (distance) *distance = cp->distance;
    if (fuel) *fuel = cp->fuel;
    if (aoa) *aoa = cp->angle_of_attack;
}

void control_panel_set_derived(ControlPanel *cp, float mach, float reynolds, float density, float dynamic_pressure, float sound_speed) {
    if (!cp) return;
    cp->mach_number = mach;
    cp->reynolds_number = reynolds;
    cp->density = density;
    cp->dynamic_pressure = dynamic_pressure;
    cp->sound_speed = sound_speed;
}

#endif
