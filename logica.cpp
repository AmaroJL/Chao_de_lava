#include "logica.h"
#include "cenario.h"
#include <GL/glut.h>
#include <iostream>

// ANDREY

// Posição Visual (Onde o cubo (personagem) está sendo desenhado no 3D)
float jogadorX; 
float jogadorY;
float jogadorZ;

// Posição Lógica (Em qual índice da matriz do mapa ele está)
int gridX;
int gridZ;

void inicializarJogador() {
    jogadorX = 2.0f; // Começa no meio do mapa (largura 5, meio é 2)
    jogadorY = 1.0f; 
    jogadorZ = 0.0f;
    gridX = 2;
    gridZ = 0;
}

void processarEntrada(unsigned char key) {
    // O jogador só pode pular se não estiver no meio de outro pulo.
    // Aqui você altera as variáveis lógicas (gridX e gridZ) com base no teclado:
    switch(key) {
        case 'w': case 'W': gridZ++; break; // Vai para frente
        case 's': case 'S': gridZ--; break; // Vai para trás
        case 'a': case 'A': gridX--; break; // Vai para esquerda
        case 'd': case 'D': gridX++; break; // Vai para direita
    }
    
}

void atualizarFisica() {
    // Animação do jogadorX e jogadorZ indo em direção ao gridX e gridZ.
    // Fazer a parábola do pulo alterando o jogadorY.
    
    // VERIFICAÇÃO DE MORTE:
    // Quando o pulo acabar (jogadorY voltar ao chão), verifique o bloco:
    // int blocoChao = getTipoBloco(gridX, gridZ);
    // if (blocoChao == 0) { printf("GAME OVER! Caiu na lava."); }
    // A função getTipoBloco é implemetada por Bruno e retorna se a posição atual do personagem é pedra ou lava
    
}

void desenharJogador() {
    // Desenhar o cubo (ou o .obj importado depois, vamo ver se agente faz importado ou se agente cria o tum tum sahur) na posição jogadorX, Y, Z
    // Lembrar de dar um glPushMatrix() e glPopMatrix() aqui!
    
}

// Getters para a câmera, vai servir pra André
float getJogadorX() { return jogadorX; }
float getJogadorY() { return jogadorY; }
float getJogadorZ() { return jogadorZ; }