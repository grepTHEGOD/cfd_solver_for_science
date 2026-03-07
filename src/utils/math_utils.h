#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <math.h>

void vec3_add(const float *a, const float *b, float *out);
void vec3_sub(const float *a, const float *b, float *out);
void vec3_scale(const float *a, float s, float *out);
float vec3_dot(const float *a, const float *b);
void vec3_cross(const float *a, const float *b, float *out);
void vec3_normalize(const float *a, float *out);
float vec3_length(const float *a);
float vec3_distance(const float *a, const float *b);

void mat4_identity(float *out);
void mat4_multiply(const float *a, const float *b, float *out);
void mat4_translate(float *out, float x, float y, float z);
void mat4_scale(float *out, float x, float y, float z);
void mat4_rotate_x(float *out, float angle);
void mat4_rotate_y(float *out, float angle);
void mat4_rotate_z(float *out, float angle);
void mat4_perspective(float *out, float fov, float aspect, float near, float far);
void mat4_ortho(float *out, float left, float right, float bottom, float top, float near, float far);
void mat4_look_at(float *out, const float *eye, const float *center, const float *up);
void mat4_invert(const float *in, float *out);
void mat4_transpose(const float *in, float *out);

float clamp(float val, float min_val, float max_val);
float lerp(float a, float b, float t);
float deg_to_rad(float deg);
float rad_to_deg(float rad);

#endif
