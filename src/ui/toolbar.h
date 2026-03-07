#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <gtk/gtk.h>

typedef struct Toolbar Toolbar;

struct Toolbar {
    GtkWidget *toolbar;
    GtkToolItem *btn_import;
    GtkToolItem *btn_export;
    GtkToolItem *btn_play;
    GtkToolItem *btn_pause;
    GtkToolItem *btn_reset;
    GtkToolItem *btn_screenshot;
};

Toolbar* toolbar_create(void);
void toolbar_destroy(Toolbar *tb);
void toolbar_attach_callbacks(Toolbar *tb,
    void (*on_import)(void*),
    void (*on_play)(void*),
    void (*on_pause)(void*),
    void (*on_reset)(void*),
    void *user_data);

#endif
