/// PROJETO FINAL
/// Computacao Grafica - 2018.2
/// Ana Julia de Oliveira Bellini - RA 111.774

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

/* VARIAVEIS GLOBAIS */
int alturaJanela = 700, larguraJanela = 700, flagCaminhada = 0, flagImpulso = 0, flagEscudo = 0, flagJogar = 0;

GLfloat correcaoAspecto, anguloProjecao = 45.0;

float posicaoCameraX = 0.0f, posicaoCameraY = 0.0f, posicaoCameraZ = 0.0f, anguloCenaX = 0.0f, anguloCenaY = 0.0f,
anguloCenaZ = 0.0f, anguloCameraX = 0.0f, anguloCameraY = 0.0f, anguloCameraZ = 0.0f, posicaoPersonagemX = -1.06f,
posicaoPersonagemY = 0.0f, posicaoPersonagemZ = -0.15f, anguloCoxaE = 0.0f, anguloCoxaD = 0.0f, anguloCanelaE = 0.0f,
anguloCanelaD = 45.0f, auxCoxaE = 2.0f, auxCoxaD = 2.0f, auxCanelaE = 2.0f, auxCanelaD = 2.0f, anguloOmbroE = 0.0f,
anguloOmbroD = 0.0f, anguloCotoveloE = 90.0f, anguloCotoveloD = 0.0f, auxOmbroE = 2.0f, auxOmbroD = 2.0f,
auxCotoveloE = 2.0f, auxCotoveloD = 2.0f, escudoX, escudoY = -0.3f, escudoZ,
inicialCanelaD = 0.0f, inicialCanelaE = 0.0f, inicialCotoveloD = 0.0f, inicialCotoveloE = 0.0f,
inicialCoxaD = 0.0f, inicialCoxaE = 0.0f, inicialOmbroD = 0.0f, inicialOmbroE = 0.0f,
impulsoCotoveloE = 0.0f, impulsoOmbroE = 0.0f, anguloPorta = 0.0f,
posicaoMaoX = 0.0f, posicaoMaoY = 0.0f, posicaoMaoZ = 0.0f,
rotacaoVoandoX = 0.0f, escudoVoandoX = 0.0f, rotacaoPersonagem = 90.0f, velPersonagem = 0.004;

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
    GLfloat light_ambient[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_position[] = { 0.0f, 2.0f, 1.0f, 0.0f };

    GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat high_shininess[] = { 100.0f };

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glClearColor(0.74902, 0.847059, 0.847059, 0.0);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, high_shininess);
}

/* ESTRUTURA DA FILA DE ESCUDOS */
typedef struct escudo {
    float posicaoEscudoX, posicaoEscudoY, posicaoEscudoZ;
    float anguloEscudoX, anguloEscudoY, anguloEscudoZ;
    struct escudo *proximoEscudo;
} noEscudo;
typedef noEscudo *pontEscudo;

typedef struct fila {
    pontEscudo primeiroEscudo;
    pontEscudo ultimoEscudo;
} filaEsc;
typedef filaEsc *filaEscudos;

/* INICIALIZACAO DA FILA */
filaEscudos iniciarFilaEscudos() {
    filaEscudos fila = (filaEscudos) malloc(sizeof(filaEsc));
    fila->primeiroEscudo = NULL;
    fila->ultimoEscudo = NULL;
}

/* INSERCAO DOS ESCUDOS NO FINAL DA FILA */
int inserirEscudo(filaEscudos fila, float posicaoX, float posicaoY, float posicaoZ) {
    pontEscudo novoEscudo;
    if (fila == NULL) return -1;

    novoEscudo = (pontEscudo) malloc(sizeof(noEscudo));
    novoEscudo->posicaoEscudoX = posicaoX;
    novoEscudo->posicaoEscudoY = posicaoY;
    novoEscudo->posicaoEscudoZ = posicaoZ;
    novoEscudo->anguloEscudoX = 0.0f;
    novoEscudo->anguloEscudoY = 0.0f;
    novoEscudo->anguloEscudoZ = 0.0f;

    novoEscudo->proximoEscudo = NULL;

    if (fila->ultimoEscudo != NULL) fila->ultimoEscudo->proximoEscudo = novoEscudo;
    else fila->primeiroEscudo = novoEscudo;
    fila->ultimoEscudo = novoEscudo;

    return 0;
}

/* REMOCAO DO ESCUDO NO INICIO DA FILA */
filaEscudos removerEscudo(filaEscudos fila) {
    pontEscudo escudo;
    if (fila == NULL) return NULL;
    escudo = fila->primeiroEscudo;

    if (fila->primeiroEscudo == fila->ultimoEscudo) fila->ultimoEscudo = NULL;
    fila->primeiroEscudo = fila->primeiroEscudo->proximoEscudo;
    free(escudo);
    return fila;
}
filaEscudos filaEscudosCena;

/* MAQUINA DE ESTADOS PARA O PERSONAGEM */
enum maqPersonagem {
    Pronto,
    IdaX,
    IdaZEsq,
    IdaZDir,
    PegandoEscudo,
    VoltaZEsq,
    VoltaZDir,
    ManobraEsq,
    ManobraDir,
    VoltaX,
    TomandoImpulso,
    JogandoEscudo,
    RecuperandoEscudo,
    VoltandoCasa,
    EmCasa,
    SaindoCasa,
    ForaCasa
} estadoPersonagem;

