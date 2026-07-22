#include <GL/glut.h>
#include <algorithm>

#include "Camera.h"

/* 
Arquivo que gerencia a câmera em terceira
pessoa que segue o jogador
*/

// Construtor da câmera
Camera::Camera() {
    dist = 10.0f;
    pitch = 25.0f;
    yaw = -90.0f;
}

// Move a câmera a partir das setas
void Camera::Update(Input* input) {
    float angleSpeed = 2.0f;
    
    if (input->IsSpecialKeyPressed(GLUT_KEY_LEFT)) { yaw -= angleSpeed; }
    if (input->IsSpecialKeyPressed(GLUT_KEY_RIGHT)) { yaw += angleSpeed; }
    if (input->IsSpecialKeyPressed(GLUT_KEY_UP)) { pitch = std::max(pitch - angleSpeed, 5.0f); }
    if (input->IsSpecialKeyPressed(GLUT_KEY_DOWN)) { pitch = std::min(pitch + angleSpeed, 75.0f); }
}

// Aplica a câmera a um alvo
void Camera::Apply(Player* target) {
    float camYawRad = yaw * M_PI / 180.0f;
    float camPitchRad = pitch * M_PI / 180.0f;

    float camOffsetX = -dist * cos(camPitchRad) * cos(camYawRad);
    float camOffsetY = dist * sin(camPitchRad);
    float camOffsetZ = -dist * cos(camPitchRad) * sin(camYawRad);

    float camX = target->x + camOffsetX;
    float camY = target->y + 1.0f + camOffsetY;
    float camZ = target->z + camOffsetZ;

    gluLookAt(camX, camY, camZ,
              target->x, target->y + 1.0f, target->z,
              0.0f, 1.0f, 0.0f);
}
