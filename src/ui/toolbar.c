#ifndef TOOLBAR_C
#define TOOLBAR_C

#include "toolbar.h"
#include <stdio.h>
#include <stdlib.h>

static void on_import_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    Toolbar *tb = (Toolbar*)data;
    if (tb && tb->on_import) {
        tb->on_import(tb->user_data);
    }
}

static void on_play_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    Toolbar *tb = (Toolbar*)data;
    if (tb && tb->on_play) {
        tb->on_play(tb->user_data);
    }
}

static void on_pause_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    Toolbar *tb = (Toolbar*)data;
    if (tb && tb->on_pause) {
        tb->on_pause(tb->user_data);
    }
}

static void on_reset_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    Toolbar *tb = (Toolbar*)data;
    if (tb && tb->on_reset) {
        tb->on_reset(tb->user_data);
    }
}

static void on_screenshot_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    (void)data;
    printf("Screenshot requested\n");
}

Toolbar* toolbar_create(void) {
    Toolbar *tb = (Toolbar*)calloc(1, sizeof(Toolbar));
    if (!tb) return NULL;
    
    tb->toolbar = gtk_toolbar_new();
    gtk_toolbar_set_style(GTK_TOOLBAR(tb->toolbar), GTK_TOOLBAR_BOTH_HORIZ);
    gtk_toolbar_set_icon_size(GTK_TOOLBAR(tb->toolbar), GTK_ICON_SIZE_SMALL_TOOLBAR);
    
    tb->btn_import = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(tb->btn_import), "Import");
    gtk_toolbar_insert(GTK_TOOLBAR(tb->toolbar), tb->btn_import, -1);
    g_signal_connect(tb->btn_import, "clicked", G_CALLBACK(on_import_clicked), tb);
    
    tb->btn_export = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(tb->btn_export), "Export");
    gtk_toolbar_insert(GTK_TOOLBAR(tb->toolbar), tb->btn_export, -1);
    
    gtk_toolbar_insert(GTK_TOOLBAR(tb->toolbar), gtk_separator_tool_item_new(), -1);
    
    tb->btn_play = gtk_tool_button_new_from_stock(GTK_STOCK_MEDIA_PLAY);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(tb->btn_play), "Play");
    gtk_toolbar_insert(GTK_TOOLBAR(tb->toolbar), tb->btn_play, -1);
    g_signal_connect(tb->btn_play, "clicked", G_CALLBACK(on_play_clicked), tb);
    
    tb->btn_pause = gtk_tool_button_new_from_stock(GTK_STOCK_MEDIA_PAUSE);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(tb->btn_pause), "Pause");
    gtk_toolbar_insert(GTK_TOOLBAR(tb->toolbar), tb->btn_pause, -1);
    g_signal_connect(tb->btn_pause, "clicked", G_CALLBACK(on_pause_clicked), tb);
    
    tb->btn_reset = gtk_tool_button_new_from_stock(GTK_STOCK_REFRESH);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(tb->btn_reset), "Reset");
    gtk_toolbar_insert(GTK_TOOLBAR(tb->toolbar), tb->btn_reset, -1);
    g_signal_connect(tb->btn_reset, "clicked", G_CALLBACK(on_reset_clicked), tb);
    
    gtk_toolbar_insert(GTK_TOOLBAR(tb->toolbar), gtk_separator_tool_item_new(), -1);
    
    tb->btn_screenshot = gtk_tool_button_new_from_stock(GTK_STOCK_FLOPPY);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(tb->btn_screenshot), "Screenshot");
    gtk_toolbar_insert(GTK_TOOLBAR(tb->toolbar), tb->btn_screenshot, -1);
    g_signal_connect(tb->btn_screenshot, "clicked", G_CALLBACK(on_screenshot_clicked), tb);
    
    gtk_widget_show_all(tb->toolbar);
    
    return tb;
}

void toolbar_destroy(Toolbar *tb) {
    if (!tb) return;
    gtk_widget_destroy(tb->toolbar);
    free(tb);
}

void toolbar_attach_callbacks(Toolbar *tb,
    void (*on_import)(void*),
    void (*on_play)(void*),
    void (*on_pause)(void*),
    void (*on_reset)(void*),
    void *user_data) {
    if (!tb) return;
    tb->on_import = on_import;
    tb->on_play = on_play;
    tb->on_pause = on_pause;
    tb->on_reset = on_reset;
    tb->user_data = user_data;
}

#endif