/* CONTROLE DO PERSONAGEM NA ANIMACAO */
void controlePersonagem(void) {
    switch(estadoPersonagem) {
        case Pronto:
            if (filaEscudosCena->primeiroEscudo != NULL) estadoPersonagem = IdaX;
            break;

        case IdaX:
            flagCaminhada = 1;
            if (posicaoPersonagemX <= (filaEscudosCena->primeiroEscudo->posicaoEscudoX - 0.02))
                posicaoPersonagemX += velPersonagem;
            else {
                if (posicaoPersonagemZ > (filaEscudosCena->primeiroEscudo->posicaoEscudoZ + 0.1))
                    estadoPersonagem = IdaZEsq;
                else if (posicaoPersonagemZ < (filaEscudosCena->primeiroEscudo->posicaoEscudoZ - 0.1))
                    estadoPersonagem = IdaZDir;
                else estadoPersonagem = PegandoEscudo;
            }
            break;

        case IdaZEsq:
            if (rotacaoPersonagem < 180.0) rotacaoPersonagem += (velPersonagem * 500);
            else {
                if (posicaoPersonagemZ > (filaEscudosCena->primeiroEscudo->posicaoEscudoZ + 0.1))
                    posicaoPersonagemZ -= velPersonagem;
                else estadoPersonagem = PegandoEscudo;
            }
            break;

        case IdaZDir:
            if (rotacaoPersonagem > 0.0) rotacaoPersonagem -= (velPersonagem * 500);
            else {
                if (posicaoPersonagemZ < (filaEscudosCena->primeiroEscudo->posicaoEscudoZ - 0.1))
                    posicaoPersonagemZ += velPersonagem;
                else estadoPersonagem = PegandoEscudo;
            }
            break;

        case PegandoEscudo:
            flagCaminhada = 0;
            flagImpulso = 1;

            if (impulsoCotoveloE > -90) impulsoCotoveloE -= 2;
            else if (filaEscudosCena->primeiroEscudo->posicaoEscudoY < -0.15)
                filaEscudosCena->primeiroEscudo->posicaoEscudoY += velPersonagem;
            else if (filaEscudosCena->primeiroEscudo->anguloEscudoX < 90)
                filaEscudosCena->primeiroEscudo->anguloEscudoX += (velPersonagem * 500);
            else {
                filaEscudosCena = removerEscudo(filaEscudosCena);
                if (posicaoPersonagemZ < -0.15) estadoPersonagem = VoltaZEsq;
                else if (posicaoPersonagemZ > -0.15) estadoPersonagem = VoltaZDir;
            }
            break;

        case VoltaZEsq:
            flagCaminhada = 1; flagEscudo = 1;
            if (rotacaoPersonagem > 0.0) rotacaoPersonagem -= (velPersonagem * 500);
            else {
                if (posicaoPersonagemZ < -0.15) posicaoPersonagemZ += velPersonagem;
                else estadoPersonagem = ManobraEsq;
            }
            break;

        case VoltaZDir:
            flagCaminhada = 1; flagEscudo = 1;
            if (rotacaoPersonagem < 180.0) rotacaoPersonagem += (velPersonagem * 500);
            else {
                if (posicaoPersonagemZ > -0.15) posicaoPersonagemZ -= velPersonagem;
                else estadoPersonagem = ManobraDir;
            }
            break;

        case ManobraEsq:
            if (rotacaoPersonagem > -90.0f) rotacaoPersonagem -= (velPersonagem * 500);
            else {
                rotacaoPersonagem = 270.0f;
                estadoPersonagem = VoltaX;
            }
            break;

        case ManobraDir:
            if (rotacaoPersonagem < 270.0f) rotacaoPersonagem += (velPersonagem * 500);
            else estadoPersonagem = VoltaX;
            break;

        case VoltaX:
            if (posicaoPersonagemX > -0.76f) posicaoPersonagemX -= velPersonagem;
            else if (rotacaoPersonagem > 90.0f) rotacaoPersonagem -= (velPersonagem * 500);
            else {
                flagCaminhada = 0;
                estadoPersonagem = TomandoImpulso;
            }
            break;

        case TomandoImpulso:
            if (impulsoCotoveloE < 0.0) impulsoCotoveloE += 2;
            else if (impulsoOmbroE < 45.0) impulsoOmbroE += 2;
            else estadoPersonagem = JogandoEscudo;
            break;

        case JogandoEscudo:
            flagJogar = 1; flagEscudo = 0;
            if (impulsoOmbroE > -90.0) impulsoOmbroE -= 10;
            else if (escudoVoandoX < 2.5f) {
                escudoVoandoX += (3 * velPersonagem);
                rotacaoVoandoX += 10;
            }
            else estadoPersonagem = RecuperandoEscudo;
            break;

        case RecuperandoEscudo:
            if (escudoVoandoX > 0.0f) {
                escudoVoandoX -= (3 * velPersonagem);
                rotacaoVoandoX += 10;
            }
            else {
                flagJogar = 0; flagEscudo = 1;
                if (impulsoOmbroE < 0.0) impulsoOmbroE += 10;
                else if (impulsoCotoveloE > -90.0) impulsoCotoveloE -= 10;
                else estadoPersonagem = VoltandoCasa;
            }
            break;

        case VoltandoCasa:
            if (anguloPorta < 90.0) anguloPorta += 5;
            else {
                flagCaminhada = 1;
                if (rotacaoPersonagem < 270.0) rotacaoPersonagem += (velPersonagem * 500);
                else if (posicaoPersonagemX > -1.6) posicaoPersonagemX -= velPersonagem;
                else {
                    flagCaminhada = 0;
                    estadoPersonagem = EmCasa;
                }
            }
            break;

        case EmCasa:
            flagImpulso = 0; impulsoCotoveloE = 0.0f; impulsoOmbroE = 0.0f; flagEscudo = 0;
            if (anguloPorta > 0.0) anguloPorta -= 5;
            else if (rotacaoPersonagem > 90.0) rotacaoPersonagem -= (velPersonagem * 500);
            else estadoPersonagem = SaindoCasa;
            break;

        case SaindoCasa:
            flagCaminhada = 1;
            if (anguloPorta < 90.0) anguloPorta += 5;
            else if (posicaoPersonagemX < -1.06f) posicaoPersonagemX += velPersonagem;
            else estadoPersonagem = ForaCasa;
            break;

        case ForaCasa:
            flagCaminhada = 0;
            if (anguloPorta > 0.0) anguloPorta -= 5;
            else estadoPersonagem = Pronto;
            break;
    }
}

