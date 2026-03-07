#ifndef OBJECT_BROWSER_C
#define OBJECT_BROWSER_C

#include "object_browser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void on_tree_selection_changed(GtkTreeSelection *selection, gpointer data) {
    (void)selection;
    ObjectBrowser *ob = (ObjectBrowser*)data;
    if (!ob || !ob->on_select) return;
    
    GtkTreeModel *model;
    GtkTreeIter iter;
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        int *id_ptr = NULL;
        gtk_tree_model_get(model, &iter, 1, &id_ptr, -1);
        if (id_ptr) {
            ob->on_select(ob->user_data, *id_ptr);
        }
    }
}

static void on_cell_toggled(GtkCellRendererToggle *cell, gchar *path_str, gpointer data) {
    (void)cell;
    ObjectBrowser *ob = (ObjectBrowser*)data;
    if (!ob || !ob->on_toggle_visibility) return;
    
    GtkTreeModel *model = GTK_TREE_MODEL(ob->tree_store);
    GtkTreePath *path = gtk_tree_path_new_from_string(path_str);
    GtkTreeIter iter;
    
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        int *id_ptr = NULL;
        gboolean visible = FALSE;
        gtk_tree_model_get(model, &iter, 1, &id_ptr, 2, &visible, -1);
        
        if (id_ptr) {
            gtk_tree_store_set(ob->tree_store, &iter, 2, !visible, -1);
            ob->on_toggle_visibility(ob->user_data, *id_ptr, !visible);
        }
    }
    gtk_tree_path_free(path);
}

ObjectBrowser* object_browser_create(void) {
    ObjectBrowser *ob = (ObjectBrowser*)calloc(1, sizeof(ObjectBrowser));
    if (!ob) return NULL;
    
    ob->scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(ob->scrolled_window),
        GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    
    ob->tree_store = gtk_tree_store_new(3, G_TYPE_STRING, G_TYPE_INT, G_TYPE_BOOLEAN);
    ob->tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(ob->tree_store));
    
    GtkCellRenderer *renderer = gtk_cell_renderer_toggle_new();
    g_signal_connect(renderer, "toggled", G_CALLBACK(on_cell_toggled), ob);
    GtkTreeViewColumn *col = gtk_tree_view_column_new_with_attributes("Visible", renderer, "active", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(ob->tree_view), col);
    
    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("Objects", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(ob->tree_view), col);
    
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(ob->tree_view));
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
    g_signal_connect(selection, "changed", G_CALLBACK(on_tree_selection_changed), ob);
    
    gtk_container_add(GTK_CONTAINER(ob->scrolled_window), ob->tree_view);
    gtk_widget_set_size_request(ob->scrolled_window, 250, -1);
    gtk_widget_show_all(ob->scrolled_window);
    
    return ob;
}

void object_browser_destroy(ObjectBrowser *ob) {
    if (!ob) return;
    g_list_free(ob->objects);
    gtk_widget_destroy(ob->scrolled_window);
    free(ob);
}

void object_browser_add_object(ObjectBrowser *ob, const char *name, ObjectType type) {
    if (!ob || !name) return;
    
    BrowserObject *obj = (BrowserObject*)malloc(sizeof(BrowserObject));
    strncpy(obj->name, name, sizeof(obj->name) - 1);
    obj->type = type;
    obj->visible = TRUE;
    obj->selected = FALSE;
    obj->id = ob->next_id++;
    
    ob->objects = g_list_append(ob->objects, obj);
    
    GtkTreeIter iter;
    gtk_tree_store_append(ob->tree_store, &iter, NULL);
    gtk_tree_store_set(ob->tree_store, &iter,
        0, name,
        1, obj->id,
        2, TRUE, -1);
}

void object_browser_remove_object(ObjectBrowser *ob, int object_id) {
    (void)ob;
    (void)object_id;
}

void object_browser_set_visible(ObjectBrowser *ob, int object_id, gboolean visible) {
    (void)ob;
    (void)object_id;
    (void)visible;
}

void object_browser_set_selected(ObjectBrowser *ob, int object_id, gboolean selected) {
    (void)ob;
    (void)object_id;
    (void)selected;
}

void object_browser_attach_callbacks(ObjectBrowser *ob,
    void (*on_select)(void*, int),
    void (*on_toggle_visibility)(void*, int, gboolean),
    void *user_data) {
    if (!ob) return;
    ob->on_select = on_select;
    ob->on_toggle_visibility = on_toggle_visibility;
    ob->user_data = user_data;
}

#endif
