#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cfloat>
#include <algorithm>

#include "ModelOBJ.h"

ModelOBJ::ModelOBJ() {
    displayListId = 0;
    loaded = false;
}

ModelOBJ::~ModelOBJ() {
    if (loaded) {
        glDeleteLists(displayListId, 1);
    }
}

bool ModelOBJ::load(const std::string& path) {
    std::ifstream file(path.c_str());
    if (!file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo " << path << std::endl;
        return false;
    }

    std::vector<Vertex> temp_verts;
    std::vector<Normal> temp_normals;
    std::vector<Face> faces;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream s(line);
        std::string prefix;
        s >> prefix;

        if (prefix == "v") {
            Vertex v;
            s >> v.x >> v.y >> v.z;
            temp_verts.push_back(v);
        }
        else if (prefix == "vn") {
            Normal n;
            s >> n.nx >> n.ny >> n.nz;
            temp_normals.push_back(n);
        }
        else if (prefix == "f") {
            Face f;
            std::string token;
            
            for (int i = 0; i < 3; i++) {
                s >> token;
                
                int v = 0, vt = 0, vn = 0;
                
                if (sscanf(token.c_str(), "%d/%d/%d", &v, &vt, &vn) == 3) {
                    f.v[i] = v; f.n[i] = vn;
                } else if (sscanf(token.c_str(), "%d//%d", &v, &vn) == 2) {
                    f.v[i] = v; f.n[i] = vn;
                } else if (sscanf(token.c_str(), "%d/%d", &v, &vt) == 2) {
                    f.v[i] = v; f.n[i] = -1;
                } else if (sscanf(token.c_str(), "%d", &v) == 1) {
                    f.v[i] = v; f.n[i] = -1;
                }
                
                f.v[i]--; 
                if (f.n[i] > 0) f.n[i]--;
            }
            faces.push_back(f);
        }
    }
    file.close();

    float bMinX = FLT_MAX, bMaxX = -FLT_MAX;
    float bMinY = FLT_MAX, bMaxY = -FLT_MAX;
    float bMinZ = FLT_MAX, bMaxZ = -FLT_MAX;

    for (const Vertex& v : temp_verts) {
        bMinX = std::min(bMinX, v.x); bMaxX = std::max(bMaxX, v.x);
        bMinY = std::min(bMinY, v.y); bMaxY = std::max(bMaxY, v.y);
        bMinZ = std::min(bMinZ, v.z); bMaxZ = std::max(bMaxZ, v.z);
    }

    hitBox = { bMinX, bMaxX, bMinY, bMaxY, bMinZ, bMaxZ };

    displayListId = glGenLists(1);
    glNewList(displayListId, GL_COMPILE);
    
    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < faces.size(); i++) {
        for (int j = 0; j < 3; j++) {  
            if (faces[i].n[j] >= 0 && faces[i].n[j] < (int)temp_normals.size()) {
                Normal n = temp_normals[faces[i].n[j]];
                glNormal3f(n.nx, n.ny, n.nz);
            }
            
            Vertex v = temp_verts[faces[i].v[j]];
            glVertex3f(v.x, v.y, v.z);
        }
    }
    glEnd();
    
    glEndList();
    loaded = true;
    
    std::cout << "Modelo carregado com sucesso: " << path << " (" << faces.size() << " triangulos)" << std::endl;
    
    return true;
}

void ModelOBJ::draw() {
    if (loaded) {
        glCallList(displayListId);
    }
}