const char* obterEstado(enum maqPersonagem estado) {
    switch (estado) {
        case Pronto:
            return "Pronto";
        case IdaX:
            return "IdaX";
        case IdaZEsq:
            return "IdaZEsq";
        case IdaZDir:
            return "IdaZDir";
        case PegandoEscudo:
            return "PegandoEscudo";
        case VoltaZEsq:
            return "VoltaZEsq";
        case VoltaZDir:
            return "VoltaZDir";
        case ManobraEsq:
            return "ManobraEsq";
        case ManobraDir:
            return "ManobraDir";
        case VoltaX:
            return "VoltaX";
        case TomandoImpulso:
            return "TomandoImpulso";
        case JogandoEscudo:
            return "JogandoEscudo";
        case RecuperandoEscudo:
            return "RecuperandoEscudo";
        case VoltandoCasa:
            return "VoltandoCasa";
        case EmCasa:
            return "EmCasa";
        case SaindoCasa:
            return "SaindoCasa";
        case ForaCasa:
            return "ForaCasa";
    }
}

/* DESENHO DO ESCUDO */
void desenharEscudo(float posX, float posY, float posZ, float angX, float angY, float angZ) {
    float angulo; int i;

    glPushMatrix();
        glColor3f(0.22, 0.22, 0.22);
        glTranslatef(posX, posY, posZ);
        glRotatef(angX, 1.0, 0.0, 0.0);
        glRotatef(angY, 0.0, 1.0, 0.0);
        glRotatef(angZ, 0.0, 0.0, 1.0);

        // arco externo
        glBegin(GL_QUAD_STRIP);
            for (i = 0; i <= 20; i++) {
                angulo = (float) (2 * M_PI * i / 20.0f);
                glVertex3f(0.07f * cosf(angulo), 0.0f, 0.07f * sinf(angulo));
                glVertex3f(0.07f * cosf(angulo), 0.01f, 0.07f * sinf(angulo));
            }
        glEnd();

        // face de cima
        glBegin(GL_POLYGON);
            for (i = 0; i < 20; i++) {
                angulo = (float) (2 * M_PI * i / 20.0f);
                glVertex3f(0.07f * cosf(angulo), 0.0f, 0.07f * sinf(angulo));
            }
        glEnd();

        // face de baixo
        glBegin(GL_POLYGON);
            for (i = 0; i < 20; i++) {
                angulo = (float) (2 * M_PI * i / 20.0);
                glVertex3f(0.07f * cosf(angulo), 0.01f, 0.07f * sinf(angulo));
            }
        glEnd();

        // estrela no escudo
        for (int j = 0; j < 50; j++) {
            glPushMatrix();
            glTranslatef(0.0f, 0.02f, -0.01f);
            glRotatef(90.0, 1.0, 0.0, 0.0);
            glScalef(0.2f - (j / 100), 0.2f - (j / 100), 0.2f - (j / 100));
            glColor3f(1.0f, 1.0f, 1.0f);
            glEnable(GL_LINE_SMOOTH);
            glBegin(GL_LINE_LOOP);
                glVertex3f(0.0, 0.25, 0.0);
                glVertex3f(0.08, 0.1, 0.0);
                glVertex3f(0.2, 0.1, 0.0);
                glVertex3f(0.1, 0.0, 0.0);
                glVertex3f(0.15, -0.15f, 0.0);
                glVertex3f(0.0, -0.05f, 0.0);
                glVertex3f(-0.15f, -0.15f, 0.0);
                glVertex3f(-0.1f, 0.0, 0.0);
                glVertex3f(-0.2f, 0.1, 0.0);
                glVertex3f(-0.08f, 0.1, 0.0);
            glEnd();
            glDisable(GL_LINE_SMOOTH);
            glPopMatrix();
        }
    glPopMatrix();
}

