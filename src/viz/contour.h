#ifndef CONTOUR_H
#define CONTOUR_H

#include "../models/mesh.h"

typedef struct {
    float *vertices;
    float *colors;
    int vertex_count;
    int *contour_levels;
    int num_levels;
} ContourLines;

ContourLines* contour_generate(Mesh *mesh, float *scalar_field, int num_levels, float *levels);
void contour_destroy(ContourLines *contours);
void contour_render(ContourLines *contours);

#endif
