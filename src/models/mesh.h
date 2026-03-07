#ifndef MESH_H
#define MESH_H

#include <stdlib.h>
#include <string.h>

typedef struct {
    float *vertices;
    float *normals;
    float *colors;
    float *uvs;
    float *scalars;
    unsigned int *indices;
    
    int vertex_count;
    int face_count;
    
    float center[3];
    float bounding_min[3];
    float bounding_max[3];
    
    char name[256];
} CFDMesh;

CFDMesh* mesh_create(void);
void mesh_destroy(CFDMesh *mesh);
void mesh_allocate(CFDMesh *mesh, int vertex_count, int face_count);
void mesh_set_vertices(CFDMesh *mesh, float *vertices, int count);
void mesh_set_normals(CFDMesh *mesh, float *normals, int count);
void mesh_set_colors(CFDMesh *mesh, float *colors, int count);
void mesh_set_indices(CFDMesh *mesh, unsigned int *indices, int count);
void mesh_compute_bounding_box(CFDMesh *mesh);
void mesh_center_at_origin(CFDMesh *mesh);
void mesh_scale_to_unit_size(CFDMesh *mesh);

#endif