/* DESENHO DO CAPITAO AMERICA */
void cabeca() {
    int i; float angulo;

    // cabeca
    glPushMatrix();
        glColor3f(0.92f, 0.79f, 0.49f);
        glScalef(1.0f, 1.2f, 1.0f);
        glutSolidSphere(0.03, 50, 50);
    glPopMatrix();

    // cabelo
    glPushMatrix();
        glTranslatef(0.0f, 0.005f, -0.01f);
        glColor3f(0.61, 0.41, 0.0);
        glScalef(1.0f, 1.2f, 1.0f);
        glutSolidSphere(0.03, 50, 50);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0f, 0.035f, 0.0f);
        glRotatef(250.0, 1.0, 0.0, 0.0);
        glColor3f(0.61, 0.41, 0.0);
        glutSolidCone(0.02f, 0.006f, 50, 50);
    glPopMatrix();

    // olho esquerdo
    glPushMatrix();
        glColor3f(0.01, 0.61, 0.78);
        glTranslatef(-0.01f, 0.01f, 0.025f);
        glRotatef(90.0, 1.0, 0.0, 0.0);

        glBegin(GL_QUAD_STRIP);
        for (i = 0; i <= 20; i++) {
            angulo = (float) (2 * M_PI * i / 20.0f);
            glVertex3f(0.003f * cosf(angulo), 0.0f, 0.003f * sinf(angulo));
            glVertex3f(0.003f * cosf(angulo), 0.005f, 0.003f * sinf(angulo));
        }
        glEnd();

        glBegin(GL_POLYGON);
        for (i = 0; i < 20; i++) {
            angulo = (float) (2 * M_PI * i / 20.0f);
            glVertex3f(0.003f * cosf(angulo), 0.0f, 0.003f * sinf(angulo));
        }
        glEnd();

        glBegin(GL_POLYGON);
        for (i = 0; i < 20; i++) {
            angulo = (float) (2 * M_PI * i / 20.0);
            glVertex3f(0.003f * cosf(angulo), 0.005f, 0.003f * sinf(angulo));
        }
        glEnd();
    glPopMatrix();

    // olho direito
    glPushMatrix();
        glColor3f(0.01, 0.61, 0.78);
        glTranslatef(0.01f, 0.01f, 0.025f);
        glRotatef(90.0, 1.0, 0.0, 0.0);

        glBegin(GL_QUAD_STRIP);
        for (i = 0; i <= 20; i++) {
            angulo = (float) (2 * M_PI * i / 20.0f);
            glVertex3f(0.003f * cosf(angulo), 0.0f, 0.003f * sinf(angulo));
            glVertex3f(0.003f * cosf(angulo), 0.005f, 0.003f * sinf(angulo));
        }
        glEnd();

        glBegin(GL_POLYGON);
        for (i = 0; i < 20; i++) {
            angulo = (float) (2 * M_PI * i / 20.0f);
            glVertex3f(0.003f * cosf(angulo), 0.0f, 0.003f * sinf(angulo));
        }
        glEnd();

        glBegin(GL_POLYGON);
        for (i = 0; i < 20; i++) {
            angulo = (float) (2 * M_PI * i / 20.0);
            glVertex3f(0.003f * cosf(angulo), 0.005f, 0.003f * sinf(angulo));
        }
        glEnd();
    glPopMatrix();

    // nariz
    glPushMatrix();
        glTranslatef(0.001f, 0.0f, 0.03f);
        glRotatef(30.0, 0.0, 0.0, 1.0);
        glColor3f(0.0, 0.0, 0.0);
        glScalef(0.2, 0.8, 0.2);
        glutSolidCube(0.008);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0f, -0.003f, 0.03f);
        glRotatef(90.0, 0.0, 0.0, 1.0);
        glColor3f(0.0, 0.0, 0.0);
        glScalef(0.2, 0.8, 0.2);
        glutSolidCube(0.008);
    glPopMatrix();

    // boca
    glPushMatrix();
        glTranslatef(-0.009f, -0.012f, 0.027f);
        glRotatef(30.0, 0.0, 0.0, 1.0);
        glColor3f(0.0, 0.0, 0.0);
        glScalef(0.2, 0.8, 0.2);
        glutSolidCube(0.008);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0f, -0.014f, 0.027f);
        glRotatef(90.0, 0.0, 0.0, 1.0);
        glColor3f(0.0, 0.0, 0.0);
        glScalef(0.2, 2.0, 0.2);
        glutSolidCube(0.008);
    glPopMatrix();

    // pescoco
    glPushMatrix();
        glColor3f(0.0f, 0.16f, 0.27f);
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

    glPushMatrix();
        glColor3f(0.0f, 0.24f, 0.41f);
        glTranslatef(0.0f, -0.07f, 0.0f);
        glScalef(1.5f, 0.8f, 1.0f);

        // peitoral
        glPushMatrix();
            glutSolidSphere(0.03, 50, 50);

            // estrela no peito
            glPushMatrix();
                glTranslatef(0.0f, 0.0f, 0.032f);
                glScalef(0.05, 0.05, 0.05);
                glColor3f(1.0f, 1.0f, 1.0f);
                glEnable(GL_LINE_SMOOTH);
                glBegin(GL_LINE_LOOP);
                    glVertex3f(0.0, 0.25, 0.0);
                    glVertex3f(0.08, 0.1, 0.0);
                    glVertex3f(0.2, 0.1, 0.0);
                    glVertex3f(0.1, 0.0, 0.0);
                    glVertex3f(0.15, -0.15 ,0.0);
                    glVertex3f(0.0, -0.05, 0.0);
                    glVertex3f(-0.15, -0.15 ,0.0);
                    glVertex3f(-0.1, 0.0, 0.0);
                    glVertex3f(-0.2, 0.1, 0.0);
                    glVertex3f(-0.08, 0.1, 0.0);
                glEnd();
                glDisable(GL_LINE_SMOOTH);
            glPopMatrix();
        glPopMatrix();

        glColor3f(0.0f, 0.24f, 0.41f);

        // asas de maromba
        glPushMatrix();
            glRotatef(90.0, 1.0, 0.0, 0.0);
            glutSolidCone(0.03f, 0.09f, 50, 50);
        glPopMatrix();

        // barriga
        glPushMatrix();
            glColor3f(0.84, 0.85, 0.85);
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
                    if (i < 10) glColor3f(0.72f, 0.0f, 0.0f);
                    glVertex3f(0.015f * cosf(angulo), 0.0f, 0.015f * sinf(angulo));
                }
            glEnd();

            glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0);
                    if (i < 10) glColor3f(0.72f, 0.0f, 0.0f);
                    glVertex3f(0.015f * cosf(angulo), 0.1f, 0.015f * sinf(angulo));
                }
            glEnd();

            // faixas vermelhas
            glPushMatrix();
                glTranslatef(0.008f, 0.0f, 0.011f);
                glBegin(GL_QUAD_STRIP);
                for (i = 0; i <= 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.005f * cosf(angulo), 0.0f, 0.005f * sinf(angulo));
                    glVertex3f(0.005f * cosf(angulo), 0.06f, 0.005f * sinf(angulo));
                }
                glEnd();

                glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.005f * cosf(angulo), 0.0f, 0.005f * sinf(angulo));
                }
                glEnd();

                glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0);
                    glVertex3f(0.005f * cosf(angulo), 0.06f, 0.005f * sinf(angulo));
                }
                glEnd();
            glPopMatrix();

            glPushMatrix();
                glTranslatef(-0.008f, 0.0f, 0.011f);
                glBegin(GL_QUAD_STRIP);
                for (i = 0; i <= 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.005f * cosf(angulo), 0.0f, 0.005f * sinf(angulo));
                    glVertex3f(0.005f * cosf(angulo), 0.06f, 0.005f * sinf(angulo));
                }
                glEnd();

                glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.005f * cosf(angulo), 0.0f, 0.005f * sinf(angulo));
                }
                glEnd();

                glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0);
                    glVertex3f(0.005f * cosf(angulo), 0.06f, 0.005f * sinf(angulo));
                }
                glEnd();
            glPopMatrix();

            glPushMatrix();
                glTranslatef(0.008f, 0.0f, -0.011f);
                glBegin(GL_QUAD_STRIP);
                for (i = 0; i <= 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.005f * cosf(angulo), 0.0f, 0.005f * sinf(angulo));
                    glVertex3f(0.005f * cosf(angulo), 0.06f, 0.005f * sinf(angulo));
                }
                glEnd();

                glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.005f * cosf(angulo), 0.0f, 0.005f * sinf(angulo));
                }
                glEnd();

                glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0);
                    glVertex3f(0.005f * cosf(angulo), 0.06f, 0.005f * sinf(angulo));
                }
                glEnd();
            glPopMatrix();

            glPushMatrix();
                glTranslatef(-0.008f, 0.0f, -0.011f);
                glBegin(GL_QUAD_STRIP);
                for (i = 0; i <= 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.005f * cosf(angulo), 0.0f, 0.005f * sinf(angulo));
                    glVertex3f(0.005f * cosf(angulo), 0.06f, 0.005f * sinf(angulo));
                }
                glEnd();

                glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0f);
                    glVertex3f(0.005f * cosf(angulo), 0.0f, 0.005f * sinf(angulo));
                }
                glEnd();

                glBegin(GL_POLYGON);
                for (i = 0; i < 20; i++) {
                    angulo = (float) (2 * M_PI * i / 20.0);
                    glVertex3f(0.005f * cosf(angulo), 0.06f, 0.005f * sinf(angulo));
                }
                glEnd();
            glPopMatrix();

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
            if (flagCaminhada) glRotatef(anguloOmbroD, 1.0, 0.0, 0.0);
            else glRotatef(inicialOmbroD, 1.0, 0.0, 0.0);
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
                if (flagCaminhada) glRotatef(-anguloCotoveloD, 1.0, 0.0, 0.0);
                else glRotatef(inicialCotoveloD, 1.0, 0.0, 0.0);
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
            if (flagImpulso) glRotatef(impulsoOmbroE, 1.0, 0.0, 0.0);
            else if (flagCaminhada) glRotatef(anguloOmbroE, 1.0, 0.0, 0.0);
            else glRotatef(inicialOmbroE, 1.0, 0.0, 0.0);
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
                if (flagImpulso) glRotatef(impulsoCotoveloE, 1.0, 0.0, 0.0);
                else if (flagCaminhada) glRotatef(-anguloCotoveloE, 1.0, 0.0, 0.0);
                else glRotatef(-inicialCotoveloE, 1.0, 0.0, 0.0);
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
                    if (flagEscudo) desenharEscudo(posicaoMaoX, posicaoMaoY, posicaoMaoZ, 0.0, 0.0, 90.0);
                    else if (flagJogar) desenharEscudo(0.0, -escudoVoandoX, 0.0, rotacaoVoandoX, 0.0, 90.0);
                    posicaoMaoX = 0.0f; posicaoMaoY = -0.018f; posicaoMaoZ = 0.0f;
                    glColor3f(0.72f, 0.0f, 0.0f);
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
            if (flagCaminhada) glRotatef(-anguloCoxaD, 1.0, 0.0, 0.0);
            else glRotatef(-inicialCoxaD, 1.0, 0.0, 0.0);
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
                if (flagCaminhada) glRotatef(anguloCanelaD, 1.0, 0.0, 0.0);
                else glRotatef(inicialCanelaD, 1.0, 0.0, 0.0);
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
            if (flagCaminhada) glRotatef(anguloCoxaE, 1.0, 0.0, 0.0);
            else glRotatef(inicialCoxaE, 1.0, 0.0, 0.0);
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
                if (flagCaminhada) glRotatef(anguloCanelaE, 1.0, 0.0, 0.0);
                else glRotatef(inicialCanelaE, 1.0, 0.0, 0.0);
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
        glRotatef(rotacaoPersonagem, 0.0, 1.0, 0.0);

        cabeca();
        tronco();
        bracoDireito();
        bracoEsquerdo();
        pernaDireita();
        pernaEsquerda();
    glPopMatrix();
}

