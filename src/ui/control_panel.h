#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include "gui_framework.h"
#include "units.h"
#include <stdbool.h>

typedef enum {
    UNIT_ALT_FEET,
    UNIT_ALT_METERS
} AltitudeUnit;

typedef struct {
    float airspeed;
    float altitude;
    float distance;
    float fuel;
    float angle_of_attack;
    float temperature;
    float pressure;
    float turbulence;
    float viscosity;
    float characteristic_length;
    
    SpeedUnit speed_unit;
    AltitudeUnit altitude_unit;
    
    float mach_number;
    float reynolds_number;
    float density;
    float dynamic_pressure;
    float sound_speed;
    
    Rectangle bounds;
    Slider airspeed_slider;
    Slider altitude_slider;
    Slider distance_slider;
    Slider fuel_slider;
    Slider aoa_slider;
    Toggle speed_toggle;
    Toggle altitude_toggle;
    
    void (*on_param_changed)(void*);
    void *user_data;
} ControlPanel;

ControlPanel* control_panel_create(Rectangle bounds);
void control_panel_destroy(ControlPanel *cp);
void control_panel_update(ControlPanel *cp);
void control_panel_draw(ControlPanel *cp);
void control_panel_get_values(ControlPanel *cp, float *airspeed, float *altitude, float *distance, float *fuel, float *aoa);
void control_panel_set_derived(ControlPanel *cp, float mach, float reynolds, float density, float dynamic_pressure, float sound_speed);

#endif
