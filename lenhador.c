/// PROJETO FINAL
/// Computacao Grafica - 2018.2
/// Ana Julia de Oliveira Bellini - RA 111.774

#include <GL/glut.h>
#include <stdio.h>

/* VARIAVEIS GLOBAIS */
int alturaJanela = 700, larguraJanela = 700;

GLfloat correcaoAspecto, anguloProjecao = 45.0;

float posicaoCameraX = 0.0f, posicaoCameraY = 0.0f, posicaoCameraZ = 0.0f;
float anguloCameraX = 0.0f, anguloCameraY = 0.0f, anguloCameraZ = 0.0f;

/* PROJECAO PERSPECTIVA */
void ProjecaoCena() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(anguloProjecao, correcaoAspecto, 0.1, 800);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(posicaoCameraX, posicaoCameraY, posicaoCameraZ);
    glRotatef(anguloCameraZ, 0.0, 0.0, 1.0);
    glRotatef(anguloCameraY, 0.0, 1.0, 0.0);
    glRotatef(anguloCameraX, 1.0, 0.0, 0.0);
    gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);
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

/*  */

/* DESENHO DO LENHADOR */
void desenharLenhador() {

}

/* DESENHO DO CENARIO */

/* DESENHAR CENA */
void desenharCena() {
    glClearColor(0.74902, 0.847059, 0.847059, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        glutWireCube(0.1);
    glPopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();
}

/* MOVIMENTAR CENA NO TECLADO */
void leituraTeclado(unsigned char tecla) {
    switch (tecla) {
        case 'x':   // trazer camera para a esquerda
            posicaoCameraX -= 0.01;
            break;
        case 'X':   // trazer camera para a direita
            posicaoCameraX += 0.01;
            break;
        case 'y':   // trazer camera para baixo
            posicaoCameraY -= 0.01;
            break;
        case 'Y':   // trazer camera para cima
            posicaoCameraY += 0.01;
            break;
        case 'z':   // trazer camera para tras
            posicaoCameraZ -= 0.01;
            break;
        case 'Z':   // trazer camera para a frente
            posicaoCameraZ += 0.01;
            break;
        case 'w':   // diminuir angulo da camera em X
            anguloCameraX -= 2;
            break;
        case 'W':   // aumentar angulo da camera em X
            anguloCameraX += 2;
            break;
        case 'a':   // diminuir angulo da camera em Y
            anguloCameraY -= 2;
            break;
        case 'A':   // aumentar angulo da camera em Y
            anguloCameraY += 2;
            break;
        case 'd':   // diminuir angulo da camera em Z
            anguloCameraZ -= 2;
            break;
        case 'D':   // aumentar angulo da camera em Z
            anguloCameraZ += 2;
            break;
        default:
            glutIdleFunc(NULL);
            break;
    }

    ProjecaoCena();
    glutPostRedisplay();
}

/* FUNCAO PRINCIPAL */
int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(larguraJanela, alturaJanela);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("O Lenhador");

    glutReshapeFunc(redimensionarDesenho);
    glutKeyboardFunc(leituraTeclado);

    glutDisplayFunc(desenharCena);
    glutMainLoop();
    return 0;
}