#ifndef VECTORS_H
#define VECTORS_H

#include "../models/mesh.h"

typedef struct {
    float *positions;
    float *directions;
    float *magnitudes;
    int count;
    float scale;
    float min_magnitude;
    float max_magnitude;
} VectorField;

VectorField* vectors_create(Mesh *mesh, float *velocity_field, int num_points);
void vectors_destroy(VectorField *vf);
void vectors_set_scale(VectorField *vf, float scale);
void vectors_render(VectorField *vf);

#endif