/* DESENHO DO CENARIO */
void chaoCabana() {
    // chao da cabana
    glPushMatrix();
        glColor3f(0.26f, 0.0f, 0.0f);
        glTranslatef(-1.50f, -0.28f, -0.2f);
        glScalef(10.0f, 0.5f, 20.0f);
        glutSolidCube(0.05);
    glPopMatrix();

    // chao da varanda
    glPushMatrix();
        glColor3f(0.21, 0.2, 0.2);
        glTranslatef(-1.12f, -0.28f, -0.18f);
        glScalef(5.0f, 0.5f, 20.0f);
        glutSolidCube(0.05);
    glPopMatrix();
}

void paredesCabana() {
    // parede do fundo
    glPushMatrix();
        glColor3f(0.26f, 0.0f, 0.0f);
        glTranslatef(-1.75f, -0.05f, -0.2f);
        glScalef(0.5f, 10.0f, 20.0f);
        glutSolidCube(0.05);
    glPopMatrix();

    // parede da esquerda
    glPushMatrix();
        glColor3f(0.26f, 0.0f, 0.0f);
        glTranslatef(-1.5f, -0.05f, 0.3f);
        glScalef(10.0f, 10.0f, 0.5f);
        glutSolidCube(0.05);
    glPopMatrix();

    // parede da direita
    glPushMatrix();
        glColor3f(0.26f, 0.0f, 0.0f);
        glTranslatef(-1.39f, -0.05f, -0.7f);
        glScalef(15.0f, 10.0f, 0.5f);
        glutSolidCube(0.05);
    glPopMatrix();

    // parede da frente, lado esquerdo
    glPushMatrix();
        glColor3f(0.26f, 0.0f, 0.0f);
        glTranslatef(-1.25f, -0.05f, 0.14f);
        glScalef(0.5f, 10.0f, 7.0f);
        glutSolidCube(0.05);
    glPopMatrix();

    // base da parede da frente, lado direito
    glPushMatrix();
        glColor3f(0.26f, 0.0f, 0.0f);
        glTranslatef(-1.25f, -0.05f, -0.48f);
        glScalef(0.5f, 10.0f, 9.0f);
        glutSolidCube(0.05);
    glPopMatrix();
}

