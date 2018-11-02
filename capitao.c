/// PROJETO FINAL
/// Computacao Grafica - 2018.2
/// Ana Julia de Oliveira Bellini - RA 111.774

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

/* VARIAVEIS GLOBAIS */
int alturaJanela = 700, larguraJanela = 700;

GLfloat correcaoAspecto, anguloProjecao = 45.0;

float posicaoCameraX = 0.0f, posicaoCameraY = 0.0f, posicaoCameraZ = 0.0f;
float anguloCameraX = 0.0f, anguloCameraY = 0.0f, anguloCameraZ = 0.0f;
float anguloCenaX = 0.0f, anguloCenaY = 0.0f, anguloCenaZ = 0.0f;

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
// TODO fila de toras
// TODO desenho do escudo
/* DESENHO DO CAPITAO AMERICA */

void desenharPersonagem() {
    glPushMatrix();
        // cabeca
        glPushMatrix();

        glPopMatrix();

        // pescoco
        glPushMatrix();

        glPopMatrix();

        // tronco
        glPushMatrix();

        glPopMatrix();

        // braco direito
        glPushMatrix();

        glPopMatrix();

        // braco esquerdo
        glPushMatrix();

        glPopMatrix();

        // perna direita
        glPushMatrix();

        glPopMatrix();

        // perna esquerda
        glPushMatrix();

        glPopMatrix();
    glPopMatrix();
}

/* DESENHO DO CENARIO */
void troncoArvore() {
    int i; float anguloTronco;

    glPushMatrix();
        glColor3f(0.26f, 0.0f, 0.0f);

        // tronco
        glBegin(GL_QUAD_STRIP);
        for (i = 0; i <= 20; i++) {
            anguloTronco = (float) (2 * M_PI * i / 20.0f);
            glVertex3f(0.02f * cosf(anguloTronco), 0.0f, 0.02f * sinf(anguloTronco));
            glVertex3f(0.02f * cosf(anguloTronco), 0.1f, 0.02f * sinf(anguloTronco));
        }
        glEnd();

        glBegin(GL_POLYGON);
        for (i = 0; i < 20; i++) {
            anguloTronco = (float) (2 * M_PI * i / 20.0f);
            glVertex3f(0.02f * cosf(anguloTronco), 0.0f, 0.02f * sinf(anguloTronco));
        }
        glEnd();

        glBegin(GL_POLYGON);
        for (i = 0; i < 20; i++) {
            anguloTronco = (float) (2 * M_PI * i / 20.0);
            glVertex3f(0.02f * cosf(anguloTronco), 0.1f, 0.02f * sinf(anguloTronco));
        }
        glEnd();
    glPopMatrix();
}

/* DESENHO DO CENARIO */
void desenharCenario() {
    // grama
    glPushMatrix();
        glColor3f(0.01f, 0.45f, 0.02f);
        glTranslatef(0.0f, -0.25f, -0.25f);
        glScalef(20.0f, 0.5f, 15.0f);
        glutSolidCube(0.05f);
    glPopMatrix();

    // tronco da arvore
    troncoArvore();

    // TODO desenhar folhas da árvore
}

/* DESENHAR CENA COMPLETA */
void desenharCenaCompleta() {
    glClearColor(0.74902, 0.847059, 0.847059, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
        glRotatef(anguloCenaX, 1.0, 0.0, 0.0);
        glRotatef(anguloCenaY, 0.0, 1.0, 0.0);
        glRotatef(anguloCenaZ, 0.0, 0.0, 1.0);
        desenharCenario();
        desenharPersonagem();
    glPopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();
}

/* MOVIMENTAR CENA NO TECLADO */
void leituraTeclado(unsigned char tecla, int x, int y) {
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
        case 's':   // diminuir angulo da cena em X
            anguloCenaX -= 2;
            break;
        case 'S':   // aumentar angulo da cena em X
            anguloCenaX += 2;
            break;
        case 'f':   // diminuir angulo da cena em Y
            anguloCenaY -= 2;
            break;
        case 'F':   // aumentar angulo da cena em Y
            anguloCenaY += 2;
            break;
        case 'c':   // diminuir angulo da cena em Z
            anguloCenaZ -= 2;
            break;
        case 'C':   // aumentar angulo da cena em Z
            anguloCenaZ += 2;
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(larguraJanela, alturaJanela);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("O Lenhador");

    glutReshapeFunc(redimensionarDesenho);
    glutKeyboardFunc(leituraTeclado);

    glutDisplayFunc(desenharCenaCompleta);
    glutMainLoop();
    return 0;
}