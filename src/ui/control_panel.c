#ifndef CONTROL_PANEL_C
#define CONTROL_PANEL_C

#include "control_panel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void on_airspeed_changed(GtkRange *range, gpointer data) {
    (void)range;
    ControlPanel *cp = (ControlPanel*)data;
    if (cp && cp->on_param_changed) {
        cp->airspeed = gtk_range_get_value(range);
        cp->on_param_changed(cp->user_data);
    }
}

static void on_altitude_changed(GtkRange *range, gpointer data) {
    (void)range;
    ControlPanel *cp = (ControlPanel*)data;
    if (cp && cp->on_param_changed) {
        cp->altitude = gtk_range_get_value(range);
        cp->on_param_changed(cp->user_data);
    }
}

static void on_aoa_changed(GtkRange *range, gpointer data) {
    (void)range;
    ControlPanel *cp = (ControlPanel*)data;
    if (cp && cp->on_param_changed) {
        cp->angle_of_attack = gtk_range_get_value(range);
        cp->on_param_changed(cp->user_data);
    }
}

static void on_pressure_changed(GtkRange *range, gpointer data) {
    (void)range;
    ControlPanel *cp = (ControlPanel*)data;
    if (cp && cp->on_param_changed) {
        cp->pressure = gtk_range_get_value(range);
        cp->on_param_changed(cp->user_data);
    }
}

static void on_temperature_changed(GtkRange *range, gpointer data) {
    (void)range;
    ControlPanel *cp = (ControlPanel*)data;
    if (cp && cp->on_param_changed) {
        cp->temperature = gtk_range_get_value(range);
        cp->on_param_changed(cp->user_data);
    }
}

static void on_turbulence_changed(GtkRange *range, gpointer data) {
    (void)range;
    ControlPanel *cp = (ControlPanel*)data;
    if (cp && cp->on_param_changed) {
        cp->turbulence = gtk_range_get_value(range);
        cp->on_param_changed(cp->user_data);
    }
}

static void on_viscosity_changed(GtkRange *range, gpointer data) {
    (void)range;
    ControlPanel *cp = (ControlPanel*)data;
    if (cp && cp->on_param_changed) {
        double log_val = gtk_range_get_value(range);
        cp->viscosity = pow(10.0, log_val - 5.0);
        cp->on_param_changed(cp->user_data);
    }
}

static void update_label_value(GtkWidget *label, double value, const char *unit) {
    char buf[64];
    snprintf(buf, sizeof(buf), "%.2f %s", value, unit);
    gtk_label_set_text(GTK_LABEL(label), buf);
}

