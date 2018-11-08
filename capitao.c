/// PROJETO FINAL
/// Computacao Grafica - 2018.2
/// Ana Julia de Oliveira Bellini - RA 111.774

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

/* VARIAVEIS GLOBAIS */
int alturaJanela = 700, larguraJanela = 700;

GLfloat correcaoAspecto, anguloProjecao = 45.0;

float posicaoCameraX = 0.0f, posicaoCameraY = 0.0f, posicaoCameraZ = 0.0f, anguloCenaX = 0.0f, anguloCenaY = 0.0f,
anguloCenaZ = 0.0f, anguloCameraX = 0.0f, anguloCameraY = 0.0f, anguloCameraZ = 0.0f, posicaoPersonagemX = 0.0f,
posicaoPersonagemY = 0.0f, posicaoPersonagemZ = 0.0f, anguloCoxaE = 0.0f, anguloCoxaD = 0.0f, anguloCanelaE = 0.0f,
anguloCanelaD = 45.0f, auxCoxaE = 1.0f, auxCoxaD = 1.0f, auxCanelaE = 1.0f, auxCanelaD = 1.0f, anguloOmbroE = 0.0f,
anguloOmbroD = 0.0f, anguloCotoveloE = 90.0f, anguloCotoveloD = 0.0f, auxOmbroE = 1.0f, auxOmbroD = 1.0f,
auxCotoveloE = 1.0f, auxCotoveloD = 1.0f, escudoX, escudoY = -0.3f, escudoZ;

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

/* ILUMINACAO DA CENA */
void iluminarCenario() {
    GLfloat luzAmbiente[4] = {0.3, 0.3, 0.3, 1.0};
    GLfloat luzDifusa[4] = {0.7, 0.7, 0.7, 1.0};
    GLfloat luzEspecular[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat posicaoLuz[4] = {-50.0f, 50.0, 50.0, 1.0};
    GLfloat objetoAmbiente[4] = {0.5, 0.0, 0.0, 1.0};
    GLfloat objetoDifusa[4] = {1.0,0.0,0.0,1.0};
    GLfloat especularidade[4] = {0.5, 0.5, 0.5, 1.0};
    GLint especMaterial = 30;

    glClearColor(0.74902, 0.847059, 0.847059, 0.0);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

    glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, objetoAmbiente);
    glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, objetoDifusa);
    glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
    glMateriali(GL_FRONT,GL_SHININESS, especMaterial);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}

// TODO aplicação de textura

/* ESTRUTURA DA FILA DE ESCUDOS */
typedef struct escudo {
    float posicaoEscudoX;
    float posicaoEscudoY;
    float posicaoEscudoZ;
    struct escudo *proximoEscudo;
} filaEscudos;
filaEscudos *primeiroEscudo = NULL;

/* INSERCAO DOS ESCUDOS NO FINAL DA FILA */
void inserirEscudo(float posicaoX, float posicaoY, float posicaoZ) {
    // primeiro escudo da fila
    if (primeiroEscudo == NULL) {
        filaEscudos *novoEscudo = (filaEscudos*)malloc(sizeof(filaEscudos));
        novoEscudo->posicaoEscudoX = posicaoX;
        novoEscudo->posicaoEscudoY = posicaoY;
        novoEscudo->posicaoEscudoZ = posicaoZ;
        novoEscudo->proximoEscudo = primeiroEscudo;
        primeiroEscudo = novoEscudo;
        return;
    }

    // outras toras da fila, entrando no final
    filaEscudos *auxFila = primeiroEscudo, *novoEscudo;

    while (auxFila->proximoEscudo != NULL) auxFila = auxFila->proximoEscudo;

    novoEscudo = (filaEscudos*)malloc(sizeof(filaEscudos));
    novoEscudo->posicaoEscudoX = posicaoX;
    novoEscudo->posicaoEscudoY = posicaoY;
    novoEscudo->posicaoEscudoZ = posicaoZ;
    novoEscudo->proximoEscudo = NULL;
    auxFila->proximoEscudo = novoEscudo;
}

