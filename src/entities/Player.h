#ifndef PLAYER_H
#define PLAYER_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include "../core/Input.h"

class Player {
public:
    static constexpr float halfW = 0.2f;
    static constexpr float halfH = 1.1f;
    static constexpr float halfD = 0.2f;

    float x, y, z;
    float yaw;
    float velY;
    bool onGround;

    float idleTime;
    float idleBob;
    float idleArmSwing;
    float idleHeadTilt;

    float walkTime;
    float walkBob;
    float walkArmSwing;
    float walkLegSwing;

    Player();
    
    void Update(Input *input, float moveX, float moveZ);

    void landOn(float feetY);

    void Render();

private:
    void DrawCylinder(float radius, float height, int slices = 12);
    void DrawCylinderLow(float radius, float height, int slices = 12);
};

#endif
