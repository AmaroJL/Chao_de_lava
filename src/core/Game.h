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

struct Pedra {
    float x, y, z;            
    float largura;         
    float profundidade;    
};

enum GameState {
    PLAYING,
    WON,
    GAME_OVER
};

class Game {
public:
    GameState state;
    Input input;
    Camera camera;
    Player player;
    ModelOBJ wall;
    GLuint texturaLava;
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
    void carregarTexturaLava();
    void desenharTextoNaTela(const char* texto, float x, float y, float r, float g, float b, float escala);
    void reset();
};

#endif
