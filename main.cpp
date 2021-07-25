#include "ProcesadorImagenes.h"

#define WIDTH  600
#define HEIGHT 600

ProcesadorImagenes procesadorImagenes;

GLfloat posicionMouseX;
GLfloat posicionMouseY;

// Matriz de multiplicacion
GLfloat matrizMultiplicacion[16] = {1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    0, 0, 0, 1};

void mostrarImagenes() {
    for (auto &it : procesadorImagenes.imagenes) {
        // cout << "Negro : '" << to_string(it.atXYZC(242,34, 1, 1)) << "'" << endl;
        // cout << "Blanco: '" << to_string(it.atXYZC(243,34, 1, 1)) << "'" << endl;
        // cout << "Punto : '" << to_string(it.atXYZC(244,34, 1, 1)) << "'" << endl;
        it.display();
    }
}

void iniciar() {
    // Calcular los puntos desde las imagenes
    /*procesadorImagenes.leerImagenes("../imagenes.txt", 100);
    // mostrarImagenes();
    procesadorImagenes.extraerPuntos(7.0);
    // procesadorImagenes.exportarPuntos("../puntos.txt");*/

    // Carga puntos desde archivo
    procesadorImagenes.cargarArchivo("../puntos.txt");
}

void funcionReshape(int w, int h) {
    float rango = (float)w/2;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho((float)-w/2, (float)w/2, (float)-h/2, (float)h/2, -rango, rango);
    glMatrixMode(GL_MODELVIEW);
}

void funcionDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_POINTS);
    for (auto &imagen : procesadorImagenes.puntos) {
        for (auto &punto : imagen) {
            glColor3f(1.0, 0.0, 0.0);
            glVertex3f(punto.coordenadaX, punto.coordenadaY, punto.coordenadaZ);
        }
    }
    glEnd();

    glFlush();
    glutSwapBuffers();
}

void funcionMotion(int x, int y) {
    GLint W = glutGet(GLUT_WINDOW_WIDTH);
    GLint H = glutGet(GLUT_WINDOW_HEIGHT);

    GLfloat rayo = (W > H) ? ((GLfloat)W * (GLfloat)W / 2) : ((GLfloat)H * (GLfloat)H / 2);

    // Se usa el centro de la ventana como origen
    posicionMouseX -= (GLfloat)W / 2;
    GLfloat posicionX = (GLfloat)x - (GLfloat)W / 2;
    posicionMouseY = (GLfloat)H / 2 - posicionMouseY;
    GLfloat posicionY = (GLfloat)H / 2 - (GLfloat)y;

    // Encontrar la coordenada en Z de cada punto dentro de la ventana
    GLfloat cuadradoMouse = posicionMouseX * posicionMouseX + posicionMouseY * posicionMouseY;
    GLfloat cuadradoPosicion = posicionX * posicionX + posicionY * posicionY;

    if (cuadradoMouse < rayo && cuadradoPosicion < rayo) {
        GLfloat posicionMouseZ = sqrt(rayo - cuadradoMouse);
        GLfloat posicionZ = sqrt(rayo - cuadradoPosicion);

        GLfloat ejeRotacion[3];
        ejeRotacion[0] = -posicionMouseZ * posicionY + posicionMouseY * posicionZ;
        ejeRotacion[1] = posicionMouseZ * posicionX - posicionMouseX * posicionZ;
        ejeRotacion[2] = -posicionMouseY * posicionX + posicionMouseX * posicionY;

        GLfloat anguloRotacion = atan(sqrt(ejeRotacion[0] * ejeRotacion[0] + ejeRotacion[1] * ejeRotacion[1] + ejeRotacion[2] * ejeRotacion[2]));

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(anguloRotacion, ejeRotacion[0], ejeRotacion[1], ejeRotacion[2]);
        glMultMatrixf(matrizMultiplicacion);
        glGetFloatv(GL_MODELVIEW_MATRIX, matrizMultiplicacion);
    }
    posicionMouseX = (GLfloat)x;
    posicionMouseY = (GLfloat)y;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    iniciar();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    int POS_X = (glutGet(GLUT_SCREEN_WIDTH) - WIDTH) >> 1;
    int POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) >> 1;
    glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Reconstruccion 3D");

    glutReshapeFunc(funcionReshape);
    glutDisplayFunc(funcionDisplay);
    glutMotionFunc(funcionMotion);
    glEnable(GL_DEPTH_TEST);
    glPointSize(1.0);

    glutMainLoop();

    return 0;
}
