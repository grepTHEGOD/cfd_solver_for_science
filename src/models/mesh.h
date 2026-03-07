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
} Mesh;

Mesh* mesh_create(void);
void mesh_destroy(Mesh *mesh);
void mesh_allocate(Mesh *mesh, int vertex_count, int face_count);
void mesh_set_vertices(Mesh *mesh, float *vertices, int count);
void mesh_set_normals(Mesh *mesh, float *normals, int count);
void mesh_set_colors(Mesh *mesh, float *colors, int count);
void mesh_set_indices(Mesh *mesh, unsigned int *indices, int count);
void mesh_compute_bounding_box(Mesh *mesh);
void mesh_center_at_origin(Mesh *mesh);
void mesh_scale_to_unit_size(Mesh *mesh);

#endif
