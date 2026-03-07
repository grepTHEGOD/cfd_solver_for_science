#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include <gtk/gtk.h>

typedef struct ControlPanel ControlPanel;

typedef enum {
    UNIT_ALT_FEET,
    UNIT_ALT_METERS
} AltitudeUnit;

typedef enum {
    UNIT_SPEED_MPH,
    UNIT_SPEED_MACH,
    UNIT_SPEED_MS
} SpeedUnit;

typedef enum {
    UNIT_TEMP_K,
    UNIT_TEMP_C,
    UNIT_TEMP_F
} TemperatureUnit;

typedef enum {
    UNIT_PRESSURE_KPA,
    UNIT_PRESSURE_PA,
    UNIT_PRESSURE_PSI,
    UNIT_PRESSURE_ATM
} PressureUnit;

struct ControlPanel {
    GtkWidget *frame;
    GtkWidget *grid;
    
    GtkWidget *airspeed_scale;
    GtkWidget *airspeed_label;
    GtkWidget *altitude_scale;
    GtkWidget *altitude_label;
    GtkWidget *aoa_scale;
    GtkWidget *aoa_label;
    GtkWidget *pressure_scale;
    GtkWidget *pressure_label;
    GtkWidget *temperature_scale;
    GtkWidget *temperature_label;
    GtkWidget *turbulence_scale;
    GtkWidget *turbulence_label;
    GtkWidget *viscosity_scale;
    GtkWidget *viscosity_label;
    
    GtkWidget *altitude_unit_btn;
    GtkWidget *speed_unit_btn;
    GtkWidget *temp_unit_btn;
    GtkWidget *pressure_unit_btn;
    
    GtkWidget *mach_label;
    GtkWidget *reynolds_label;
    GtkWidget *density_label;
    GtkWidget *dynamic_pressure_label;
    
    AltitudeUnit alt_unit;
    SpeedUnit speed_unit;
    TemperatureUnit temp_unit;
    PressureUnit pressure_unit;
    
    double airspeed;
    double altitude;
    double angle_of_attack;
    double pressure;
    double temperature;
    double turbulence;
    double viscosity;
    double characteristic_length;
    
    void (*on_param_changed)(void*);
    void *user_data;
};

ControlPanel* control_panel_create(void);
void control_panel_destroy(ControlPanel *cp);
void control_panel_set_values(ControlPanel *cp,
    double airspeed, double altitude, double aoa,
    double pressure, double temperature);
void control_panel_get_values(ControlPanel *cp,
    double *airspeed, double *altitude, double *aoa,
    double *pressure, double *temperature);
void control_panel_set_unit(ControlPanel *cp, AltitudeUnit unit);
void control_panel_set_speed_unit(ControlPanel *cp, SpeedUnit unit);
void control_panel_set_temp_unit(ControlPanel *cp, TemperatureUnit unit);
void control_panel_set_pressure_unit(ControlPanel *cp, PressureUnit unit);
void control_panel_update_derived(ControlPanel *cp,
    double mach, double reynolds, double density, double q);
void control_panel_attach_callbacks(ControlPanel *cp,
    void (*on_param_changed)(void*),
    void *user_data);

#endif
