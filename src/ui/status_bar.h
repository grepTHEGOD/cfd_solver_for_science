#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include <gtk/gtk.h>

typedef struct StatusBar StatusBar;

struct StatusBar {
    GtkWidget *hbox;
    GtkWidget *unit_label;
    GtkWidget *status_label;
    GtkWidget *fps_label;
    GtkWidget *vertices_label;
    GtkWidget *faces_label;
    GtkWidget *progress_bar;
    GtkWidget *sim_time_label;
};

StatusBar* status_bar_create(void);
void status_bar_destroy(StatusBar *sb);
void status_bar_set_unit(StatusBar *sb, const char *unit);
void status_bar_set_status(StatusBar *sb, const char *status);
void status_bar_set_fps(StatusBar *sb, double fps);
void status_bar_set_mesh_info(StatusBar *sb, int vertices, int faces);
void status_bar_set_progress(StatusBar *sb, double progress);
void status_bar_set_sim_time(StatusBar *sb, double time);

#endif
