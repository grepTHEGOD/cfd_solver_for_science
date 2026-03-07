#ifndef CFD_APP_H
#define CFD_APP_H

#include <gtk/gtk.h>
#include "ui/main_window.h"
#include "models/model_manager.h"
#include "physics/atmosphere.h"

typedef struct {
    GtkApplication *app;
    MainWindow *main_window;
    ModelManager *model_manager;
    AtmosphereData atmosphere;
    SimulationParams sim_params;
    UnitSettings units;
    int is_running;
    double last_update_time;
} CFDApp;

CFDApp* cfd_app_create(void);
void cfd_app_destroy(CFDApp *app);
void cfd_app_run(CFDApp *app);
void cfd_app_update(CFDApp *app, double delta_time);
void cfd_app_on_param_changed(CFDApp *app);

#endif