/* REMOCAO DO ESCUDO NO INICIO DA FILA */
void removerEscudo(void) {
    if (primeiroEscudo != NULL) {
        filaEscudos *auxFila = primeiroEscudo;
        primeiroEscudo = auxFila->proximoEscudo;
        free(auxFila);
    }
}

/* DESENHO DO CAPITAO AMERICA */
void cabeca() {
    int i; float angulo;

    // TODO desenhar cabelo e barba com splines
    // TODO aplicar textura para o rosto

    // cabeca
    glPushMatrix();
        glColor3f(0.92f, 0.79f, 0.49f);
        glScalef(1.0f, 1.2f, 1.0f);
        glutSolidSphere(0.03, 50, 50);
    glPopMatrix();

    // pescoco
    glPushMatrix();
        glColor3f(0.92f, 0.79f, 0.49f);
        glTranslatef(0.0f, -0.05f, 0.0f);

        glBegin(GL_QUAD_STRIP);
            for (i = 0; i <= 20; i++) {
                angulo = (float) (2 * M_PI * i / 20.0f);
                glVertex3f(0.01f * cosf(angulo), 0.0f, 0.01f * sinf(angulo));
                glVertex3f(0.01f * cosf(angulo), 0.05f, 0.01f * sinf(angulo));
            }
        glEnd();

        glBegin(GL_POLYGON);
            for (i = 0; i < 20; i++) {
                angulo = (float) (2 * M_PI * i / 20.0f);
                glVertex3f(0.01f * cosf(angulo), 0.0f, 0.01f * sinf(angulo));
            }
        glEnd();

        glBegin(GL_POLYGON);
            for (i = 0; i < 20; i++) {
                angulo = (float) (2 * M_PI * i / 20.0);
                glVertex3f(0.01f * cosf(angulo), 0.05f, 0.01f * sinf(angulo));
            }
        glEnd();
    glPopMatrix();
}

void tronco() {
    int i; float angulo;

    // TODO aplicar textura no corpo

    glPushMatrix();
        glColor3f(0.0f, 0.24f, 0.41f);
        glTranslatef(0.0f, -0.07f, 0.0f);
        glScalef(1.5f, 0.8f, 1.0f);

        // peitoral
        glPushMatrix();
            glutSolidSphere(0.03, 50, 50);
        glPopMatrix();

        // asas de maromba
        glPushMatrix();
            glRotatef(90.0, 1.0, 0.0, 0.0);
            glutSolidCone(0.03f, 0.09f, 50, 50);
        glPopMatrix();

        // barriga
        glPushMatrix();
            glTranslatef(0.0f, -0.1f, 0.0f);
            glBegin(GL_QUAD_STRIP);
                for (i = 0; i <= 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.015f * cosf(angulo), 0.0f, 0.015f * sinf(angulo));
                    glVertex3f(0.015f * cosf(angulo), 0.1f, 0.015f * sinf(angulo));
                }
            glEnd();

            glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.015f * cosf(angulo), 0.0f, 0.015f * sinf(angulo));
                }
            glEnd();

            glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0);
                    glVertex3f(0.015f * cosf(angulo), 0.1f, 0.015f * sinf(angulo));
                }
            glEnd();

            // cinto
            glPushMatrix();
                glColor3f(0.0f, 0.0f, 0.0f);
                glTranslatef(0.0f, -0.01f, 0.0f);
                glScalef(1.0f, 0.1f, 1.0f);

                glBegin(GL_QUAD_STRIP);
                for (i = 0; i <= 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.015f * cosf(angulo), 0.0f, 0.015f * sinf(angulo));
                    glVertex3f(0.015f * cosf(angulo), 0.1f, 0.015f * sinf(angulo));
                }
                glEnd();

                glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.015f * cosf(angulo), 0.0f, 0.015f * sinf(angulo));
                }
                glEnd();

                glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0);
                    glVertex3f(0.015f * cosf(angulo), 0.1f, 0.015f * sinf(angulo));
                }
                glEnd();
            glPopMatrix();

            // quadril
            glPushMatrix();
                glColor3f(0.0f, 0.16f, 0.27f);
                glTranslatef(0.0f, -0.04f, 0.0f);
                glScalef(1.0f, 0.5f, 1.0f);

                glBegin(GL_QUAD_STRIP);
                for (i = 0; i <= 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.015f * cosf(angulo), 0.0f, 0.015f * sinf(angulo));
                    glVertex3f(0.015f * cosf(angulo), 0.1f, 0.015f * sinf(angulo));
                }
                glEnd();

                glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.015f * cosf(angulo), 0.0f, 0.015f * sinf(angulo));
                }
                glEnd();

                glBegin(GL_POLYGON);
                    for (i = 0; i < 20; i++) {
                        angulo = (float) (2 * M_PI * i / 20.0);
                        glVertex3f(0.015f * cosf(angulo), 0.1f, 0.015f * sinf(angulo));
                    }
                glEnd();

                // bumbum
                glPushMatrix();
                    glTranslatef(0.0f, 0.027f, -0.01f);
                    glScalef(1.1f, 3.0f, 1.2f);
                    glutSolidSphere(0.01f, 50, 50);
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

