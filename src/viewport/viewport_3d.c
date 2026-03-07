#ifndef VIEWPORT_3D_C
#define VIEWPORT_3D_C

#include "viewport_3d.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Viewport3D* viewport_3d_create(Rectangle bounds) {
    Viewport3D *vp = (Viewport3D*)calloc(1, sizeof(Viewport3D));
    if (!vp) return NULL;
    
    vp->bounds = bounds;
    vp->show_grid = true;
    vp->show_axis = true;
    vp->wireframe_mode = false;
    vp->rotation_speed = 0.5f;
    vp->zoom_speed = 1.0f;
    vp->model_position = (Vector3){0, 0, 0};
    vp->model_rotation = (Vector3){0, 0, 0};
    vp->model_scale = 1.0f;
    vp->meshes = NULL;
    vp->mesh_count = 0;
    vp->background_color = (Color){30, 30, 35, 255};
    
    vp->camera.position = (Vector3){5.0f, 5.0f, 5.0f};
    vp->camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    vp->camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    vp->camera.fovy = 60.0f;
    vp->camera.projection = CAMERA_PERSPECTIVE;
    
    return vp;
}

void viewport_3d_destroy(Viewport3D *vp) {
    if (!vp) return;
    if (vp->meshes) {
        for (int i = 0; i < vp->mesh_count; i++) {
            UnloadMesh(vp->meshes[i]);
        }
        free(vp->meshes);
    }
    free(vp);
}

void viewport_3d_update(Viewport3D *vp) {
    if (!vp) return;
    
    if (CheckCollisionPointRec(GetMousePosition(), vp->bounds)) {
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            vp->camera.position.x += wheel * vp->zoom_speed;
            vp->camera.position.y += wheel * vp->zoom_speed;
            vp->camera.position.z += wheel * vp->zoom_speed;
        }
        
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            float delta_x = GetMouseDelta().x * vp->rotation_speed * 0.01f;
            float delta_y = GetMouseDelta().y * vp->rotation_speed * 0.01f;
            
            float dist = sqrtf(vp->camera.position.x * vp->camera.position.x +
                              vp->camera.position.y * vp->camera.position.y +
                              vp->camera.position.z * vp->camera.position.z);
            
            float angle = atan2f(vp->camera.position.z, vp->camera.position.x);
            angle += delta_x;
            
            float pitch = asinf(vp->camera.position.y / dist);
            pitch += delta_y;
            pitch = fmaxf(-1.5f, fminf(1.5f, pitch));
            
            vp->camera.position.x = dist * cosf(pitch) * cosf(angle);
            vp->camera.position.y = dist * sinf(pitch);
            vp->camera.position.z = dist * cosf(pitch) * sinf(angle);
        }
        
        if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
            float delta_x = GetMouseDelta().x * 0.01f;
            float delta_y = GetMouseDelta().y * 0.01f;
            
            vp->camera.target.x -= delta_x;
            vp->camera.target.y += delta_y;
        }
    }
}

void viewport_3d_draw(Viewport3D *vp) {
    if (!vp) return;
    
    BeginScissorMode((int)vp->bounds.x, (int)vp->bounds.y, 
                     (int)vp->bounds.width, (int)vp->bounds.height);
    
    ClearBackground(vp->background_color);
    
    BeginMode3D(vp->camera);
    
    if (vp->show_grid) {
        DrawGrid(20, 1.0f);
    }
    
    if (vp->show_axis) {
        DrawLine3D((Vector3){0, 0, 0}, (Vector3){2, 0, 0}, RED);
        DrawLine3D((Vector3){0, 0, 0}, (Vector3){0, 2, 0}, GREEN);
        DrawLine3D((Vector3){0, 0, 0}, (Vector3){0, 0, 2}, BLUE);
    }
    
    if (vp->mesh_count > 0 && vp->meshes) {
        Matrix transform = (Matrix){
            vp->model_scale, 0, 0, 0,
            0, vp->model_scale, 0, 0,
            0, 0, vp->model_scale, 0,
            vp->model_position.x, vp->model_position.y, vp->model_position.z, 1
        };
        
        Material mat = LoadMaterialDefault();
        
        for (int i = 0; i < vp->mesh_count; i++) {
            DrawMesh(vp->meshes[i], mat, transform);
        }
    } else {
        DrawSphereWires((Vector3){0, 0, 0}, 1.0f, 8, 8, (Color){100, 100, 100, 150});
    }
    
    EndMode3D();
    
    EndScissorMode();
    
    DrawRectangleLinesEx(vp->bounds, 2, (Color){62, 62, 66, 255});
}

void viewport_3d_load_mesh(Viewport3D *vp, Mesh mesh) {
    if (!vp) return;
    
    vp->mesh_count = 1;
    vp->meshes = (Mesh*)malloc(sizeof(Mesh));
    vp->meshes[0] = mesh;
}

void viewport_3d_toggle_grid(Viewport3D *vp) {
    if (!vp) return;
    vp->show_grid = !vp->show_grid;
}

void viewport_3d_toggle_axis(Viewport3D *vp) {
    if (!vp) return;
    vp->show_axis = !vp->show_axis;
}

void viewport_3d_toggle_wireframe(Viewport3D *vp) {
    if (!vp) return;
    vp->wireframe_mode = !vp->wireframe_mode;
}

void viewport_3d_reset_camera(Viewport3D *vp) {
    if (!vp) return;
    vp->camera.position = (Vector3){5.0f, 5.0f, 5.0f};
    vp->camera.target = (Vector3){0.0f, 0.0f, 0.0f};
}

void viewport_3d_set_background(Viewport3D *vp, Color color) {
    if (!vp) return;
    vp->background_color = color;
}

#endif