ControlPanel* control_panel_create(void) {
    ControlPanel *cp = (ControlPanel*)calloc(1, sizeof(ControlPanel));
    if (!cp) return NULL;
    
    cp->frame = gtk_frame_new("Simulation Parameters");
    gtk_frame_set_shadow_type(GTK_FRAME(cp->frame), GTK_SHADOW_NONE);
    
    cp->grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(cp->grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(cp->grid), 8);
    gtk_container_set_border_width(GTK_CONTAINER(cp->grid), 8);
    gtk_container_add(GTK_CONTAINER(cp->frame), cp->grid);
    
    GtkWidget *lbl = gtk_label_new("Airspeed:");
    gtk_misc_set_alignment(GTK_MISC(lbl), 0.0, 0.5);
    cp->airspeed_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.0, 2000.0, 1.0);
    gtk_scale_set_value_pos(GTK_SCALE(cp->airspeed_scale), GTK_POS_RIGHT);
    gtk_range_set_value(GTK_RANGE(cp->airspeed_scale), 150.0);
    g_signal_connect(cp->airspeed_scale, "value-changed", G_CALLBACK(on_airspeed_changed), cp);
    cp->airspeed_label = gtk_label_new("150.0 mph");
    gtk_misc_set_alignment(GTK_MISC(cp->airspeed_label), 0.0, 0.5);
    gtk_grid_attach(GTK_GRID(cp->grid), lbl, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(cp->grid), cp->airspeed_scale, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(cp->grid), cp->airspeed_label, 2, 0, 1, 1);
    
    lbl = gtk_label_new("Altitude:");
    gtk_misc_set_alignment(GTK_MISC(lbl), 0.0, 0.5);
    cp->altitude_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.0, 100000.0, 100.0);
    gtk_scale_set_value_pos(GTK_SCALE(cp->altitude_scale), GTK_POS_RIGHT);
    gtk_range_set_value(GTK_RANGE(cp->altitude_scale), 0.0);
    g_signal_connect(cp->altitude_scale, "value-changed", G_CALLBACK(on_altitude_changed), cp);
    cp->altitude_label = gtk_label_new("0 ft");
    gtk_misc_set_alignment(GTK_MISC(cp->altitude_label), 0.0, 0.5);
    gtk_grid_attach(GTK_GRID(cp->grid), lbl, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(cp->grid), cp->altitude_scale, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(cp->grid), cp->altitude_label, 2, 1, 1, 1);
    
    lbl = gtk_label_new("Angle of Attack:");
    gtk_misc_set_alignment(GTK_MISC(lbl), 0.0, 0.5);
    cp->aoa_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, -45.0, 45.0, 0.5);
    gtk_scale_set_value_pos(GTK_SCALE(cp->aoa_scale), GTK_POS_RIGHT);
    gtk_range_set_value(GTK_RANGE(cp->aoa_scale), 0.0);
    g_signal_connect(cp->aoa_scale, "value-changed", G_CALLBACK(on_aoa_changed), cp);
    cp->aoa_label = gtk_label_new("0.0 deg");
    gtk_misc_set_alignment(GTK_MISC(cp->aoa_label), 0.0, 0.5);
    gtk_grid_attach(GTK_GRID(cp->grid), lbl, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(cp->grid), cp->aoa_scale, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(cp->grid), cp->aoa_label, 2, 2, 1, 1);
    
    lbl = gtk_label_new("Pressure:");
    gtk_misc_set_alignment(GTK_MISC(lbl), 0.0, 0.5);
    cp->pressure_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 50.0, 150.0, 0.1);
    gtk_scale_set_value_pos(GTK_SCALE(cp->pressure_scale), GTK_POS_RIGHT);
    gtk_range_set_value(GTK_RANGE(cp->pressure_scale), 101.325);
    g_signal_connect(cp->pressure_scale, "value-changed", G_CALLBACK(on_pressure_changed), cp);
    cp->pressure_label = gtk_label_new("101.325 kPa");
    gtk_misc_set_alignment(GTK_MISC(cp->pressure_label), 0.0, 0.5);
    gtk_grid_attach(GTK_GRID(cp->grid), lbl, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(cp->grid), cp->pressure_scale, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(cp->grid), cp->pressure_label, 2, 3, 1, 1);
    
    lbl = gtk_label_new("Temperature:");
    gtk_misc_set_alignment(GTK_MISC(lbl), 0.0, 0.5temperature_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 150.0, 400.0, );
    cp->0.1);
    gtk_scale_set_value_pos(GTK_SCALE(cp->temperature_scale), GTK_POS_RIGHT);
    gtk_range_set_value(GTK_RANGE(cp->temperature_scale), 288.15);
    g_signal_connect(cp->temperature_scale, "value-changed", G_CALLBACK(on_temperature_changed), cp);
    cp->temperature_label = gtk_label_new("288.15 K");
    gtk_misc_set_alignment(GTK_MISC(cp->temperature_label), 0.0, 0.5);
    gtk_grid_attach(GTK_GRID(cp->grid), lbl, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(cp->grid), cp->temperature_scale, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(cp->grid), cp->temperature_label, 2, 4, 1, 1);
    
    lbl = gtk_label_new("Mach Number:");
    cp->mach_label = gtk_label_new("0.20");
    gtk_misc_set_alignment(GTK_MISC(cp->mach_label), 0.0, 0.5);
    gtk_grid_attach(GTK_GRID(cp->grid), lbl, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(cp->grid), cp->mach_label, 1, 5, 1, 1);
    
    lbl = gtk_label_new("Reynolds #:");
    cp->reynolds_label = gtk_label_new("2.5e6");
    gtk_misc_set_alignment(GTK_MISC(cp->reynolds_label), 0.0, 0.5);
    gtk_grid_attach(GTK_GRID(cp->grid), lbl, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(cp->grid), cp->reynolds_label, 1, 6, 1, 1);
    
    cp->altitude_unit = UNIT_ALT_FEET;
    cp->speed_unit = UNIT_SPEED_MPH;
    cp->temp_unit = UNIT_TEMP_K;
    cp->pressure_unit = UNIT_PRESSURE_KPA;
    
    cp->airspeed = 150.0;
    cp->altitude = 0.0;
    cp->angle_of_attack = 0.0;
    cp->pressure = 101.325;
    cp->temperature = 288.15;
    cp->turbulence = 5.0;
    cp->viscosity = 1.8e-5;
    cp->characteristic_length = 1.0;
    
    gtk_widget_show_all(cp->frame);
    
    return cp;
}