void bracoDireito() {
    int i; float angulo;

    glPushMatrix();
        // braco direito
        glPushMatrix();
            glColor3f(0.0f, 0.24f, 0.41f);
            glTranslatef(0.035f, -0.11f, 0.0f);
            glTranslatef(0.0f, 0.04f, 0.0f);
            glRotatef(anguloOmbroD, 1.0, 0.0, 0.0);
            glTranslatef(0.0f, -0.04f, 0.0f);

            glBegin(GL_QUAD_STRIP);
                for (i = 0; i <= 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.009f * cosf(angulo), 0.0f, 0.009f * sinf(angulo));
                    glVertex3f(0.009f * cosf(angulo), 0.04f, 0.009f * sinf(angulo));
                }
            glEnd();

            glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.009f * cosf(angulo), 0.0f, 0.009f * sinf(angulo));
                }
            glEnd();

            glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0);
                    glVertex3f(0.009f * cosf(angulo), 0.04f, 0.009f * sinf(angulo));
                }
            glEnd();

            // cotovelo direito
            glPushMatrix();
                glutSolidSphere(0.009f, 50, 50);
            glPopMatrix();

            // antebraco direito
            glPushMatrix();
                glColor3f(0.72f, 0.0f, 0.0f);
                glTranslatef(0.0f, -0.04f, 0.0f);
                glTranslatef(0.0f, 0.04f, 0.0f);
                glRotatef(-anguloCotoveloD, 1.0, 0.0, 0.0);
                glTranslatef(0.0f, -0.04f, 0.0f);

                glBegin(GL_QUAD_STRIP);
                    for (i = 0; i <= 20; i++) {
                        angulo = (float) (2 * M_PI * i / 20.0f);
                        glVertex3f(0.01f * cosf(angulo), 0.0f, 0.01f * sinf(angulo));
                        glVertex3f(0.01f * cosf(angulo), 0.04f, 0.01f * sinf(angulo));
                    }
                glEnd();

                glBegin(GL_POLYGON);
                    for (i = 0; i < 20; i++) {
                        angulo = (float) (2 * M_PI * i / 20.0f);
                        glVertex3f(0.01f * cosf(angulo), 0.0f, 0.01f * sinf(angulo));
                    }
                glEnd();

                glBegin(GL_POLYGON);
                    for (i = 0; i < 20; i++) {
                        angulo = (float) (2 * M_PI * i / 20.0);
                        glVertex3f(0.01f * cosf(angulo), 0.04f, 0.01f * sinf(angulo));
                    }
                glEnd();

                // mao direita
                glPushMatrix();
                    glTranslatef(0.0f, -0.011f, 0.0f);
                    glScalef(0.3f, 1.1f, 0.8f);
                    glutSolidCube(0.021f);
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