void janelaCabana() {
    // parede da janela
    glPushMatrix();
        glColor3f(0.26f, 0.0f, 0.0f);
        glTranslatef(-1.0f, -0.05f, -0.52f);

        // esquerda
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 0.16f);
            glScalef(0.5f, 10.0f, 1.2f);
            glutSolidCube(0.05);
        glPopMatrix();

        // direita
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, -0.16f);
            glScalef(0.5f, 10.0f, 1.2f);
            glutSolidCube(0.05);
        glPopMatrix();

        // abaixo
        glPushMatrix();
            glTranslatef(0.0f, -0.16f, 0.0f);
            glScalef(0.5f, 3.5f, 7.5f);
            glutSolidCube(0.05);
        glPopMatrix();

        // acima
        glPushMatrix();
            glTranslatef(0.0f, 0.19f, 0.0f);
            glScalef(0.5f, 2.5f, 7.5f);
            glutSolidCube(0.05);
        glPopMatrix();

        // vidro da janela
        glPushMatrix();
            glColor3f(0.02, 0.29, 0.49);
            glTranslatef(0.0f, 0.03f, 0.0f);
            glScalef(0.5f, 4.0f, 5.0f);
            glutSolidCube(0.05);
        glPopMatrix();

        // parapeito da janela
        glPushMatrix();
            glColor3f(0.62, 0.38, 0.03);
            glTranslatef(0.02f, -0.06f, 0.0f);
            glScalef(0.5f, 0.8f, 5.0f);
            glutSolidCube(0.05);
        glPopMatrix();
    glPopMatrix();

    // parede entre janela e porta
    glPushMatrix();
        glColor3f(0.26f, 0.0f, 0.0f);
        glTranslatef(-1.13f, -0.05f, -0.34f);
        glScalef(4.5f, 10.0f, 0.5f);
        glutSolidCube(0.05);
    glPopMatrix();
}

