#include <GL/glut.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <iostream>

#include "Game.h"

/*
Arquivo que gerencia o fluxo do jogo, incluindo
inicialização, renderização e atualização de posição
*/

Game::Game() {}
Game::~Game() {}

struct Pedra {
    float x, y, z;            
    float largura;         
    float profundidade;    
};
std::vector<Pedra> listaPedras;
float mapY = 0.3f;

// Inicializa e configura o jogo
void Game::Init() {
    glClearColor(0.53f, 0.81f, 0.98f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
   

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

    carregarTexturaLava();
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

    mapY += 0.00041f;
    if (player.y < 0.1f) {
        std::cout << "GAME OVER!" << std::endl;
        player.x = 0.0f;
        player.y = 5.0f;
        player.z = 0.0f;
    } else if (mapY > player.y + 0.3f && player.velY == 0) {
        mapY = 0.3f;
        player.x = 0.0f;
        player.y = 5.0f;
        player.z = 0.0f;
    }
    if (player.z == -135.0f && player.y == 4.0f && player.velY == 0) {
        std::cout << "\n=================================================" << std::endl;
        std::cout << "    VITORIA! VOCE ESCAPOU DO RIO DE LAVA!     " << std::endl;
        std::cout << "=================================================\n" << std::endl;
    }
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
    initializeScenario();

    // Desenha o Chão de Lava
    GLfloat mat_ambient_lava[]   = { 0.8f, 0.2f, 0.0f, 1.0f }; 
    GLfloat mat_diffuse_lava[]   = { 0.9f, 0.3f, 0.0f, 1.0f };
    GLfloat mat_specular_lava[]  = { 0.8f, 0.8f, 0.8f, 1.0f }; 
    GLfloat mat_shininess_lava[] = { 80.0f };
    
    GLfloat mat_emission_lava[]  = { 0.4f, 0.1f, 0.0f, 1.0f }; 

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_lava);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_lava);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_lava);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess_lava);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission_lava);

    glEnable(GL_TEXTURE_2D); // LIGA A TEXTURA
    glBindTexture(GL_TEXTURE_2D, texturaLava); // SELECIONA O BMP DA LAVA

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f); 

        // As coordenadas 15.0f fazem a textura se repetir 15 vezes no chão gigante
        glTexCoord2f(0.0f, 15.0f); glVertex3f(-100.0f, mapY,  200.0f/2);
        glTexCoord2f(15.0f, 15.0f); glVertex3f( 100.0f, mapY,  200.0f/2);
        glTexCoord2f(15.0f, 0.0f); glVertex3f( 100.0f, mapY, -200.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, mapY, -200.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D); // DESLIGA PARA NÃO AFETAR AS PLATAFORMAS (PEDRAS)

    // Tira a emissão de luz das plataformas
    GLfloat sem_emissao[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, sem_emissao);
    
    // Desenha as plataformas
    GLfloat mat_specular[]  = { 0.1f, 0.1f, 0.1f, 1.0f }; 
    GLfloat mat_shininess[] = { 10.0f };                 

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // Cores das plataformas
    GLfloat corAzul[]    = { 0.0f, 0.0f, 1.0f, 1.0f };
    GLfloat corAmarelo[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    GLfloat corVerde[]   = { 0.0f, 1.0f, 0.0f, 1.0f };

    for (size_t i = 0; i < listaPedras.size(); i++) {
        const auto& p = listaPedras[i];

        // Alterna a cor
        if (i % 3 == 0) {
            glMaterialfv(GL_FRONT, GL_AMBIENT, corAzul);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, corAzul);
        } else if (i % 3 == 1) {
            glMaterialfv(GL_FRONT, GL_AMBIENT, corAmarelo);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, corAmarelo);
        } else {
            glMaterialfv(GL_FRONT, GL_AMBIENT, corVerde);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, corVerde);
        }

        glPushMatrix();
            glTranslatef(p.x, p.y, p.z); 
            glScalef(p.largura, 1.0f, p.profundidade); 
            glutSolidCube(1.0);
        glPopMatrix();
    }
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

    player.Render();

    
    glDisable(GL_LIGHTING); // Desativa a luz para a sombra ficar sólida e escura
    glColor3f(0.1f, 0.1f, 0.1f); // Cor da sombra (cinza quase preto)

    glPushMatrix();
        
        // Pega a direção da luz 
        float Lx = 0.6f;
        float Ly = 1.0f;
        float Lz = 0.5f;

        // Lógica para a sombra não pular junto com o boneco
        static float alturaSombra = 4.0f;
        if (player.velY == 0.0f) { 
            alturaSombra = player.y; 
        }

        glTranslatef(0.0f, alturaSombra + 0.01f, 0.0f);

        GLfloat matrizSombra[16] = {
             1.0f,     0.0f,    0.0f,    0.0f,
            -Lx/Ly,    0.0f,   -Lz/Ly,   0.0f,
             0.0f,     0.0f,    1.0f,    0.0f,
             0.0f,     0.0f,    0.0f,    1.0f
        };
        glMultMatrixf(matrizSombra);

        glTranslatef(0.0f, -alturaSombra, 0.0f);

        player.Render();

    glPopMatrix();

    glEnable(GL_LIGHTING);
    glutSwapBuffers();
}

// Funções auxiliares para criação do mapa

const std::vector<Pedra>& getPedras() {
    return listaPedras;
}

