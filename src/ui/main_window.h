#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <gtk/gtk.h>

typedef struct MainWindow MainWindow;

struct MainWindow {
    GtkApplication *app;
    GtkWidget *window;
    GtkWidget *header_bar;
    GtkWidget *main_box;
    GtkWidget *paned_h;      /* Horizontal paned for left/center/right */
    GtkWidget *paned_v;      /* Vertical paned for top/bottom */
    GtkWidget *menu_bar;
    GtkWidget *toolbar;
    GtkWidget *object_browser;
    GtkWidget *viewport;     /* OpenGL viewport widget */
    GtkWidget *properties;
    GtkWidget *control_panel;
    GtkWidget *status_bar;
};

MainWindow* main_window_create(GtkApplication *app);
void main_window_destroy(MainWindow *win);
void main_window_set_viewport_widget(MainWindow *win, GtkWidget *viewport);

#endif
