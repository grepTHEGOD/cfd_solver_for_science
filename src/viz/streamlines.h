#ifndef STREAMLINES_H
#define STREAMLINES_H

#include "../models/mesh.h"

typedef struct {
    float *vertices;
    int *counts;
    int num_streamlines;
} Streamlines;

Streamlines* streamlines_compute(Mesh *mesh, float *velocity_field, float *seed_points, int num_seeds, float step_size, int max_steps);
void streamlines_destroy(Streamlines *sl);
void streamlines_set_step_size(Streamlines *sl, float step_size);
void streamlines_render(Streamlines *sl);

#endif