void control_panel_destroy(ControlPanel *cp) {
    if (!cp) return;
    gtk_widget_destroy(cp->frame);
    free(cp);
}

void control_panel_set_values(ControlPanel *cp,
    double airspeed, double altitude, double aoa,
    double pressure, double temperature) {
    if (!cp) return;
    cp->airspeed = airspeed;
    cp->altitude = altitude;
    cp->angle_of_attack = aoa;
    cp->pressure = pressure;
    cp->temperature = temperature;
    
    gtk_range_set_value(GTK_RANGE(cp->airspeed_scale), airspeed);
    gtk_range_set_value(GTK_RANGE(cp->altitude_scale), altitude);
    gtk_range_set_value(GTK_RANGE(cp->aoa_scale), aoa);
    gtk_range_set_value(GTK_RANGE(cp->pressure_scale), pressure);
    gtk_range_set_value(GTK_RANGE(cp->temperature_scale), temperature);
}

void control_panel_get_values(ControlPanel *cp,
    double *airspeed, double *altitude, double *aoa,
    double *pressure, double *temperature) {
    if (!cp) return;
    if (airspeed) *airspeed = cp->airspeed;
    if (altitude) *altitude = cp->altitude;
    if (aoa) *aoa = cp->angle_of_attack;
    if (pressure) *pressure = cp->pressure;
    if (temperature) *temperature = cp->temperature;
}

void control_panel_set_unit(ControlPanel *cp, AltitudeUnit unit) {
    if (!cp) return;
    cp->altitude_unit = unit;
}

void control_panel_set_speed_unit(ControlPanel *cp, SpeedUnit unit) {
    if (!cp) return;
    cp->speed_unit = unit;
}

void control_panel_set_temp_unit(ControlPanel *cp, TemperatureUnit unit) {
    if (!cp) return;
    cp->temp_unit = unit;
}

void control_panel_set_pressure_unit(ControlPanel *cp, PressureUnit unit) {
    if (!cp) return;
    cp->pressure_unit = unit;
}

void control_panel_update_derived(ControlPanel *cp,
    double mach, double reynolds, double density, double q) {
    if (!cp) return;
    char buf[64];
    snprintf(buf, sizeof(buf), "%.2f", mach);
    gtk_label_set_text(GTK_LABEL(cp->mach_label), buf);
    snprintf(buf, sizeof(buf), "%.2e", reynolds);
    gtk_label_set_text(GTK_LABEL(cp->reynolds_label), buf);
    snprintf(buf, sizeof(buf), "%.3f kg/m³", density);
    gtk_label_set_text(GTK_LABEL(cp->density_label), buf);
    snprintf(buf, sizeof(buf), "%.2f Pa", q);
    gtk_label_set_text(GTK_LABEL(cp->dynamic_pressure_label), buf);
}

void control_panel_attach_callbacks(ControlPanel *cp,
    void (*on_param_changed)(void*),
    void *user_data) {
    if (!cp) return;
    cp->on_param_changed = on_param_changed;
    cp->user_data = user_data;
}

#endif
