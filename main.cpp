#include <GL/glut.h>
#include <iostream>

#include "cenario.h" 
#include "logica.h"   
#include "camera.h"  
#include "iluminacao.h" 

// Variáveis globais de controle de tela
int larguraTela = 800;
int alturaTela = 600;

// Função de inicialização (Executada uma vez no começo)
void init() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glEnable(GL_DEPTH_TEST); // Ativa a remoção de superfícies ocultas (Z-Buffer)

    configurarIluminacao(); 
    
    inicializarJogador();
}

// Função de Redimensionamento da Janela
void reshape(int w, int h) {
    larguraTela = w;
    alturaTela = h;
    if (h == 0) h = 1;

    glViewport(0, 0, w, h);

    // Uso de projeção perspectiva
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    atualizarCamera(getJogadorX(), getJogadorY(), getJogadorZ());

    desenharCenario();

    desenharJogador();

    glutSwapBuffers();
}

// Função de Captura de Teclado
void keyboard(unsigned char key, int x, int y) {
    processarEntrada(key);
}

// Função de Atualização Física (Timer) - Roda a 60 FPS
void timer(int value) {
    // Atualiza a matemática do pulo (gravidade, movimento)
    atualizarFisica();

    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0); // Chama a cada ~16ms
}

int main(int argc, char** argv) {
    // Configuração inicial do FreeGLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // DOUBLE = Animação fluida, DEPTH = Z-Buffer
    glutInitWindowSize(larguraTela, alturaTela);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Projeto CG - O Rio de Lava");

    init();

    // Registro das funções de Callback
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();

    return 0;
}