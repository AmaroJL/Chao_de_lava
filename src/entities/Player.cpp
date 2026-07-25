#include <GL/glut.h>

#include "Player.h"
#include "../core/Input.h"

Player::Player() {
    x = 0.0f;
    y = 5.0f;
    z = 0.0f;
    yaw = 0.0f;
    velY = 0.0f;
    onGround = false;

    idleTime = 0.0f;
    idleBob = 0.0f;
    idleArmSwing = 0.0f;
    idleHeadTilt = 0.0f;

    walkTime = 0.0f;
    walkBob = 0.0f;
    walkArmSwing = 0.0f;
    walkLegSwing = 0.0f;
}

void Player::Update(Input *input, float moveX, float moveZ) {
    float charSpeed = 0.15f;

    if (input->IsKeyPressed(' ') && onGround) {
        velY = 0.50f;
        onGround = false;
    }
    
    velY -= 0.02f;
    y += velY;
    onGround = false;

    if (moveX != 0.0f || moveZ != 0.0f) {
        float length = sqrt(moveX * moveX + moveZ * moveZ);
        moveX /= length;
        moveZ /= length;

        x += moveX * charSpeed;
        z += moveZ * charSpeed;
        
        float targetYaw = atan2(moveX, moveZ) * 180.0f / M_PI;
        float diff = targetYaw - yaw;
        
        while (diff > 180.0f)  diff -= 360.0f;
        while (diff < -180.0f) diff += 360.0f;
        
        yaw += diff * 0.15f;
        
        while (yaw >= 360.0f) yaw -= 360.0f;
        while (yaw < 0.0f)    yaw += 360.0f;
    }

    bool isWalking = (moveX != 0.0f || moveZ != 0.0f);
    
    if (isWalking) {
        walkTime += 0.016f;
        walkArmSwing = sin(walkTime * 15.0f) * 45.0f;
        walkLegSwing = sin(walkTime * 15.0f) * 35.0f;
        walkBob = fabs(sin(walkTime * 15.0f)) * 0.05f; 
    } else {
        walkTime = 0.0f;
        walkArmSwing *= 0.8f;
        walkLegSwing *= 0.8f;
        walkBob *= 0.8f;
    }

    idleTime += 0.016f;
    idleBob      = sin(idleTime * 2.0f) * 0.01f;
    idleArmSwing = sin(idleTime * 1.5f) * 4.0f;
    idleHeadTilt = sin(idleTime * 0.9f) * 2.5f;
}

void Player::landOn(float feetY) {
    y = feetY;
    velY = 0.0f;
    onGround = true;
}

void Player::DrawCylinder(float radius, float height, int slices) {
    glPushMatrix();
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        GLUquadric* q = gluNewQuadric();
        gluCylinder(q, radius, radius * 0.88f, height, slices, 1);
        gluDeleteQuadric(q);
    glPopMatrix();
}

void Player::DrawCylinderLow(float radius, float height, int slices) {
    glPushMatrix();
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        GLUquadric* q = gluNewQuadric();
        gluCylinder(q, radius, radius * 0.88f, height, slices, 1);
        gluDeleteQuadric(q);
    glPopMatrix();
}

