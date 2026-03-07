#ifndef MENU_BAR_C
#define MENU_BAR_C

#include "menu_bar.h"
#include <stdio.h>
#include <stdlib.h>

static void on_import_activate(GtkMenuItem *item, gpointer data) {
    (void)item;
    MenuBar *mb = (MenuBar*)data;
    if (mb && mb->import_callback) {
        mb->import_callback(mb->user_data);
    }
}

static void on_export_activate(GtkMenuItem *item, gpointer data) {
    (void)item;
    MenuBar *mb = (MenuBar*)data;
    if (mb && mb->export_callback) {
        mb->export_callback(mb->user_data);
    }
}

static void on_quit_activate(GtkMenuItem *item, gpointer data) {
    (void)item;
    (void)data;
    gtk_main_quit();
}

MenuBar* menu_bar_create(void) {
    MenuBar *mb = (MenuBar*)calloc(1, sizeof(MenuBar));
    if (!mb) return NULL;
    
    mb->menubar = gtk_menu_bar_new();
    
    mb->file_menu = gtk_menu_new();
    GtkWidget *file_item = gtk_menu_item_new_with_label("File");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item), mb->file_menu);
    
    GtkWidget *import_item = gtk_menu_item_new_with_label("Import Model...");
    g_signal_connect(import_item, "activate", G_CALLBACK(on_import_activate), mb);
    gtk_menu_shell_append(GTK_MENU_SHELL(mb->file_menu), import_item);
    
    GtkWidget *export_item = gtk_menu_item_new_with_label("Export Image...");
    g_signal_connect(export_item, "activate", G_CALLBACK(on_export_activate), mb);
    gtk_menu_shell_append(GTK_MENU_SHELL(mb->file_menu), export_item);
    
    gtk_menu_shell_append(GTK_MENU_SHELL(mb->file_menu), gtk_separator_menu_item_new());
    
    GtkWidget *quit_item = gtk_menu_item_new_with_label("Quit");
    g_signal_connect(quit_item, "activate", G_CALLBACK(on_quit_activate), mb);
    gtk_menu_shell_append(GTK_MENU_SHELL(mb->file_menu), quit_item);
    
    gtk_menu_shell_append(GTK_MENU_SHELL(mb->menubar), file_item);
    
    mb->view_menu = gtk_menu_new();
    GtkWidget *view_item = gtk_menu_item_new_with_label("View");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(view_item), mb->view_menu);
    
    gtk_menu_shell_append(GTK_MENU_SHELL(mb->menubar), view_item);
    
    mb->viz_menu = gtk_menu_new();
    GtkWidget *viz_item = gtk_menu_item_new_with_label("Visualization");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(viz_item), mb->viz_menu);
    
    gtk_menu_shell_append(GTK_MENU_SHELL(mb->menubar), viz_item);
    
    mb->help_menu = gtk_menu_new();
    GtkWidget *help_item = gtk_menu_item_new_with_label("Help");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(help_item), mb->help_menu);
    
    gtk_menu_shell_append(GTK_MENU_SHELL(mb->menubar), help_item);
    
    return mb;
}

void menu_bar_destroy(MenuBar *menubar) {
    if (!menubar) return;
    gtk_widget_destroy(menubar->menubar);
    free(menubar);
}

void menu_bar_attach_callbacks(MenuBar *menubar, 
    void (*on_import)(void*),
    void (*on_export)(void*),
    void *user_data) {
    if (!menubar) return;
    menubar->import_callback = on_import;
    menubar->export_callback = on_export;
    menubar->user_data = user_data;
}
