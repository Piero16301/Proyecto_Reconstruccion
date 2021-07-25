#include "Interfaz.h"

#define WIDTH  600
#define HEIGHT 600

int main(int argc, char** argv) {
    Interfaz::iniciar();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    int POS_X = (glutGet(GLUT_SCREEN_WIDTH) - WIDTH) >> 1;
    int POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) >> 1;
    glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Reconstruccion 3D");

    glutReshapeFunc(Interfaz::funcionReshape);
    glutDisplayFunc(Interfaz::funcionDisplay);
    glutMotionFunc(Interfaz::funcionMotion);
    glEnable(GL_DEPTH_TEST);
    glPointSize(1.0);

    glutMainLoop();

    return 0;
}
