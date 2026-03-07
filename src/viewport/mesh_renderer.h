#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include <GL/glew.h>
#include "shader.h"
#include "../models/mesh.h"

typedef enum {
    RENDER_SURFACE,
    RENDER_WIREFRAME,
    RENDER_POINTS,
    RENDER_SURFACE_WIREFRAME
} RenderMode;

typedef struct {
    GLuint vao;
    GLuint vbo_vertices;
    GLuint vbo_normals;
    GLuint vbo_colors;
    GLuint vbo_uvs;
    GLuint ebo;
    
    int vertex_count;
    int face_count;
    
    RenderMode mode;
    float opacity;
    int use_colormap;
    
    Shader *shader;
    Mesh *mesh;
} MeshRenderer;

void mesh_renderer_init(MeshRenderer *renderer);
void mesh_renderer_destroy(MeshRenderer *renderer);
void mesh_renderer_set_mesh(MeshRenderer *renderer, Mesh *mesh);
void mesh_renderer_set_mode(MeshRenderer *renderer, RenderMode mode);
void mesh_renderer_set_opacity(MeshRenderer *renderer, float opacity);
void mesh_renderer_set_colormap(MeshRenderer *renderer, int use);
void mesh_renderer_update_buffers(MeshRenderer *renderer);
void mesh_renderer_render(MeshRenderer *renderer, float *view_matrix, float *proj_matrix);
void mesh_renderer_render_grid(MeshRenderer *renderer, float *view_matrix, float *proj_matrix, int size, float spacing);
void mesh_renderer_render_axes(MeshRenderer *renderer, float *view_matrix, float *proj_matrix, float size);

#endif
