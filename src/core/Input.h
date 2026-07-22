#ifndef INPUT_H
#define INPUT_H

/*
Cabeçalho do input
*/

class Input {
public:
    bool keys[256];
    bool specialKeys[256];

    Input();

    void SetKey(unsigned char key, bool state);
    void SetSpecialKey(int key, bool state);
    
    bool IsKeyPressed(unsigned char key);
    bool IsSpecialKeyPressed(int key);
};

#endif
