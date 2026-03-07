#ifndef MAIN_WINDOW_C
#define MAIN_WINDOW_C

#include "main_window.h"
#include <stdio.h>
#include <stdlib.h>

static void on_window_destroy(GtkWidget *widget, gpointer data) {
    (void)widget;
    (void)data;
    gtk_main_quit();
}

static void setup_css_provider(GtkWidget *window) {
    GtkCssProvider *provider = gtk_css_provider_new();
    const char *css = 
        "window { background: #1E1E1E; }"
        "frame { border: 1px solid #3E3E42; }"
        "label { color: #CCCCCC; }"
        "button { background: #3E3E42; color: #CCCCCC; }"
        "button:hover { background: #4E4E52; }"
        "entry { background: #3C3C3C; color: #CCCCCC; border: 1px solid #555; }"
        "treeview { background: #252526; color: #CCCCCC; }"
        "slider { -gtk-slider-filler: #007ACC; }"
        "scale { color: #858585; }"
        ".panel { background: #252526; border: 1px solid #3E3E42; }"
        ".accent { color: #007ACC; }"
        ".toolbar { background: #2D2D30; }";
    
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
}

MainWindow* main_window_create(GtkApplication *app) {
    MainWindow *win = (MainWindow*)calloc(1, sizeof(MainWindow));
    if (!win) return NULL;
    
    win->app = app;
    
    win->window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(win->window), "CFD Studio - Fluid Dynamics Visualization");
    gtk_window_set_default_size(GTK_WINDOW(win->window), 1600, 900);
    gtk_window_set_position(GTK_WINDOW(win->window), GTK_WIN_POS_CENTER);
    
    setup_css_provider(win->window);
    
    win->header_bar = gtk_header_bar_new();
    gtk_header_bar_set_title(GTK_HEADER_BAR(win->header_bar), "CFD Studio");
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(win->header_bar), TRUE);
    gtk_window_set_titlebar(GTK_WINDOW(win->window), win->header_bar);
    
    win->main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(win->window), win->main_box);
    
    win->paned_h = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_paned_set_position(GTK_PANED(win->paned_h), 250);
    gtk_box_pack_start(GTK_BOX(win->main_box), win->paned_h, TRUE, TRUE, 0);
    
    win->paned_v = gtk_paned_new(GTK_ORIENTATION_VERTICAL);
    gtk_paned_set_position(GTK_PANED(win->paned_v), 650);
    gtk_paned_add1(GTK_PANED(win->paned_h), win->paned_v);
    
    g_signal_connect(win->window, "destroy", G_CALLBACK(on_window_destroy), NULL);
    
    gtk_widget_show_all(win->window);
    
    return win;
}

void main_window_destroy(MainWindow *win) {
    if (!win) return;
    gtk_widget_destroy(win->window);
    free(win);
}

void main_window_set_viewport_widget(MainWindow *win, GtkWidget *viewport) {
    if (!win || !viewport) return;
    win->viewport = viewport;
}
