#ifndef STL_LOADER_H
#define STL_LOADER_H

#include "mesh.h"

typedef enum {
    STL_ASCII,
    STL_BINARY
} STLType;

Mesh* stl_load(const char *filename);
int stl_save(const char *filename, const Mesh *mesh);
STLType stl_detect_type(const char *filename);

#endif