void Game::initializeScenario() {
    listaPedras.clear();

    listaPedras.push_back({ 0.0f, 4.0f,   0.0f,  5.0f,  5.0f}); 
    listaPedras.push_back({ 0.0f, 4.0f,  -5.0f,  3.0f,  3.0f}); 
    listaPedras.push_back({ 0.0f, 4.5f, -10.0f,  3.0f,  3.0f}); 

    listaPedras.push_back({-3.0f, 5.5f, -14.0f,  2.5f,  2.5f}); 
    listaPedras.push_back({-6.0f, 6.5f, -18.0f,  2.5f,  2.5f}); 
    listaPedras.push_back({-3.0f, 7.5f, -22.0f,  2.5f,  2.5f}); 

    listaPedras.push_back({ 0.0f, 4.0f, -27.0f,  4.0f,  4.0f}); 

    listaPedras.push_back({ 5.0f, 4.0f, -32.0f,  2.5f,  2.5f}); 
    listaPedras.push_back({ 9.0f, 4.0f, -37.0f,  2.5f,  2.5f}); 
    listaPedras.push_back({ 5.0f, 4.0f, -42.0f,  2.5f,  2.5f}); 

    listaPedras.push_back({-4.0f, 5.0f, -32.0f,  1.5f,  1.5f}); 
    listaPedras.push_back({-8.0f, 6.0f, -37.0f,  1.5f,  1.5f}); 
    listaPedras.push_back({-4.0f, 7.0f, -42.0f,  1.5f,  1.5f}); 

    listaPedras.push_back({ 0.0f, 4.0f, -50.0f,  8.0f,  8.0f}); 

    listaPedras.push_back({-8.0f, 5.0f, -56.0f,  3.0f,  3.0f}); 
    listaPedras.push_back({-14.0f, 6.0f, -62.0f,  3.0f,  3.0f}); 
    listaPedras.push_back({-8.0f, 7.0f, -68.0f,  2.0f,  2.0f}); 
    listaPedras.push_back({ 0.0f, 8.5f, -74.0f,  2.0f,  2.0f}); 
    listaPedras.push_back({ 8.0f, 7.0f, -80.0f,  3.0f,  3.0f}); 
    listaPedras.push_back({14.0f, 5.0f, -86.0f,  3.0f,  3.0f}); 

    listaPedras.push_back({ 6.0f, 4.5f, -92.0f,  2.0f,  2.0f});
    listaPedras.push_back({ 0.0f, 5.5f, -97.0f,  1.5f,  1.5f}); 
    listaPedras.push_back({-4.0f, 6.5f,-102.0f,  1.5f,  1.5f});
    listaPedras.push_back({ 0.0f, 4.5f,-107.0f,  1.5f,  1.5f}); 

    listaPedras.push_back({ 0.0f, 5.5f,-113.0f,  2.0f,  2.0f});
    listaPedras.push_back({ 0.0f, 6.5f,-119.0f,  2.0f,  2.0f});
    listaPedras.push_back({ 0.0f, 7.5f,-125.0f,  1.5f,  1.5f}); 

    listaPedras.push_back({ 0.0f, 4.0f,-135.0f, 15.0f, 10.0f});

    for (const auto& p : listaPedras) {
        worldBoxes.push_back(createAABB(
            p.x,
            p.y,                     
            p.z,
            p.largura / 2.0f,
            0.5f,                    
            p.profundidade / 2.0f
        ));
    }
}

void Game::carregarTexturaLava() {
    // abrir o arquivo 
    FILE * file = fopen("assets/lava.bmp", "rb");
    if (!file) {
        std::cout << "Imagem BMP nao encontrada!" << std::endl;
        return;
    }

    // ler o cabeçalho de 54 bytes do BMP
    unsigned char header[54];
    if (fread(header, 1, 54, file) != 54 || header[0] != 'B' || header[1] != 'M') {
        std::cout << "Nao e um arquivo BMP valido!" << std::endl;
        fclose(file);
        return;
    }

    // informações de tamanho
    unsigned int dataPos    = *(int*)&(header[0x0A]); // posicao onde começam os pixels
    unsigned int imageSize  = *(int*)&(header[0x22]); // tamanho da imagem em bytes
    unsigned int width      = *(int*)&(header[0x12]);
    unsigned int height     = *(int*)&(header[0x16]);


    if (imageSize == 0)    imageSize = width * height * 3; 
    if (dataPos == 0)      dataPos = 54; 

    // ler os dados (pixels) da imagem
    unsigned char * data = new unsigned char[imageSize];
    fread(data, 1, imageSize, file);
    fclose(file);

    // BMP salva as cores em formato BGR. convertemos para RGB
    for(unsigned int i = 0; i < imageSize; i += 3) {
        unsigned char temp = data[i];     //  azul
        data[i] = data[i+2];              // vermelho -> azul
        data[i+2] = temp;                 // azul -> vermelho
    }

    // enviar a textura para o OpenGL
    glGenTextures(1, &texturaLava);
    glBindTexture(GL_TEXTURE_2D, texturaLava);

    // comportamentos
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // repetir no eixo S (horizontal)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // repetir no eixo T(vertical)

    // filtros da textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // (textura ficar menor)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // (textura ficar maior)

    // envia dados (memoria -> gpu)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    delete[] data; // libera memoria
    std::cout << "Textura carregada: " << width << "x" << height << std::endl;
}


// Trata teclas pressionadas
void Game::KeyDown(unsigned char key) { input.SetKey(key, true); }
void Game::KeyUp(unsigned char key) { input.SetKey(key, false); }
void Game::SpecialKeyDown(int key) { input.SetSpecialKey(key, true); }
void Game::SpecialKeyUp(int key) { input.SetSpecialKey(key, false); }
