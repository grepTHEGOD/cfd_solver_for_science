#ifndef GL_VIEWPORT_H
#define GL_VIEWPORT_H

#include <gtk/gtk.h>
#include <GL/glew.h>
#include "camera.h"
#include "mesh_renderer.h"

typedef struct GLViewport GLViewport;

struct GLViewport {
    GtkWidget *drawing_area;
    GdkGLContext *gl_context;
    GdkWindow *gl_window;
    
    Camera camera;
    MeshRenderer renderer;
    
    int width;
    int height;
    
    gboolean show_grid;
    gboolean show_axes;
    gboolean show_edges;
    gboolean lighting_enabled;
    
    float background_color[4];
};

GLViewport* gl_viewport_create(void);
void gl_viewport_destroy(GLViewport *viewport);
void gl_viewport_realize(GLViewport *viewport);
void gl_viewport_unrealize(GLViewport *viewport);
void gl_viewport_resize(GLViewport *viewport, int width, int height);
void gl_viewport_render(GLViewport *viewport);
void gl_viewport_set_mesh(GLViewport *viewport, Mesh *mesh);
void gl_viewport_set_background(GLViewport *viewport, float r, float g, float b, float a);
void gl_viewport_set_show_grid(GLViewport *viewport, gboolean show);
void gl_viewport_set_show_axes(GLViewport *viewport, gboolean show);
void gl_viewport_set_show_edges(GLViewport *viewport, gboolean show);
void gl_viewport_set_lighting(GLViewport *viewport, gboolean enabled);
void gl_viewport_camera_rotate(GLViewport *viewport, float dx, float dy);
void gl_viewport_camera_pan(GLViewport *viewport, float dx, float dy);
void gl_viewport_camera_zoom(GLViewport *viewport, float delta);
Camera* gl_viewport_get_camera(GLViewport *viewport);

#endif
