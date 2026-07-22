#include <GL/glut.h>
#include <math.h>
#include <algorithm>

#include "Game.h"

/*
Arquivo que gerencia o fluxo do jogo, incluindo
inicialização, renderização e atualização de posição
*/

Game::Game() {}
Game::~Game() {}

// Inicializa e configura o jogo
void Game::Init() {
    glClearColor(0.53f, 0.81f, 0.98f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat ambient_global[] = { 0.15f, 0.18f, 0.25f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_global);

    GLfloat sun_diffuse[] = { 1.0f, 0.95f, 0.80f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_diffuse);

    GLfloat sun_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_SPECULAR, sun_specular);

    GLfloat sun_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, sun_ambient);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 800.0f/500.0f, 0.1f, 100.0f);

    wall.load("assets/models/wall.obj");

    AABB hitBoxWall = wall.hitBox;
    hitBoxWall.minX += 0.0f; hitBoxWall.maxX += 0.0f;
    hitBoxWall.minY += -2.0f; hitBoxWall.maxY += -2.0f;
    hitBoxWall.minZ += -5.0f; hitBoxWall.maxZ += -5.0f;
    worldBoxes.push_back(hitBoxWall);

    worldBoxes.push_back(createAABB(0.0f, -3.0f, 0.0f, 500.0f, 1.0f, 500.0f));
}

// Atualiza a posição do jogador a partir das teclas WASD
void Game::Update() {
    camera.Update(&input);

    float camYawRad = camera.yaw * M_PI / 180.0f;
    float forwardX = cos(camYawRad);
    float forwardZ = sin(camYawRad);
    float rightX = cos(camYawRad + M_PI / 2.0f);
    float rightZ = sin(camYawRad + M_PI / 2.0f);

    float moveX = 0.0f;
    float moveZ = 0.0f;

    if (input.IsKeyPressed('w') || input.IsKeyPressed('W')) { moveX += forwardX; moveZ += forwardZ; }
    if (input.IsKeyPressed('s') || input.IsKeyPressed('S')) { moveX -= forwardX; moveZ -= forwardZ; }
    if (input.IsKeyPressed('a') || input.IsKeyPressed('A')) { moveX -= rightX; moveZ -= rightZ; }
    if (input.IsKeyPressed('d') || input.IsKeyPressed('D')) { moveX += rightX; moveZ += rightZ; }

    float prevX = player.x;
    float prevY = player.y;
    float prevZ = player.z;

    player.Update(&input, moveX, moveZ);

    resolvePlayerCollision(prevX, prevY, prevZ);
}

// Resolve colisões do jogador com o mundo
void Game::resolvePlayerCollision(float prevX, float prevY, float prevZ) {
    for (const AABB& worldBox : worldBoxes) {

        AABB testX = createAABB(player.x, prevY + Player::halfH, prevZ, Player::halfW, Player::halfH, Player::halfD);
        if (verifyCollision(testX, worldBox)) {
            player.x = prevX;
        }

        AABB testZ = createAABB(player.x, prevY + Player::halfH, player.z, Player::halfW, Player::halfH, Player::halfD);
        if (verifyCollision(testZ, worldBox)) {
            player.z = prevZ;
        }

        AABB testY = createAABB(player.x, player.y + Player::halfH, player.z, Player::halfW, Player::halfH, Player::halfD);
        if (verifyCollision(testY, worldBox)) {
            if (player.velY <= 0.0f) {
                player.landOn(worldBox.maxY);
            } else {
                player.y = prevY;
                player.velY = 0.0f;
            }
        }
    }
}

// Cria o chão do jogo
void Game::CreateGround() {
    // glNormal3f(0.0f, 1.0f, 0.0f);

    // glColor3f(0.2f, 0.6f, 0.2f);
    // glBegin(GL_QUADS);
    //     glVertex3f(-500.0f, -2.0f, -500.0f);
    //     glVertex3f(-500.0f, -2.0f,  500.0f);
    //     glVertex3f( 500.0f, -2.0f,  500.0f);
    //     glVertex3f( 500.0f, -2.0f, -500.0f);
    // glEnd();

    // glColor3f(0.1f, 0.4f, 0.1f);
    // glBegin(GL_LINES);
    //     for(float i = -500; i <= 500; i += 2.0f) {
    //         glVertex3f(i, -1.99f, -500.0f);
    //         glVertex3f(i, -1.99f,  500.0f);
    //         glVertex3f(-500.0f, -1.99f, i);
    //         glVertex3f( 500.0f, -1.99f, i);
    //     }
    // glEnd();
}

// Renderiza os elementos do jogo
void Game::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    camera.Apply(&player);

    GLfloat sun_direction[] = { 0.6f, 1.0f, 0.5f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, sun_direction);

    CreateGround();

    glPushMatrix();
        glTranslatef(0.0f, -2.0f, -5.0f);
        glColor3f(0.8f, 0.8f, 0.8f);
        wall.draw();
    glPopMatrix();

    player.Render();

    glutSwapBuffers();
}

// Trata teclas pressionadas
void Game::KeyDown(unsigned char key) { input.SetKey(key, true); }
void Game::KeyUp(unsigned char key) { input.SetKey(key, false); }
void Game::SpecialKeyDown(int key) { input.SetSpecialKey(key, true); }
void Game::SpecialKeyUp(int key) { input.SetSpecialKey(key, false); }