void bracoEsquerdo() {
    int i; float angulo;

    glPushMatrix();
        // braco esquerdo
        glPushMatrix();
            glColor3f(0.0f, 0.24f, 0.41f);
            glTranslatef(-0.035f, -0.11f, 0.0f);
            glTranslatef(0.0f, 0.04f, 0.0f);
            glRotatef(anguloOmbroE, 1.0, 0.0, 0.0);
            glTranslatef(0.0f, -0.04f, 0.0f);

            glBegin(GL_QUAD_STRIP);
                for (i = 0; i <= 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.009f * cosf(angulo), 0.0f, 0.009f * sinf(angulo));
                    glVertex3f(0.009f * cosf(angulo), 0.04f, 0.009f * sinf(angulo));
                }
            glEnd();

            glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.009f * cosf(angulo), 0.0f, 0.009f * sinf(angulo));
                }
            glEnd();

            glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0);
                    glVertex3f(0.009f * cosf(angulo), 0.04f, 0.009f * sinf(angulo));
                }
            glEnd();

            // cotovelo esquerdo
            glPushMatrix();
                glutSolidSphere(0.009f, 50, 50);
            glPopMatrix();

            // antebraco esquerdo
            glPushMatrix();
                glColor3f(0.72f, 0.0f, 0.0f);
                glTranslatef(0.0f, -0.04f, 0.0f);
                glTranslatef(0.0f, 0.04f, 0.0f);
                glRotatef(-anguloCotoveloE, 1.0, 0.0, 0.0);
                glTranslatef(0.0f, -0.04f, 0.0f);

                glBegin(GL_QUAD_STRIP);
                    for (i = 0; i <= 20; i++) {
                        angulo = (float) (2 * M_PI * i / 20.0f);
                        glVertex3f(0.01f * cosf(angulo), 0.0f, 0.01f * sinf(angulo));
                        glVertex3f(0.01f * cosf(angulo), 0.04f, 0.01f * sinf(angulo));
                    }
                glEnd();

                glBegin(GL_POLYGON);
                    for (i = 0; i < 20; i++) {
                        angulo = (float) (2 * M_PI * i / 20.0f);
                        glVertex3f(0.01f * cosf(angulo), 0.0f, 0.01f * sinf(angulo));
                    }
                glEnd();

                glBegin(GL_POLYGON);
                    for (i = 0; i < 20; i++) {
                        angulo = (float) (2 * M_PI * i / 20.0);
                        glVertex3f(0.01f * cosf(angulo), 0.04f, 0.01f * sinf(angulo));
                    }
                glEnd();

                // mao esquerda
                glPushMatrix();
                    glTranslatef(0.0f, -0.011f, 0.0f);
                    glScalef(0.3f, 1.1f, 0.8f);
                    glutSolidCube(0.021f);
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

void pernaDireita() {
    int i; float angulo;

    glPushMatrix();
        // coxa direita
        glPushMatrix();
            glColor3f(0.0f, 0.16f, 0.27f);
            glTranslatef(0.012f, -0.21f, 0.0f);
            glTranslatef(0.0f, 0.04f, 0.0f);
            glRotatef(-anguloCoxaD, 1.0, 0.0, 0.0);
            glTranslatef(0.0f, -0.04f, 0.0f);

            glBegin(GL_QUAD_STRIP);
                for (i = 0; i <= 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.01f * cosf(angulo), 0.0f, 0.01f * sinf(angulo));
                    glVertex3f(0.01f * cosf(angulo), 0.04f, 0.01f * sinf(angulo));
                }
            glEnd();

            glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.01f * cosf(angulo), 0.0f, 0.01f * sinf(angulo));
                }
            glEnd();

            glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0);
                    glVertex3f(0.01f * cosf(angulo), 0.04f, 0.01f * sinf(angulo));
                }
            glEnd();

            // joelho direito
            glPushMatrix();
                glutSolidSphere(0.01f, 50, 50);
            glPopMatrix();

            // canela direita
            glPushMatrix();
                glColor3f(0.15f, 0.17f, 0.19f);
                glTranslatef(0.0f, -0.04f, 0.0f);
                glTranslatef(0.0f, 0.04f, 0.0f);
                glRotatef(anguloCanelaD, 1.0, 0.0, 0.0);
                glTranslatef(0.0f, -0.04f, 0.0f);

                glBegin(GL_QUAD_STRIP);
                    for (i = 0; i <= 20; i++) {
                        angulo = (float) (2 * M_PI * i / 20.0f);
                        glVertex3f(0.01f * cosf(angulo), 0.0f, 0.01f * sinf(angulo));
                        glVertex3f(0.01f * cosf(angulo), 0.04f, 0.01f * sinf(angulo));
                    }
                glEnd();

                glBegin(GL_POLYGON);
                    for (i = 0; i < 20; i++) {
                        angulo = (float) (2 * M_PI * i / 20.0f);
                        glVertex3f(0.01f * cosf(angulo), 0.0f, 0.01f * sinf(angulo));
                    }
                glEnd();

                glBegin(GL_POLYGON);
                    for (i = 0; i < 20; i++) {
                        angulo = (float) (2 * M_PI * i / 20.0);
                        glVertex3f(0.01f * cosf(angulo), 0.04f, 0.01f * sinf(angulo));
                    }
                glEnd();

                // pe direito
                glPushMatrix();
                    glTranslatef(0.0f, -0.001f, 0.007f);
                    glScalef(1.0f, 1.0f, 1.7f);
                    glutSolidCube(0.02f);
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

