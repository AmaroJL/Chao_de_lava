#include "Input.h"

/*
Arquivo que gerencia o input das teclas do jogo
*/

Input::Input() {
    for (int i = 0; i < 256; i++) {
        keys[i] = false;
        specialKeys[i] = false;
    }
}

// Estados de teclas: pressionada (true) e solta (false)

// Atualiza o estado de uma tecla
void Input::SetKey(unsigned char key, bool state) {
    keys[key] = state;
}

// Atualiza o estado de uma tecla especial
void Input::SetSpecialKey(int key, bool state) {
    specialKeys[key] = state;
}

// Verifica se uma tecla foi pressionada
bool Input::IsKeyPressed(unsigned char key) {
    return keys[key];
}

// Verifica se uma tecla especial foi pressionada
bool Input::IsSpecialKeyPressed(int key) {
    return specialKeys[key];
}
