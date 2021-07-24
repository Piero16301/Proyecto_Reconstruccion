#ifndef PROYECTO_RECONSTRUCCION_PROCESADORIMAGENES_H
#define PROYECTO_RECONSTRUCCION_PROCESADORIMAGENES_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

#include "CImg.h"
#include "Punto3D.h"

using namespace cimg_library;
using namespace std;

class ProcesadorImagenes {
public:
    vector <CImg <char>> imagenes;
    vector <vector <Punto3D>> puntos;
    int cantidadImagenes = 0;

public:
    CImg <char> binarizar(CImg<float> &imagen, int umbral) const;
    void leerImagenes(const string& rutasImagenes, int umbral);
    void extraerPuntos(float distanciaImagenes);
    void exportarPuntos(const string& ruta);
};

#endif //PROYECTO_RECONSTRUCCION_PROCESADORIMAGENES_H
