#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <math.h>

typedef enum {
    CAMERA_ORTHO,
    CAMERA_PERSPECTIVE
} CameraType;

typedef enum {
    CAMERA_TOP,
    CAMERA_FRONT,
    CAMERA_SIDE,
    CAMERA_ISO
} CameraPreset;

typedef struct {
    float position[3];
    float target[3];
    float up[3];
    
    float fov;
    float aspect;
    float near_clip;
    float far_clip;
    
    float azimuth;
    float elevation;
    float distance;
    
    float pan_x;
    float pan_y;
    
    CameraType type;
    
    float view_matrix[16];
    float proj_matrix[16];
} Camera;

void camera_init(Camera *cam);
void camera_set_perspective(Camera *cam, float fov, float aspect, float near, float far);
void camera_set_ortho(Camera *cam, float left, float right, float bottom, float top, float near, float far);
void camera_look_at(Camera *cam, float *eye, float *target, float *up);
void camera_rotate(Camera *cam, float d_azimuth, float d_elevation);
void camera_pan(Camera *cam, float dx, float dy);
void camera_zoom(Camera *cam, float delta);
void camera_set_preset(Camera *cam, CameraPreset preset);
void camera_update(Camera *cam);
void camera_get_view_matrix(Camera *cam, float *matrix);
void camera_get_projection_matrix(Camera *cam, float *matrix);

#endif
