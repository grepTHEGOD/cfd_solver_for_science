#ifndef CFD_APP_C
#define CFD_APP_C

#include "cfd_app.h"
#include "physics/units.h"
#include "models/mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CFDApp* cfd_app_create(void) {
    CFDApp *app = (CFDApp*)calloc(1, sizeof(CFDApp));
    if (!app) return NULL;
    
    app->model_manager = model_manager_create();
    app->is_running = 0;
    app->last_update_time = 0.0;
    app->show_grid = 1;
    app->show_forces = 1;
    app->show_streamlines = 0;
    app->show_heat_map = 0;
    
    app->sim_params.airspeed = 150.0;
    app->sim_params.altitude = 0.0;
    app->sim_params.angle_of_attack = 0.0;
    app->sim_params.pressure = 101.325;
    app->sim_params.temperature = 288.15;
    app->sim_params.turbulence_intensity = 5.0;
    app->sim_params.viscosity = 1.8e-5;
    app->sim_params.characteristic_length = 1.0;
    
    app->units.altitude_unit = UNIT_ALT_FEET;
    app->units.speed_unit = UNIT_MPH;
    
    atmosphere_update(&app->atmosphere, app->sim_params.altitude);
    
    app->forces.lift = 0.0;
    app->forces.drag = 0.0;
    app->forces.side_force = 0.0;
    app->forces.lift_coefficient = 0.0;
    app->forces.drag_coefficient = 0.0;
    app->forces.pitch_moment = 0.0;
    
    app->heat.heat_flux = 0.0;
    app->heat.surface_temperature = 288.15;
    app->heat.convective_coeff = 0.0;
    app->heat.stagnation_temp = 0.0;
    app->heat.thermal_load = 0.0;
    
    return app;
}

void cfd_app_destroy(CFDApp *app) {
    if (!app) return;
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
    
    double alt_meters;
    if (app->units.altitude_unit == UNIT_ALT_FEET) {
        alt_meters = app->sim_params.altitude * 0.3048;
    } else {
        alt_meters = app->sim_params.altitude;
    }
    
    atmosphere_update(&app->atmosphere, alt_meters);
    
    double speed_ms;
    if (app->units.speed_unit == UNIT_MPH) {
        speed_ms = app->sim_params.airspeed * 0.44704;
    } else if (app->units.speed_unit == UNIT_MACH) {
        speed_ms = app->sim_params.airspeed * app->atmosphere.sound_speed;
    } else {
        speed_ms = app->sim_params.airspeed;
    }
    
    app->atmosphere.mach_number = speed_ms / app->atmosphere.sound_speed;
    app->atmosphere.reynolds_number = (
        app->atmosphere.density * speed_ms * 
        app->sim_params.characteristic_length / 
        app->sim_params.viscosity
    );
    app->atmosphere.dynamic_pressure = 0.5 * app->atmosphere.density * speed_ms * speed_ms;
}

void cfd_app_import_model(CFDApp *app, const char *filepath) {
    if (!app || !filepath) return;
    
    const char *ext = strrchr(filepath, '.');
    if (!ext) return;
    
    CFDMesh mesh = {0};
    
    if (strcasecmp(ext, ".obj") == 0) {
        // obj_loader_load(filepath, &mesh);
    } else if (strcasecmp(ext, ".stl") == 0) {
        // stl_loader_load(filepath, &mesh);
    }
    if (app->model_manager) {
        // model_manager_add_mesh(app->model_manager, &mesh);
    }
}

void cfd_app_calculate(CFDApp *app) {
    if (!app) return;
    
    cfd_app_on_param_changed(app);
}

#endif
