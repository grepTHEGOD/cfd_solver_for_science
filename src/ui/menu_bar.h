#ifndef MENU_BAR_H
#define MENU_BAR_H

#include <gtk/gtk.h>

typedef struct MenuBar {
    GtkWidget *menubar;
    GtkWidget *file_menu;
    GtkWidget *view_menu;
    GtkWidget *viz_menu;
    GtkWidget *help_menu;
    void (*import_callback)(void*);
    void (*export_callback)(void*);
    void *user_data;
} MenuBar;

MenuBar* menu_bar_create(void);
void menu_bar_destroy(MenuBar *menubar);
void menu_bar_attach_callbacks(MenuBar *menubar, 
    void (*on_import)(void*),
    void (*on_export)(void*),
    void *user_data);

#endif
