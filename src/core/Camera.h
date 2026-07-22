#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glu.h>
#include <math.h>

#include "Input.h"
#include "../entities/Player.h"

/*
Cabeçalho da câmera
*/

class Camera {
public:
    float dist;
    float pitch;
    float yaw;

    Camera();

    void Update(Input* input);

    void Apply(Player* target);
};

#endif
