#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

typedef struct {
    GLuint program;
    GLuint vertex_shader;
    GLuint fragment_shader;
    
    GLint loc_position;
    GLint loc_normal;
    GLint loc_color;
    GLint loc_uv;
    
    GLint loc_model_matrix;
    GLint loc_view_matrix;
    GLint loc_proj_matrix;
    GLint loc_normal_matrix;
    
    GLint loc_light_pos;
    GLint loc_light_color;
    GLint loc_ambient;
    GLint loc_diffuse;
    GLint loc_specular;
    GLint loc_shininess;
    
    GLint loc_use_colormap;
    GLint loc_colormap_min;
    GLint loc_colormap_max;
    GLint loc_scalar_value;
} Shader;

Shader* shader_create(const char *vertex_src, const char *fragment_src);
void shader_destroy(Shader *shader);
void shader_use(Shader *shader);
void shader_set_matrix(Shader *shader, const char *name, float *matrix);
void shader_set_float(Shader *shader, const char *name, float value);
void shader_set_vec3(Shader *shader, const char *name, float x, float y, float z);
void shader_set_vec4(Shader *shader, const char *name, float x, float y, float z, float w);
void shader_set_int(Shader *shader, const char *name, int value);

Shader* shader_create_default(void);
Shader* shader_create_color_map(void);

#endif
