#include "cfd_app.h"
#include "ui/gui_framework.h"
#include "ui/control_panel.h"
#include "viewport/viewport_3d.h"
#include "models/model_manager.h"
#include "models/obj_loader.h"
#include "models/stl_loader.h"
#include "physics/units.h"
#include "physics/atmosphere.h"
#include "physics/forces.h"
#include "physics/heat_transfer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    CFDApp *app;
    Viewport3D *viewport;
    ControlPanel *control_panel;
    Rectangle toolbar_bounds;
    Rectangle left_panel_bounds;
    Rectangle right_panel_bounds;
    Button import_btn;
    Button grid_btn;
    Button axis_btn;
    Button wireframe_btn;
    Button reset_btn;
    bool show_results_panel;
    Rectangle results_bounds;
} AppState;

static void init_app_state(AppState *state, CFDApp *app) {
    int screen_width = GetScreenWidth();
    int screen_height = GetScreenHeight();
    
    state->app = app;
    state->toolbar_bounds = (Rectangle){0, 0, screen_width, 50};
    state->left_panel_bounds = (Rectangle){0, 50, 250, screen_height - 300};
    state->right_panel_bounds = (Rectangle){screen_width - 300, 50, 300, screen_height - 300};
    
    Rectangle viewport_bounds = {
        state->left_panel_bounds.x + state->left_panel_bounds.width,
        state->toolbar_bounds.height,
        screen_width - state->left_panel_bounds.width - state->right_panel_bounds.width,
        screen_height - state->toolbar_bounds.height - 250
    };
    
    state->viewport = viewport_3d_create(viewport_bounds);
    
    Rectangle control_bounds = {
        0,
        screen_height - 250,
        screen_width,
        250
    };
    state->control_panel = control_panel_create(control_bounds);
    
    state->import_btn = gui_button((Rectangle){10, 10, 100, 30}, "Import");
    state->grid_btn = gui_button((Rectangle){120, 10, 80, 30}, "Grid");
    state->axis_btn = gui_button((Rectangle){210, 10, 80, 30}, "Axis");
    state->wireframe_btn = gui_button((Rectangle){300, 10, 100, 30}, "Wireframe");
    state->reset_btn = gui_button((Rectangle){410, 10, 80, 30}, "Reset");
    
    state->show_results_panel = false;
    state->results_bounds = (Rectangle){screen_width - 300, 50, 300, 400};
}

static void draw_toolbar(AppState *state) {
    DrawRectangleRec(state->toolbar_bounds, (Color){45, 45, 48, 255});
    DrawLine(0, 50, GetScreenWidth(), 50, (Color){62, 62, 66, 255});
    
    DrawText("CFD Studio", 15, 15, 20, (Color){0, 122, 204, 255});
    
    if (gui_button_draw(&state->import_btn)) {
        char filePath[512] = {0};
        if (FileExists("test.obj")) {
            printf("Found test.obj\n");
        }
        printf("Import model from File menu or drag & drop\n");
    }
    
    if (gui_button_draw(&state->grid_btn)) {
        viewport_3d_toggle_grid(state->viewport);
    }
    
    if (gui_button_draw(&state->axis_btn)) {
        viewport_3d_toggle_axis(state->viewport);
    }
    
    if (gui_button_draw(&state->wireframe_btn)) {
        viewport_3d_toggle_wireframe(state->viewport);
    }
    
    if (gui_button_draw(&state->reset_btn)) {
        viewport_3d_reset_camera(state->viewport);
    }
}

static void draw_left_panel(AppState *state) {
    DrawRectangleRec(state->left_panel_bounds, (Color){37, 37, 38, 255});
    DrawRectangleLinesEx(state->left_panel_bounds, 1, (Color){62, 62, 66, 255});
    
    DrawText("Object Browser", (int)(state->left_panel_bounds.x + 10), 
             (int)(state->left_panel_bounds.y + 10), 14, (Color){204, 204, 204, 255});
    
    DrawLine(state->left_panel_bounds.x, state->left_panel_bounds.y + 35,
             state->left_panel_bounds.x + state->left_panel_bounds.width,
             state->left_panel_bounds.y + 35, (Color){62, 62, 66, 255});
    
    DrawText("No model loaded", (int)(state->left_panel_bounds.x + 10), 
             (int)(state->left_panel_bounds.y + 50), 12, (Color){133, 133, 133, 255});
    
    DrawText("Import a 3D model (OBJ/STL)", (int)(state->left_panel_bounds.x + 10), 
             (int)(state->left_panel_bounds.y + 70), 11, (Color){133, 133, 133, 255});
    DrawText("to begin analysis", (int)(state->left_panel_bounds.x + 10), 
             (int)(state->left_panel_bounds.y + 85), 11, (Color){133, 133, 133, 255});
}

