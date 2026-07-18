#ifndef LOGICA_H
#define LOGICA_H

void inicializarJogador();
void processarEntrada(unsigned char key);
void atualizarFisica();
void desenharJogador();

float getJogadorX();
float getJogadorY();
float getJogadorZ();

#endif