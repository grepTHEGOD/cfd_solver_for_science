#ifndef OBJECT_BROWSER_H
#define OBJECT_BROWSER_H

#include <gtk/gtk.h>

typedef struct ObjectBrowser ObjectBrowser;

typedef enum {
    OBJECT_TYPE_MESH,
    OBJECT_TYPE_CONTOUR,
    OBJECT_TYPE_VECTORS,
    OBJECT_TYPE_STREAMLINES,
    OBJECT_TYPE_FORCE_ARROW,
    OBJECT_TYPE_CUT_PLANE
} ObjectType;

typedef struct {
    char name[256];
    ObjectType type;
    gboolean visible;
    gboolean selected;
    int id;
} BrowserObject;

struct ObjectBrowser {
    GtkWidget *scrolled_window;
    GtkWidget *tree_view;
    GtkTreeStore *tree_store;
    GList *objects;
    int next_id;
};

ObjectBrowser* object_browser_create(void);
void object_browser_destroy(ObjectBrowser *ob);
void object_browser_add_object(ObjectBrowser *ob, const char *name, ObjectType type);
void object_browser_remove_object(ObjectBrowser *ob, int object_id);
void object_browser_set_visible(ObjectBrowser *ob, int object_id, gboolean visible);
void object_browser_set_selected(ObjectBrowser *ob, int object_id, gboolean selected);
void object_browser_attach_callbacks(ObjectBrowser *ob,
    void (*on_select)(void*, int),
    void (*on_toggle_visibility)(void*, int, gboolean),
    void *user_data);

#endif