void Player::Render() {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(yaw, 0.0f, 1.0f, 0.0f);

    GLfloat mat_corpo[] = { 0.92f, 0.92f, 0.92f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_corpo);
    glPushMatrix();

    // Perna esquerda
    glPushMatrix();
        glTranslatef(0.0f, 0.89f + walkBob, 0.0f);
        glRotatef(walkLegSwing, 1.0f, 0.0f, 0.0f); 
        glTranslatef(0.0f, -(0.89f + walkBob), 0.0f);

        glTranslatef(-0.12f, 0.0f, 0.0f);
        
        glPushMatrix();
            glScalef(0.9f, 0.5f, 1.1f);
            glTranslatef(0.0f, 0.1f, 0.02f);
            glutSolidCube(0.16f);
        glPopMatrix();

        glTranslatef(0.0f, 0.08f, 0.0f);
        DrawCylinder(0.07f, 0.40f, 12);

        glTranslatef(0.0f, 0.40f, 0.0f);
        DrawCylinder(0.07f, 0.40f, 12);

        glutSolidSphere(0.065f, 12, 12);

        glTranslatef(0.0f, 0.41f, 0.0f);
        glutSolidSphere(0.06f, 12, 12);
    glPopMatrix();

    // Perna direita
    glPushMatrix();
        glTranslatef(0.0f, 0.89f + walkBob, 0.0f);
        glRotatef(-walkLegSwing, 1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, -(0.89f + walkBob), 0.0f);

        glTranslatef(0.12f, 0.0f, 0.0f);
        
        glPushMatrix();
            glScalef(0.9f, 0.5f, 1.1f);
            glTranslatef(0.0f, 0.1f, 0.02f);
            glutSolidCube(0.16f);
        glPopMatrix();

        glTranslatef(0.0f, 0.08f, 0.0f);
        DrawCylinder(0.07f, 0.40f);

        glTranslatef(0.0f, 0.40f, 0.0f);
        DrawCylinder(0.07f, 0.40f);

        glutSolidSphere(0.065f, 12, 12);

        glTranslatef(0.0f, 0.41f, 0.0f);
        glutSolidSphere(0.06f, 12, 12);
    glPopMatrix();

    // Tronco
    glTranslatef(0.0f, 0.87f + idleBob + walkBob, 0.0f);
    DrawCylinder(0.14f, 0.55f);

    // Braço esquerdo
    glPushMatrix();
        glTranslatef(-0.17f, 0.40f, 0.0f);
        glutSolidSphere(0.07f, 12, 12);
        glRotatef(-20.0f + idleArmSwing, 0.0f, 0.0f, 1.0f);
        glRotatef(-walkArmSwing, 1.0f, 0.0f, 0.0f);

        DrawCylinderLow(0.075f, 0.33f);

        glTranslatef(0.0f, -0.33f, 0.0f);
        glRotatef(15.0f, 0.0f, 0.0f, 1.0f);
        glutSolidSphere(0.065f, 12, 12);
        DrawCylinderLow(0.065f, 0.30f);

        glTranslatef(0.0f, -0.28f, 0.0f);
        glutSolidSphere(0.09f, 12, 12);
    glPopMatrix();

    // Braço direito
    glPushMatrix();
        glTranslatef(0.17f, 0.40f, 0.0f);
        glutSolidSphere(0.07f, 12, 12);
        glRotatef(20.0f - idleArmSwing, 0.0f, 0.0f, 1.0f);
        glRotatef(walkArmSwing, 1.0f, 0.0f, 0.0f);

        DrawCylinderLow(0.075f, 0.33f);

        glTranslatef(0.0f, -0.33f, 0.0f);
        glRotatef(-15.0f, 0.0f, 0.0f, 1.0f);
        glutSolidSphere(0.065f, 12, 12);
        DrawCylinderLow(0.065f, 0.30f);

        glTranslatef(0.0f, -0.28f, 0.0f);
        glutSolidSphere(0.09f, 12, 12);
    glPopMatrix();
    
    // Pescoço
    glTranslatef(0.0f, 0.55f, 0.0f);
    DrawCylinder(0.07f, 0.18f);

    // Cabeça
    glTranslatef(0.0f, 0.18f, 0.0f);
    glRotatef(idleHeadTilt, 0.0f, 0.0f, 1.0f);
    glutSolidSphere(0.30f, 24, 24);

    // Olhos
    GLfloat mat_olhos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_olhos);
    glPushMatrix();
        glTranslatef(-0.1f, 0.05f, 0.27f);
        glutSolidSphere(0.04f, 24, 24);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.1f, 0.05f, 0.27f);
        glutSolidSphere(0.04f, 24, 24);
    glPopMatrix();

    // Chapéu
    glPushMatrix();
        GLfloat mat_chapeu[] = { 0.05f, 0.05f, 0.05f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_chapeu);

        glTranslatef(0.0f, 0.22f, 0.0f);
        glPushMatrix();
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            GLUquadric* quadricHatBrim = gluNewQuadric();
            gluDisk(quadricHatBrim, 0.0f, 0.46f, 24, 1);
            gluDeleteQuadric(quadricHatBrim);
        glPopMatrix();

        glPushMatrix();
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            GLUquadric* quadricHatCrown = gluNewQuadric();
            gluCylinder(quadricHatCrown, 0.22f, 0.20f, 0.25f, 24, 1);
            gluDeleteQuadric(quadricHatCrown);
            GLUquadric* quadricHatTop = gluNewQuadric();
            glTranslatef(0.0f, 0.0f, 0.25f);
            gluDisk(quadricHatTop, 0.0f, 0.20f, 24, 1);
            gluDeleteQuadric(quadricHatTop);
        glPopMatrix();
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();
}
