#ifndef MODEL_MANAGER_C
#define MODEL_MANAGER_C

#include "model_manager.h"
#include <stdlib.h>

typedef struct ModelNode {
    char name[256];
    int type;
    void *data;
    struct ModelNode *next;
} ModelNode;

struct ModelManager {
    ModelNode *first;
    int count;
};

ModelManager* model_manager_create(void) {
    ModelManager *mgr = (ModelManager*)calloc(1, sizeof(ModelManager));
    return mgr;
}

void model_manager_destroy(ModelManager *mgr) {
    if (!mgr) return;
    free(mgr);
}

int model_manager_add_mesh(ModelManager *mgr, void *mesh, const char *name) {
    (void)mgr;
    (void)mesh;
    (void)name;
    return 0;
}

void* model_manager_get_mesh(ModelManager *mgr, int index) {
    (void)mgr;
    (void)index;
    return NULL;
}

int model_manager_get_count(ModelManager *mgr) {
    return mgr ? mgr->count : 0;
}

#endif
