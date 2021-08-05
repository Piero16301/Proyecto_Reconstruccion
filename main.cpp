#include "Interfaz.h"

#define WIDTH  600
#define HEIGHT 600

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    int POS_X = (glutGet(GLUT_SCREEN_WIDTH) - WIDTH) >> 1;
    int POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) >> 1;
    glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Reconstruccion 3D");

    //omp_set_num_threads(4);
    Interfaz::iniciar();

    int submenuPuntos = glutCreateMenu(Interfaz::menuInterfaz);
    glutAddMenuEntry("Puntos de bordes", 1);
    glutAddMenuEntry("Puntos detallados", 2);

    int submenuColores = glutCreateMenu(Interfaz::menuInterfaz);
    glutAddMenuEntry("Puntos rojos", 3);
    glutAddMenuEntry("Puntos verdes", 4);
    glutAddMenuEntry("Puntos azules", 5);

    int menuPrincipal = glutCreateMenu(Interfaz::menuInterfaz);
    glutAddSubMenu("Tipo de puntos", submenuPuntos);
    glutAddSubMenu("Color de puntos", submenuColores);

    glutAddMenuEntry("Salir", 6);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutReshapeFunc(Interfaz::funcionReshape);
    glutDisplayFunc(Interfaz::funcionDisplay);
    glutKeyboardFunc(Interfaz::funcionTeclado);
    glutMotionFunc(Interfaz::funcionMotion);
    glEnable(GL_DEPTH_TEST);
    glPointSize(1.0);

    glutMainLoop();

    return 0;
}

/*
1 thread
Detallados: 5.11221
Bordes:     2.86888

2 threads
Detallados: 2.68332
Bordes:     1.47612

4 threads
Detallados: 2.51741
Bordes:     1.33912
*/
