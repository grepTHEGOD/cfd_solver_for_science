#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "mesh.h"

Mesh* obj_load(const char *filename);
int obj_save(const char *filename, const Mesh *mesh);

#endif