static void draw_right_panel(AppState *state) {
    DrawRectangleRec(state->right_panel_bounds, (Color){37, 37, 38, 255});
    DrawRectangleLinesEx(state->right_panel_bounds, 1, (Color){62, 62, 66, 255});
    
    DrawText("Analysis Results", (int)(state->right_panel_bounds.x + 10), 
             (int)(state->right_panel_bounds.y + 10), 14, (Color){204, 204, 204, 255});
    
    DrawLine(state->right_panel_bounds.x, state->right_panel_bounds.y + 35,
             state->right_panel_bounds.x + state->right_panel_bounds.width,
             state->right_panel_bounds.y + 35, (Color){62, 62, 66, 255});
    
    char buf[128];
    float y = state->right_panel_bounds.y + 50;
    
    DrawText("Forces", (int)(state->right_panel_bounds.x + 10), (int)y, 12, (Color){0, 122, 204, 255});
    y += 20;
    
    snprintf(buf, sizeof(buf), "Lift: %.2f N", state->app->forces.lift);
    DrawText(buf, (int)(state->right_panel_bounds.x + 10), (int)y, 12, (Color){204, 204, 204, 255});
    y += 18;
    
    snprintf(buf, sizeof(buf), "Drag: %.2f N", state->app->forces.drag);
    DrawText(buf, (int)(state->right_panel_bounds.x + 10), (int)y, 12, (Color){204, 204, 204, 255});
    y += 18;
    
    snprintf(buf, sizeof(buf), "Side: %.2f N", state->app->forces.side_force);
    DrawText(buf, (int)(state->right_panel_bounds.x + 10), (int)y, 12, (Color){204, 204, 204, 255});
    y += 25;
    
    snprintf(buf, sizeof(buf), "Cl: %.4f", state->app->forces.lift_coefficient);
    DrawText(buf, (int)(state->right_panel_bounds.x + 10), (int)y, 12, (Color){78, 201, 176, 255});
    y += 18;
    
    snprintf(buf, sizeof(buf), "Cd: %.4f", state->app->forces.drag_coefficient);
    DrawText(buf, (int)(state->right_panel_bounds.x + 10), (int)y, 12, (Color){78, 201, 176, 255});
    y += 30;
    
    DrawText("Heat Transfer", (int)(state->right_panel_bounds.x + 10), (int)y, 12, (Color){0, 122, 204, 255});
    y += 20;
    
    snprintf(buf, sizeof(buf), "Heat Flux: %.2f W/m²", state->app->heat.heat_flux);
    DrawText(buf, (int)(state->right_panel_bounds.x + 10), (int)y, 12, (Color){204, 204, 204, 255});
    y += 18;
    
    snprintf(buf, sizeof(buf), "Surface Temp: %.1f K", state->app->heat.surface_temperature);
    DrawText(buf, (int)(state->right_panel_bounds.x + 10), (int)y, 12, (Color){204, 204, 204, 255});
    y += 18;
    
    snprintf(buf, sizeof(buf), "Stagnation T: %.1f K", state->app->heat.stagnation_temp);
    DrawText(buf, (int)(state->right_panel_bounds.x + 10), (int)y, 12, (Color){204, 204, 204, 255});
    y += 18;
    
    snprintf(buf, sizeof(buf), "Thermal Load: %.2f W", state->app->heat.thermal_load);
    DrawText(buf, (int)(state->right_panel_bounds.x + 10), (int)y, 12, (Color){204, 204, 204, 255});
}

