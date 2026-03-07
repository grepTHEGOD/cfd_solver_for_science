#ifndef VIEWPORT_3D_H
#define VIEWPORT_3D_H

#include "raylib.h"
#include <stdbool.h>

typedef struct {
    Rectangle bounds;
    Camera3D camera;
    bool show_grid;
    bool show_axis;
    bool wireframe_mode;
    float rotation_speed;
    float zoom_speed;
    Vector3 model_position;
    Vector3 model_rotation;
    float model_scale;
    Mesh *meshes;
    int mesh_count;
    Color background_color;
} Viewport3D;

Viewport3D* viewport_3d_create(Rectangle bounds);
void viewport_3d_destroy(Viewport3D *vp);
void viewport_3d_update(Viewport3D *vp);
void viewport_3d_draw(Viewport3D *vp);
void viewport_3d_load_mesh(Viewport3D *vp, Mesh mesh);
void viewport_3d_toggle_grid(Viewport3D *vp);
void viewport_3d_toggle_axis(Viewport3D *vp);
void viewport_3d_toggle_wireframe(Viewport3D *vp);
void viewport_3d_reset_camera(Viewport3D *vp);
void viewport_3d_set_background(Viewport3D *vp, Color color);

#endif
