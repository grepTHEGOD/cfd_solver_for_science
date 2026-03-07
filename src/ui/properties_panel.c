#ifndef PROPERTIES_PANEL_C
#define PROPERTIES_PANEL_C

#include "properties_panel.h"
#include <stdio.h>
#include <stdlib.h>

static void on_representation_changed(GtkComboBox *widget, gpointer data) {
    (void)widget;
    PropertiesPanel *pp = (PropertiesPanel*)data;
    if (pp && pp->on_representation_changed) {
        RepresentationType rep = (RepresentationType)gtk_combo_box_get_active(widget);
        pp->on_representation_changed(pp->user_data, rep);
    }
}

static void on_opacity_changed(GtkRange *range, gpointer data) {
    (void)range;
    PropertiesPanel *pp = (PropertiesPanel*)data;
    if (pp && pp->on_opacity_changed) {
        double opacity = gtk_range_get_value(range);
        pp->on_opacity_changed(pp->user_data, opacity);
    }
}

static void on_colormap_changed(GtkComboBox *widget, gpointer data) {
    (void)widget;
    PropertiesPanel *pp = (PropertiesPanel*)data;
    if (pp && pp->on_colormap_changed) {
        ColormapType cmap = (ColormapType)gtk_combo_box_get_active(widget);
        pp->on_colormap_changed(pp->user_data, cmap);
    }
}

PropertiesPanel* properties_panel_create(void) {
    PropertiesPanel *pp = (PropertiesPanel*)calloc(1, sizeof(PropertiesPanel));
    if (!pp) return NULL;
    
    pp->notebook = gtk_notebook_new();
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(pp->notebook), GTK_POS_TOP);
    
    pp->display_tab = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(pp->display_tab), 8);
    gtk_grid_set_column_spacing(GTK_GRID(pp->display_tab), 8);
    
    GtkWidget *lbl = gtk_label_new("Representation:");
    pp->rep_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(pp->rep_combo), NULL, "Surface");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(pp->rep_combo), NULL, "Wireframe");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(pp->rep_combo), NULL, "Points");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(pp->rep_combo), NULL, "Surface + Wireframe");
    gtk_combo_box_set_active(GTK_COMBO_BOX(pp->rep_combo), 0);
    g_signal_connect(pp->rep_combo, "changed", G_CALLBACK(on_representation_changed), pp);
    gtk_grid_attach(GTK_GRID(pp->display_tab), lbl, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(pp->display_tab), pp->rep_combo, 1, 0, 1, 1);
    
    lbl = gtk_label_new("Opacity:");
    pp->opacity_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.0, 1.0, 0.1);
    gtk_scale_set_value_pos(GTK_SCALE(pp->opacity_scale), GTK_POS_RIGHT);
    gtk_range_set_value(GTK_RANGE(pp->opacity_scale), 1.0);
    g_signal_connect(pp->opacity_scale, "value-changed", G_CALLBACK(on_opacity_changed), pp);
    gtk_grid_attach(GTK_GRID(pp->display_tab), lbl, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(pp->display_tab), pp->opacity_scale, 1, 1, 1, 1);
    
    pp->show_edges_check = gtk_check_button_new_with_label("Show Edges");
    gtk_grid_attach(GTK_GRI D(pp->display_tab), pp->show_edges_check, 0, 2, 2, 1);
    
    pp->lighting_check = gtk_check_button_new_with_label("Enable Lighting");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pp->lighting_check), TRUE);
    gtk_grid_attach(GTK_GRID(pp->display_tab), pp->lighting_check, 0, 3, 2, 1);
    
    gtk_notebook_append_page(GTK_NOTEBOOK(pp->notebook), pp->display_tab, gtk_label_new("Display"));
    
    pp->data_tab = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(pp->data_tab), 8);
    gtk_grid_set_column_spacing(GTK_GRID(pp->data_tab), 8);
    
    lbl = gtk_label_new("Color Map:");
    pp->color_map_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(pp->color_map_combo), NULL, "Pressure");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(pp->color_map_combo), NULL, "Velocity");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(pp->color_map_combo), NULL, "Temperature");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(pp->color_map_combo), NULL, "Density");
    gtk_combo_box_set_active(GTK_COMBO_BOX(pp->color_map_combo), 0);
    g_signal_connect(pp->color_map_combo, "changed", G_CALLBACK(on_colormap_changed), pp);
    gtk_grid_attach(GTK_GRID(pp->data_tab), lbl, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(pp->data_tab), pp->color_map_combo, 1, 0, 1, 1);
    
    lbl = gtk_label_new("Min Value:");
    pp->min_value_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(pp->min_value_entry), "0.0");
    gtk_grid_attach(GTK_GRID(pp->data_tab), lbl, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(pp->data_tab), pp->min_value_entry, 1, 1, 1, 1);
    
    lbl = gtk_label_new("Max Value:");
    pp->max_value_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(pp->max_value_entry), "1.0");
    gtk_grid_attach(GTK_GRID(pp->data_tab), lbl, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(pp->data_tab), pp->max_value_entry, 1, 2, 1, 1);
    
    gtk_notebook_append_page(GTK_NOTEBOOK(pp->notebook), pp->data_tab, gtk_label_new("Data"));
    
    pp->filters_tab = gtk_label_new("Filters (Coming Soon)");
    gtk_notebook_append_page(GTK_NOTEBOOK(pp->notebook), pp->filters_tab, gtk_label_new("Filters"));
    
    gtk_widget_set_size_request(pp->notebook, 300, -1);
    gtk_widget_show_all(pp->notebook);
    
    return pp;
}

void properties_panel_destroy(PropertiesPanel *pp) {
    if (!pp) return;
    gtk_widget_destroy(pp->notebook);
    free(pp);
}

void properties_panel_set_object(PropertiesPanel *pp, int object_id) {
    if (!pp) return;
    pp->selected_object_id = object_id;
}

void properties_panel_set_representation(PropertiesPanel *pp, RepresentationType rep) {
    if (!pp) return;
    gtk_combo_box_set_active(GTK_COMBO_BOX(pp->rep_combo), (gint)rep);
}

void properties_panel_set_opacity(PropertiesPanel *pp, double opacity) {
    if (!pp) return;
    gtk_range_set_value(GTK_RANGE(pp->opacity_scale), opacity);
}

void properties_panel_set_colormap(PropertiesPanel *pp, ColormapType cmap) {
    if (!pp) return;
    gtk_combo_box_set_active(GTK_COMBO_BOX(pp->color_map_combo), (gint)cmap);
}

void properties_panel_set_range(PropertiesPanel *pp, double min_val, double max_val) {
    if (!pp) return;
    char buf[32];
    snprintf(buf, sizeof(buf), "%f", min_val);
    gtk_entry_set_text(GTK_ENTRY(pp->min_value_entry), buf);
    snprintf(buf, sizeof(buf), "%f", max_val);
    gtk_entry_set_text(GTK_ENTRY(pp->max_value_entry), buf);
}

void properties_panel_attach_callbacks(PropertiesPanel *pp,
    void (*on_representation_changed)(void*, RepresentationType),
    void (*on_opacity_changed)(void*, double),
    void (*on_colormap_changed)(void*, ColormapType),
    void (*on_range_changed)(void*, double, double),
    void *user_data) {
    if (!pp) return;
    pp->on_representation_changed = on_representation_changed;
    pp->on_opacity_changed = on_opacity_changed;
    pp->on_colormap_changed = on_colormap_changed;
    pp->on_range_changed = on_range_changed;
    pp->user_data = user_data;
}

#endif