void portaCabana() {
    // parede acima da porta
    glPushMatrix();
        glTranslatef(-1.25f, 0.14f, -0.2f);
        glScalef(0.5f, 2.5f, 7.5f);
        glutSolidCube(0.05);

        // porta
        glPushMatrix();
            glColor3f(0.82, 0.5, 0.05);
            glTranslatef(0.0f, -0.09f, 0.01f);
            glTranslatef(0.0f, 0.0f, 0.0175f);
            glRotatef(anguloPorta, 0.0, 1.0, 0.0);
            glTranslatef(0.0f, 0.0f, -0.0175f);
            glScalef(0.05f, 2.9f, 0.7f);
            glutSolidCube(0.05);

            // macaneta
            glPushMatrix();
                glColor3f(0.31, 0.22, 0.11);
                glTranslatef(0.04f, 0.0f, -0.015f);
                glutSolidSphere(0.002, 50, 50);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

void varandaCabana() {
    // parte frontal
        // corrimao
        glPushMatrix();
            glColor3f(0.49, 0.43, 0.35);
            glTranslatef(-1.01f, -0.12f, 0.14f);
            glScalef(0.5f, 0.5f, 7.0f);
            glutSolidCube(0.05);
        glPopMatrix();

        // base
        glPushMatrix();
            glColor3f(0.49, 0.43, 0.35);
            glTranslatef(-1.01f, -0.25f, 0.14f);
            glScalef(0.5f, 0.5f, 7.0f);
            glutSolidCube(0.05);
        glPopMatrix();

        // colunas
        for (int i = 0; i < 7; i++) {
            glPushMatrix();
                glColor3f(0.49, 0.43, 0.35);
                glTranslatef(-1.01f, -0.19f, -0.02f + (i / 20.0f));
                glScalef(1.0f, 15.0f, 1.0f);
                glutSolidCube(0.01);
            glPopMatrix();
        }

    // parte lateral
        // corrimao
        glPushMatrix();
            glColor3f(0.49, 0.43, 0.35);
            glTranslatef(-1.12f, -0.12f, 0.3f);
            glScalef(4.5f, 0.5f, 0.5f);
            glutSolidCube(0.05);
        glPopMatrix();

        // base
        glPushMatrix();
            glColor3f(0.49, 0.43, 0.35);
            glTranslatef(-1.12f, -0.25f, 0.3f);
            glScalef(4.5f, 0.5f, 0.5f);
            glutSolidCube(0.05);
        glPopMatrix();

        // colunas
        for (int i = 0; i < 5; i++) {
            glPushMatrix();
                glColor3f(0.49, 0.43, 0.35);
                glTranslatef(-1.25f + (i / 20.0f), -0.19f, 0.3f);
                glScalef(1.0f, 15.0f, 1.0f);
                glutSolidCube(0.01);
            glPopMatrix();
        }
}

void telhadoCabana() {
    glPushMatrix();
        // lateral esquerda
        glColor3f (.5, 0.5, .25);
        glBegin(GL_POLYGON);
            glVertex3f (-1.0f, 0.3f, 0.2f);
            glVertex3f (-1.6f, 0.3f, 0.2f);
            glVertex3f (-1.8f, 0.18f, 0.4f);
            glVertex3f (-0.8f, 0.18f, 0.4f);
        glEnd();

        // lateral direita
        glBegin(GL_POLYGON);
            glVertex3f (-1.0f, 0.3f, -0.6f);
            glVertex3f (-1.6f, 0.3f, -0.6f);
            glVertex3f (-1.8f, 0.18f, -0.8f);
            glVertex3f (-0.8f, 0.18f, -0.8f);
        glEnd();

        // fundo
        glBegin(GL_POLYGON);
            glVertex3f (-1.6f, 0.3f, 0.2f);
            glVertex3f (-1.6f, 0.3f, -0.6f);
            glVertex3f (-1.8f, 0.18f, -0.8f);
            glVertex3f (-1.8f, 0.18f, 0.4f);
        glEnd();

        // frente
        glBegin(GL_POLYGON);
            glVertex3f (-1.0f, 0.3f, -0.6f);
            glVertex3f (-1.0f, 0.3f, 0.2f);
            glVertex3f (-0.8f, 0.18f, 0.4f);
            glVertex3f (-0.8f, 0.18f, -0.8f);
        glEnd();

        // topo
        glBegin(GL_POLYGON);
            glVertex3f (-1.6f, 0.3f, -0.6f);
            glVertex3f (-1.6f, 0.3f, 0.2f);
            glVertex3f (-1.0f, 0.3f, 0.2f);
            glVertex3f (-1.0f, 0.3f, -0.6f);
        glEnd();
    glPopMatrix();
}

void cabanaCompleta() {
    glPushMatrix();
        chaoCabana();
        paredesCabana();
        janelaCabana();
        portaCabana();
        varandaCabana();
        telhadoCabana();
    glPopMatrix();
}

void folhaArvore() {
    glPushMatrix();
        glColor3f(0.04f, 0.31f, 0.0f);
        glTranslatef(0.0f, 0.15f, 0.25f);
        glRotatef(-90.0f, 1.0, 0.0, 0.0);
        glTranslatef(0.0f, 0.25f, 0.0f);
        glutSolidCone(0.15, 0.3, 50, 50);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0.04f, 0.31f, 0.0f);
        glTranslatef(0.0f, 0.30f, 0.25f);
        glRotatef(-90.0f, 1.0, 0.0, 0.0);
        glTranslatef(0.0f, 0.25f, 0.0f);
        glutSolidCone(0.12, 0.25, 50, 50);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0.04f, 0.31f, 0.0f);
        glTranslatef(0.0f, 0.45f, 0.25f);
        glRotatef(-90.0f, 1.0, 0.0, 0.0);
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
    pontEscudo auxEscudo;

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
    // for (float i = 0; i <= 4.5; i = i + 0.25f) troncoArvore((posicaoX + i), posicaoY, (posicaoZ + 2.0f));

    // arvores da esquerda
    for (float i = 0; i <= 2.0; i = i + 0.25f) troncoArvore(posicaoX, posicaoY, (posicaoZ + i));

    // arvores da direita
    for (float i = 0; i <= 2.0; i = i + 0.25f) troncoArvore((posicaoX + 4.5f), posicaoY, (posicaoZ + i));

    // desenhar escudos, se houver
    if (filaEscudosCena->primeiroEscudo != NULL)
        for (auxEscudo = filaEscudosCena->primeiroEscudo; auxEscudo != NULL; auxEscudo = auxEscudo->proximoEscudo)
            desenharEscudo(auxEscudo->posicaoEscudoX, (auxEscudo->posicaoEscudoY + 0.03f), auxEscudo->posicaoEscudoZ,
                    auxEscudo->anguloEscudoX, auxEscudo->anguloEscudoY, auxEscudo->anguloEscudoZ);

    // desenhar cabana
    cabanaCompleta();
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

    controlePersonagem();
    // printf("%s\n", obterEstado(estadoPersonagem));

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
    if (flagCaminhada) {
        // ombro esquerdo
        if (anguloOmbroE >= 45 || anguloOmbroE <= -45) auxOmbroE = -auxOmbroE;
        anguloOmbroE -= auxOmbroE;

        // cotovelo esquerdo
        if (anguloCotoveloE >= 90 || anguloCotoveloE < 0) auxCotoveloE = -auxCotoveloE;
        anguloCotoveloE += auxCotoveloE;

        // ombro direito
        if (anguloOmbroD >= 45 || anguloOmbroD <= -45) auxOmbroD = -auxOmbroD;
        anguloOmbroD += auxOmbroD;

        // cotovelo direito
        if (anguloCotoveloD >= 90 || anguloCotoveloD < 0) auxCotoveloD = -auxCotoveloD;
        anguloCotoveloD += auxCotoveloD;

        // coxa esquerda
        if (anguloCoxaE >= 45 || anguloCoxaE <= -45) auxCoxaE = -auxCoxaE;
        anguloCoxaE += auxCoxaE;

        // canela esquerda
        if (anguloCanelaE < 0 || anguloCanelaE >= 50) auxCanelaE = -auxCanelaE;
        anguloCanelaE -= auxCanelaE;

        // coxa direita
        if (anguloCoxaD >= 45 || anguloCoxaD <= -45) auxCoxaD = -auxCoxaD;
        anguloCoxaD += auxCoxaD;

        // canela direita
        if (anguloCanelaD < 0 || anguloCanelaD >= 50) auxCanelaD = -auxCanelaD;
        anguloCanelaD -= auxCanelaD;
    }

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
        escudoX = ((rand() * 2.2f)/(RAND_MAX)) - 0.6f;
        escudoZ = ((rand() * 1.3f)/(RAND_MAX)) - 0.9f;

        // criar escudo (adicionar na fila)
        inserirEscudo(filaEscudosCena, escudoX, escudoY, escudoZ);
        flagCaminhada = 1;
    }

    ProjecaoCena();
    glutPostRedisplay();
}

/* FUNCAO PRINCIPAL */
int main(int argc, char *argv[]) {
    srand(time(NULL));
    filaEscudosCena = iniciarFilaEscudos();

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