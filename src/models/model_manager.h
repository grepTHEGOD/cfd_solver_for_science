#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

#include "mesh.h"
#include "stl_loader.h"
#include "obj_loader.h"

typedef struct {
    int id;
    char name[256];
    char filename[512];
    Mesh *mesh;
    int visible;
    float color[4];
    float opacity;
} ModelData;

typedef struct {
    ModelData *models;
    int count;
    int capacity;
    int selected_id;
} ModelManager;

ModelManager* model_manager_create(void);
void model_manager_destroy(ModelManager *mm);
int model_manager_add_mesh(ModelManager *mm, Mesh *mesh, const char *name);
int model_manager_remove_model(ModelManager *mm, int id);
ModelData* model_manager_get_model(ModelManager *mm, int id);
ModelData* model_manager_get_selected(ModelManager *mm);
void model_manager_select(ModelManager *mm, int id);
int model_manager_import(ModelManager *mm, const char *filename);
int model_manager_export(ModelManager *mm, const char *filename, int model_id);
void model_manager_set_visible(ModelManager *mm, int id, int visible);
void model_manager_set_color(ModelManager *mm, int id, float r, float g, float b, float a);
void model_manager_set_opacity(ModelManager *mm, int id, float opacity);
Mesh* model_manager_get_mesh(ModelManager *mm, int id);
int model_manager_get_count(ModelManager *mm);

#endif