void pernaEsquerda() {
    int i; float angulo;

    glPushMatrix();
        // coxa esquerda
        glPushMatrix();
            glColor3f(0.0f, 0.16f, 0.27f);
            glTranslatef(-0.012f, -0.21f, 0.0f);
            glTranslatef(0.0f, 0.04f, 0.0f);
            glRotatef(anguloCoxaE, 1.0, 0.0, 0.0);
            glTranslatef(0.0f, -0.04f, 0.0f);

            glBegin(GL_QUAD_STRIP);
                for (i = 0; i <= 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.01f * cosf(angulo), 0.0f, 0.01f * sinf(angulo));
                    glVertex3f(0.01f * cosf(angulo), 0.04f, 0.01f * sinf(angulo));
                }
            glEnd();

            glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.01f * cosf(angulo), 0.0f, 0.01f * sinf(angulo));
                }
            glEnd();

            glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0);
                    glVertex3f(0.01f * cosf(angulo), 0.04f, 0.01f * sinf(angulo));
                }
            glEnd();

            // joelho esquerdo
            glPushMatrix();
                glutSolidSphere(0.01f, 50, 50);
            glPopMatrix();

            // canela esquerda
            glPushMatrix();
                glColor3f(0.15f, 0.17f, 0.19f);
                glTranslatef(0.0f, -0.04f, 0.0f);
                glTranslatef(0.0f, 0.04f, 0.0f);
                glRotatef(anguloCanelaE, 1.0, 0.0, 0.0);
                glTranslatef(0.0f, -0.04f, 0.0f);

                glBegin(GL_QUAD_STRIP);
                    for (i = 0; i <= 20; i++) {
                        angulo = (float) (2 * M_PI * i / 20.0f);
                        glVertex3f(0.01f * cosf(angulo), 0.0f, 0.01f * sinf(angulo));
                        glVertex3f(0.01f * cosf(angulo), 0.04f, 0.01f * sinf(angulo));
                    }
                glEnd();

                glBegin(GL_POLYGON);
                    for (i = 0; i < 20; i++) {
                        angulo = (float) (2 * M_PI * i / 20.0f);
                        glVertex3f(0.01f * cosf(angulo), 0.0f, 0.01f * sinf(angulo));
                    }
                glEnd();

                glBegin(GL_POLYGON);
                    for (i = 0; i < 20; i++) {
                        angulo = (float) (2 * M_PI * i / 20.0);
                        glVertex3f(0.01f * cosf(angulo), 0.04f, 0.01f * sinf(angulo));
                    }
                glEnd();

                // pe esquerdo
                glPushMatrix();
                    glTranslatef(0.0f, -0.001f, 0.007f);
                    glScalef(1.0f, 1.0f, 1.7f);
                    glutSolidCube(0.02f);
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

void desenharPersonagem() {
    glPushMatrix();
        glTranslatef(posicaoPersonagemX, 0.0f, 0.0f);
        glTranslatef(0.0f, posicaoPersonagemY, 0.0f);
        glTranslatef(0.0f, 0.0f, posicaoPersonagemZ);

        cabeca();
        tronco();
        bracoDireito();
        bracoEsquerdo();
        pernaDireita();
        pernaEsquerda();
    glPopMatrix();
}

