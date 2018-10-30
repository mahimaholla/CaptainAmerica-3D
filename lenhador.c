/// PROJETO FINAL
/// Computacao Grafica - 2018.2
/// Ana Julia de Oliveira Bellini - RA 111.774

#include <GL/glut.h>

/* VARIAVEIS GLOBAIS */
int alturaJanela = 700, larguraJanela = 700;
GLfloat correcaoAspecto;
GLfloat anguloProjecao = 45.0;

/* PROJECAO PERSPECTIVA */
void ProjecaoCena() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(anguloProjecao, correcaoAspecto, 0.1, 500);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 80, 200, 0, 0, 0, 0, 1, 0);
}

/* REDIMENSIONAR DESENHO (RESHAPE) */
void redimensionarDesenho(GLsizei largura, GLsizei altura) {
    if (altura == 0) altura = 1;
    glViewport(0, 0, largura, altura);
    correcaoAspecto = (GLfloat) largura / (GLfloat) altura;
    ProjecaoCena();
}

// TODO iluminação
// TODO desenhar cenario
// TODO desenhar personagem

/* FUNCAO PRINCIPAL */
int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(larguraJanela, alturaJanela);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("O Lenhador");

    glutReshapeFunc(redimensionarDesenho);

    glutMainLoop();
    return 0;
}