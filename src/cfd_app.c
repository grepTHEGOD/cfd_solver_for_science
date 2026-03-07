#ifndef CFD_APP_C
#define CFD_APP_C

#include "cfd_app.h"
#include <stdio.h>
#include <stdlib.h>

CFDApp* cfd_app_create(void) {
    CFDApp *app = (CFDApp*)calloc(1, sizeof(CFDApp));
    if (!app) return NULL;
    
    app->model_manager = model_manager_create();
    app->is_running = 0;
    app->last_update_time = 0.0;
    
    app->sim_params.airspeed = 150.0;
    app->sim_params.altitude = 0.0;
    app->sim_params.angle_of_attack = 0.0;
    app->sim_params.pressure = 101.325;
    app->sim_params.temperature = 288.15;
    app->sim_params.turbulence_intensity = 5.0;
    app->sim_params.viscosity = 1.8e-5;
    app->sim_params.characteristic_length = 1.0;
    
    app->units.altitude_unit = UNIT_FEET;
    app->units.speed_unit = UNIT_MPH;
    app->units.temp_unit = UNIT_KELVIN;
    app->units.pressure_unit = UNIT_KPA;
    
    atmosphere_update(&app->atmosphere, app->sim_params.altitude);
    
    return app;
}

void cfd_app_destroy(CFDApp *app) {
    if (!app) return;
    if (app->main_window) main_window_destroy(app->main_window);
    if (app->model_manager) model_manager_destroy(app->model_manager);
    free(app);
}

void cfd_app_run(CFDApp *app) {
    app->is_running = 1;
}

void cfd_app_update(CFDApp *app, double delta_time) {
    if (!app || !app->is_running) return;
    app->last_update_time += delta_time;
}

void cfd_app_on_param_changed(CFDApp *app) {
    if (!app) return;
    atmosphere_update(&app->atmosphere, app->sim_params.altitude);
    
    double speed_ms = units_convert_speed(
        app->sim_params.airspeed, 
        app->units.speed_unit, 
        UNIT_M_S
    );
    
    app->atmosphere.mach_number = speed_ms / app->atmosphere.sound_speed;
    app->atmosphere.reynolds_number = (
        app->atmosphere.density * speed_ms * 
        app->sim_params.characteristic_length / 
        app->sim_params.viscosity
    );
    app->atmosphere.dynamic_pressure = 0.5 * app->atmosphere.density * speed_ms * speed_ms;
}

#endif
