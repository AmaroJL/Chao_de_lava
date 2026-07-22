#ifndef GAME_H
#define GAME_H

#include <vector>

#include "Input.h"
#include "Camera.h"
#include "Collision.h"
#include "../entities/Player.h"
#include "../rendering/ModelOBJ.h"

/*
Cabeçalho do game
*/

class Game {
public:
    Input input;
    Camera camera;
    Player player;
    ModelOBJ wall;
    std::vector<AABB> worldBoxes; // Lista de hitboxes

    Game();
    ~Game();

    void Init();
    void Update();
    void Render();

    void KeyDown(unsigned char key);
    void KeyUp(unsigned char key);
    void SpecialKeyDown(int key);
    void SpecialKeyUp(int key);

private:
    void CreateGround();
    void initializeScenario();
    void resolvePlayerCollision(float prevX, float prevY, float prevZ);
};

#endif
