#ifndef COLORMAP_H
#define COLORMAP_H

#include <math.h>
#include "../models/mesh.h"

typedef enum {
    COLORMAP_RAINBOW,
    COLORMAP_JET,
    COLORMAP_VIRIDIS,
    COLORMAP_PLASMA,
    COLORMAP_INFERNO,
    COLORMAP_COOLWARM,
    COLORMAP_BLUE_RED,
    COLORMAP_GRAY
} ColormapType;

void colormap_get_color(ColormapType type, float t, float *r, float *g, float *b);
void colormap_get_color_rgba(ColormapType type, float t, float *r, float *g, float *b, float *a);
void colormap_apply(Mesh *mesh, float *scalar_values, int count, ColormapType type);
void colormap_get_range(float *min_val, float *max_val, float *values, int count);

#endif
