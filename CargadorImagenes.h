#ifndef PROYECTO_RECONSTRUCCION_CARGADORIMAGENES_H
#define PROYECTO_RECONSTRUCCION_CARGADORIMAGENES_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "CImg.h"
#include "Punto3D.h"

using namespace cimg_library;
using namespace std;

class CargadorImagenes {
public:
    vector <CImg <char>> imagenes;
    vector <vector <Punto3D>> puntos;
    int cantidadImagenes = 0;
    int alto = 0;
    int ancho = 0;

public:
    CImg <char> binarizar(CImg<float> &imagen, int umbral) const;
    void leerImagenes(const string& rutasImagenes, int umbral);
    void extraerPuntos();
};

#endif //PROYECTO_RECONSTRUCCION_CARGADORIMAGENES_H