/* DESENHO DO ESCUDO */
void desenharEscudo(float posX, float posY, float posZ) {
    // TODO desenhar escudo com splines
    // TODO aplicar textura no escudo

    glPushMatrix();
        glColor3f(0.0f, 0.02f, 0.31f);
        glTranslatef(posX, posY, posZ);
        glRotatef(90.0, 1.0, 0.0, 0.0);
        glutSolidTorus(0.02, 0.03, 50, 50);
    glPopMatrix();
}

/* DESENHO DO CENARIO */
void folhaArvore() {
    glPushMatrix();
        glColor3f(0.04f, 0.31f, 0.0f);
        glTranslatef(0.0f, 0.15f, 0.25f);
        glRotatef(-90.0, 1.0, 0.0, 0.0);
        glTranslatef(0.0f, 0.25f, 0.0f);
        glutSolidCone(0.15, 0.3, 50, 50);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0.04f, 0.31f, 0.0f);
        glTranslatef(0.0f, 0.30f, 0.25f);
        glRotatef(-90.0, 1.0, 0.0, 0.0);
        glTranslatef(0.0f, 0.25f, 0.0f);
        glutSolidCone(0.12, 0.25, 50, 50);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0.04f, 0.31f, 0.0f);
        glTranslatef(0.0f, 0.45f, 0.25f);
        glRotatef(-90.0, 1.0, 0.0, 0.0);
        glTranslatef(0.0f, 0.25f, 0.0f);
        glutSolidCone(0.09, 0.2, 50, 50);
    glPopMatrix();
}

void troncoArvore(float posicaoX, float posicaoY, float posicaoZ) {
    int i; float anguloTronco;

    glPushMatrix();
        glTranslatef(posicaoX, posicaoY, posicaoZ);
        glColor3f(0.26f, 0.0f, 0.0f);

        // tronco
        glBegin(GL_QUAD_STRIP);
            for (i = 0; i <= 20; i++) {
                anguloTronco = (float) (2 * M_PI * i / 20.0f);
                glVertex3f(0.04f * cosf(anguloTronco), 0.0f, 0.04f * sinf(anguloTronco));
                glVertex3f(0.04f * cosf(anguloTronco), 0.2f, 0.04f * sinf(anguloTronco));
            }
        glEnd();

        glBegin(GL_POLYGON);
            for (i = 0; i < 20; i++) {
                anguloTronco = (float) (2 * M_PI * i / 20.0f);
                glVertex3f(0.04f * cosf(anguloTronco), 0.0f, 0.04f * sinf(anguloTronco));
            }
        glEnd();

        glBegin(GL_POLYGON);
            for (i = 0; i < 20; i++) {
                anguloTronco = (float) (2 * M_PI * i / 20.0);
                glVertex3f(0.04f * cosf(anguloTronco), 0.2f, 0.04f * sinf(anguloTronco));
            }
        glEnd();

        folhaArvore();
    glPopMatrix();
}

void desenharCenario() {
    // posicoes da primeira arvore (canto superior esquerdo da grama), servirao de base para desenhar as outras
    float posicaoX = -2.25f;
    float posicaoY = -0.3f;
    float posicaoZ = -1.25f;

    // auxiliar para desenhar escudos
    filaEscudos *auxEscudo;

    // grama
    glPushMatrix();
        glColor3f(0.01f, 0.45f, 0.02f);
        glTranslatef(0.0f, -0.3f, -0.25f);
        glScalef(100.0f, 0.5f, 50.0f);
        glutSolidCube(0.05f);
    glPopMatrix();

    // arvores do fundo
    for (float i = 0; i <= 4.5; i = i + 0.25f) troncoArvore((posicaoX + i), posicaoY, posicaoZ);

    // arvores da frente
    for (float i = 0; i <= 4.5; i = i + 0.25f) troncoArvore((posicaoX + i), posicaoY, (posicaoZ + 2.0f));

    // arvores da esquerda
    for (float i = 0; i <= 2.0; i = i + 0.25f) troncoArvore(posicaoX, posicaoY, (posicaoZ + i));

    // arvores da direita
    for (float i = 0; i <= 2.0; i = i + 0.25f) troncoArvore((posicaoX + 4.5f), posicaoY, (posicaoZ + i));

    // desenhar escudos, se houver
    if (primeiroEscudo != NULL)
        for (auxEscudo = primeiroEscudo; auxEscudo != NULL; auxEscudo = auxEscudo->proximoEscudo)
            desenharEscudo(auxEscudo->posicaoEscudoX, (auxEscudo->posicaoEscudoY + 0.03f), auxEscudo->posicaoEscudoZ);
}

