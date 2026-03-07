#ifndef PROPERTIES_PANEL_H
#define PROPERTIES_PANEL_H

#include <gtk/gtk.h>

typedef struct PropertiesPanel PropertiesPanel;

typedef enum {
    REP_SURFACE,
    REP_WIREFRAME,
    REP_POINTS,
    REP_SURFACE_WITH_WIREFRAME
} RepresentationType;

typedef enum {
    COLOR_MAP_PRESSURE,
    COLOR_MAP_VELOCITY,
    COLOR_MAP_TEMPERATURE,
    COLOR_MAP_DENSITY,
    COLOR_MAP_CUSTOM
} ColormapType;

struct PropertiesPanel {
    GtkWidget *notebook;
    GtkWidget *display_tab;
    GtkWidget *data_tab;
    GtkWidget *filters_tab;
    
    GtkWidget *rep_combo;
    GtkWidget *opacity_scale;
    GtkWidget *color_map_combo;
    GtkWidget *min_value_entry;
    GtkWidget *max_value_entry;
    GtkWidget *show_edges_check;
    GtkWidget *lighting_check;
    
    int selected_object_id;
    void (*on_representation_changed)(void*, RepresentationType);
    void (*on_opacity_changed)(void*, double);
    void (*on_colormap_changed)(void*, ColormapType);
    void (*on_range_changed)(void*, double, double);
    void *user_data;
};

PropertiesPanel* properties_panel_create(void);
void properties_panel_destroy(PropertiesPanel *pp);
void properties_panel_set_object(PropertiesPanel *pp, int object_id);
void properties_panel_set_representation(PropertiesPanel *pp, RepresentationType rep);
void properties_panel_set_opacity(PropertiesPanel *pp, double opacity);
void properties_panel_set_colormap(PropertiesPanel *pp, ColormapType cmap);
void properties_panel_set_range(PropertiesPanel *pp, double min_val, double max_val);
void properties_panel_attach_callbacks(PropertiesPanel *pp,
    void (*on_representation_changed)(void*, RepresentationType),
    void (*on_opacity_changed)(void*, double),
    void (*on_colormap_changed)(void*, ColormapType),
    void (*on_range_changed)(void*, double, double),
    void *user_data);

#endif
