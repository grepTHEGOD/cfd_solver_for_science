#ifndef CFD_APP_H
#define CFD_APP_H

#include "ui/gui_framework.h"
#include "ui/control_panel.h"
#include "viewport/viewport_3d.h"
#include "models/model_manager.h"
#include "physics/atmosphere.h"
#include "physics/forces.h"
#include "physics/heat_transfer.h"

typedef struct {
    ModelManager *model_manager;
    AtmosphereData atmosphere;
    SimulationParams sim_params;
    UnitSettings units;
    ForceResult forces;
    HeatResult heat;
    int is_running;
    double last_update_time;
    int show_grid;
    int show_forces;
    int show_streamlines;
    int show_heat_map;
} CFDApp;

CFDApp* cfd_app_create(void);
void cfd_app_destroy(CFDApp *app);
void cfd_app_run(CFDApp *app);
void cfd_app_update(CFDApp *app, double delta_time);
void cfd_app_on_param_changed(CFDApp *app);
void cfd_app_import_model(CFDApp *app, const char *filepath);
void cfd_app_calculate(CFDApp *app);

#endif