static void update_physics(AppState *state) {
    CFDApp *app = state->app;
    ControlPanel *cp = state->control_panel;
    
    float airspeed, altitude, distance, fuel, aoa;
    control_panel_get_values(cp, &airspeed, &altitude, &distance, &fuel, &aoa);
    
    app->sim_params.airspeed = airspeed;
    app->sim_params.altitude = altitude;
    app->sim_params.angle_of_attack = aoa;
    
    double alt_m = (cp->altitude_unit == UNIT_ALT_FEET) ? 
        altitude * 0.3048 : altitude;
    
    atmosphere_update(&app->atmosphere, alt_m);
    
    double speed_ms;
    if (cp->speed_unit == UNIT_MPH) {
        speed_ms = airspeed * 0.44704;
    } else if (cp->speed_unit == UNIT_MACH) {
        speed_ms = airspeed * app->atmosphere.sound_speed;
    } else {
        speed_ms = airspeed;
    }
    
    app->atmosphere.mach_number = speed_ms / app->atmosphere.sound_speed;
    app->atmosphere.reynolds_number = app->atmosphere.density * speed_ms * 
        app->sim_params.characteristic_length / app->sim_params.viscosity;
    app->atmosphere.dynamic_pressure = 0.5 * app->atmosphere.density * speed_ms * speed_ms;
    
    AerodynamicForces af;
    af.velocity = speed_ms;
    af.density = app->atmosphere.density;
    af.area = 1.0;
    af.chord = 1.0;
    af.span = 1.0;
    af.aoa = aoa * 3.14159 / 180.0;
    af.mach = app->atmosphere.mach_number;
    af.reynolds = app->atmosphere.reynolds_number;
    af.cl = 0.0;
    af.cd = 0.0;
    af.cm = 0.0;
    
    forces_compute(&af, &app->forces);
    
    app->forces.lift = app->atmosphere.dynamic_pressure * af.area * app->forces.lift_coefficient;
    app->forces.drag = app->atmosphere.dynamic_pressure * af.area * app->forces.drag_coefficient;
    
    HeatParams hp;
    hp.velocity = speed_ms;
    hp.density = app->atmosphere.density;
    hp.temperature = app->atmosphere.temperature;
    hp.mach = app->atmosphere.mach_number;
    hp.surface_area = 1.0;
    hp.characteristic_length = app->sim_params.characteristic_length;
    hp.gamma = 1.4;
    hp.cp = 1005.0;
    hp.prandtl = 0.71;
    
    heat_transfer_compute(&hp, &app->heat);
    
    control_panel_set_derived(cp, 
        app->atmosphere.mach_number,
        app->atmosphere.reynolds_number,
        app->atmosphere.density,
        app->atmosphere.dynamic_pressure,
        app->atmosphere.sound_speed
    );
}

int main(void) {
    const int screen_width = 1600;
    const int screen_height = 900;
    
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(screen_width, screen_height, "CFD Studio");
    
    SetTargetFPS(60);
    
    CFDApp *app = cfd_app_create();
    if (!app) {
        CloseWindow();
        return 1;
    }
    
    AppState state;
    init_app_state(&state, app);
    
    cfd_app_on_param_changed(app);
    update_physics(&state);
    
    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();
        
        control_panel_update(state.control_panel);
        viewport_3d_update(state.viewport);
        
        update_physics(&state);
        
        BeginDrawing();
        ClearBackground((Color){30, 30, 35, 255});
        
        draw_toolbar(&state);
        draw_left_panel(&state);
        draw_right_panel(&state);
        control_panel_draw(state.control_panel);
        viewport_3d_draw(state.viewport);
        
        DrawText("Controls:", 15, 620, 12, (Color){133, 133, 133, 255});
        DrawText("Left Mouse - Rotate", 15, 635, 11, (Color){133, 133, 133, 255});
        DrawText("Middle Mouse - Pan", 15, 650, 11, (Color){133, 133, 133, 255});
        DrawText("Scroll - Zoom", 15, 665, 11, (Color){133, 133, 133, 255});
        
        EndDrawing();
    }
    
    viewport_3d_destroy(state.viewport);
    control_panel_destroy(state.control_panel);
    cfd_app_destroy(app);
    
    CloseWindow();
    
    return 0;
}