/* DESENHAR CENA COMPLETA */
void desenharCenaCompleta() {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

/* CAMINHADA DO CAPITAO AMERICA */
void caminhadaPersonagem() {
    // ombro esquerdo
    if (anguloOmbroE == 45 || anguloOmbroE == -45) auxOmbroE = -auxOmbroE;
    anguloOmbroE -= auxOmbroE;

    // cotovelo esquerdo
    if (anguloCotoveloE >= 90 || anguloCotoveloE < 0) auxCotoveloE = -auxCotoveloE;
    anguloCotoveloE += auxCotoveloE;

    // ombro direito
    if (anguloOmbroD == 45 || anguloOmbroD == -45) auxOmbroD = -auxOmbroD;
    anguloOmbroD += auxOmbroD;

    // cotovelo direito
    if (anguloCotoveloD >= 90 || anguloCotoveloD < 0) auxCotoveloD = -auxCotoveloD;
    anguloCotoveloD += auxCotoveloD;

    // coxa esquerda
    if (anguloCoxaE == 45 || anguloCoxaE == -45) auxCoxaE = -auxCoxaE;
    anguloCoxaE += auxCoxaE;

    // canela esquerda
    if (anguloCanelaE < 0 || anguloCanelaE == 50) auxCanelaE = -auxCanelaE;
    anguloCanelaE -= auxCanelaE;

    // coxa direita
    if (anguloCoxaD == 45 || anguloCoxaD == -45) auxCoxaD = -auxCoxaD;
    anguloCoxaD += auxCoxaD;

    // canela direita
    if (anguloCanelaD < 0 || anguloCanelaD == 50) auxCanelaD = -auxCanelaD;
    anguloCanelaD -= auxCanelaD;

    ProjecaoCena();
    glutPostRedisplay();
    glutTimerFunc(10, caminhadaPersonagem, 1);
}

/* MOVIMENTACAO DO PERSONAGEM */
void leituraSetas(int tecla) {
    switch (tecla) {
        case GLUT_KEY_RIGHT:
            posicaoPersonagemX += 0.005;
            break;

        case GLUT_KEY_LEFT:
            posicaoPersonagemX -= 0.005;
            break;

        case GLUT_KEY_UP:
            posicaoPersonagemZ += 0.005;
            break;

        case GLUT_KEY_DOWN:
            posicaoPersonagemZ -= 0.005;
            break;

        default:
            glutIdleFunc(NULL);
            break;
    }

    ProjecaoCena();
    glutPostRedisplay();
}

/* CRIACAO DOS ESCUDOS */
void criarEscudo(int botao, int estado) {
    if ((botao == GLUT_LEFT_BUTTON) && (estado == GLUT_DOWN)) {
        // gerar posicao aleatoria para o escudo
        escudoX = ((rand() * 4.5f)/(RAND_MAX)) - 2.25f;
        escudoZ = ((rand() * 2.0f)/(RAND_MAX)) - 1.0f;

        // criar escudo (adicionar na fila)
        inserirEscudo(escudoX, escudoY, escudoZ);
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
    glutCreateWindow("Captain America");

    glutReshapeFunc(redimensionarDesenho);
    glutKeyboardFunc(leituraTeclado);
    glutSpecialFunc(leituraSetas);
    glutMouseFunc(criarEscudo);

    glutDisplayFunc(desenharCenaCompleta);
    glutTimerFunc(10, caminhadaPersonagem, 0);
    iluminarCenario();
    glutMainLoop();
    return 0;
}