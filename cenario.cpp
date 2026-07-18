#include "cenario.h"
#include <GL/glut.h>
#include <iostream>

// BRUNO

// Matriz 2D do mapa (20 de profundidade por 5 de largura)
// 0 = Lava, 1 = Pedra
const int LARGURA_MAPA = 5;
const int PROFUNDIDADE_MAPA = 20;

int mapa[PROFUNDIDADE_MAPA][LARGURA_MAPA] = {
    {1, 1, 1, 1, 1}, // Linha 0 (Início - tudo pedra para não morrer ao nascer)
    {0, 1, 0, 1, 0}, // Linha 1 
    {0, 1, 1, 1, 0}, // Linha 2
    //  Preencher o resto do labirinto até a linha 19
};

void desenharCenario() {
    // Usar dois laços 'for' (um para X e um para Z) percorrendo a matriz.
    // Usar glPushMatrix, glTranslatef(x, 0, z), desenhar o cubo/plano e glPopMatrix.

    // !!!!!!!!!!!Usar glMaterialfv para definir as propriedades
    // ópticas (ka, kd, ks) da lava e das pedras. NÃO USE glColor3f!!!!!!!!!!
    
}

// Vai ser usada por andrey
int getTipoBloco(int gridX, int gridZ) {
    // Se o jogador tentar pular para fora do mapa, consideramos que caiu na lava (0)
    if (gridX < 0 || gridX >= LARGURA_MAPA || gridZ < 0 || gridZ >= PROFUNDIDADE_MAPA) {
        return 0; 
    }
    return mapa[gridZ][gridX];
}