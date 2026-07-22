#ifndef OBJ_H
#define OBJ_H

#include <GL/gl.h>
#include <vector>
#include <string>

#include "../core/Collision.h"

struct Vertex {
    float x, y, z;
};

struct Normal {
    float nx, ny, nz;
};

struct Face {
    int v[3]; 
    int n[3]; 
};

class ModelOBJ {
private:
    GLuint displayListId;
    bool loaded;

public:
    AABB hitBox;

    ModelOBJ();
    ~ModelOBJ();

    bool load(const std::string& path);

    void draw();
};

#endif
