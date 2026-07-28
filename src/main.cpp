#include <GL/glut.h>

#include "core/Game.h"
#include <iostream>

Game game;

void display() { 
    game.Render();
}

void timer(int value) { 
    game.Update(); 
    glutPostRedisplay(); 
    glutTimerFunc(16, timer, 0); 
    
}

void keyboard(unsigned char key, int x, int y) { game.KeyDown(key); }
void keyboardUp(unsigned char key, int x, int y) { game.KeyUp(key); }
void specialKeys(int key, int x, int y) { game.SpecialKeyDown(key); }
void specialKeysUp(int key, int x, int y) { game.SpecialKeyUp(key); }

int main(int argc, char** argv) {
    glutInit(&argc, argv);  
    // Adiciona o GLUT_STENCIL para criar o buffer de máscara
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 500);
    glutInitWindowPosition(200, 0);
    glutCreateWindow("Jogo 3D");

    
    game.Init();
    
    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutSpecialUpFunc(specialKeysUp);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    
    glutTimerFunc(0, timer, 0);

    glutMainLoop();    

    return 0;
}
