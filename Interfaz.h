#ifndef PROYECTO_RECONSTRUCCION_INTERFAZ_H
#define PROYECTO_RECONSTRUCCION_INTERFAZ_H

#include "Procesador.h"

static Procesador procesador;

static GLfloat posicionMouseX;
static GLfloat posicionMouseY;

// Matriz de multiplicacion
static GLfloat matrizMultiplicacion[16] = {1, 0, 0, 0,
                                           0, 1, 0, 0,
                                           0, 0, 1, 0,
                                           0, 0, 0, 1};

class Interfaz {
public:
    static void iniciar();

    static void funcionReshape(int w, int h);

    static void funcionDisplay();

    static void funcionMotion(int x, int y);

    static void mostrarImagenes();
};

#endif //PROYECTO_RECONSTRUCCION_INTERFAZ_H
