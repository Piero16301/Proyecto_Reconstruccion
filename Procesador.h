#ifndef PROYECTO_RECONSTRUCCION_PROCESADOR_H
#define PROYECTO_RECONSTRUCCION_PROCESADOR_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <omp.h>

#include <GL/freeglut.h>

#include "CImg.h"
#include "Punto3D.h"

using namespace cimg_library;
using namespace std;

class Procesador {
public:
    vector <CImg <char>> imagenes{};
    vector <vector <Punto3D>> puntosDetallados;
    vector <vector <Punto3D>> puntosBordes;
    int cantidadImagenes = 0;

public:
    CImg <char> binarizar(CImg<float> &imagen, int umbral) const;

    void leerImagenes(const string& rutasImagenes, int umbral);

    void extraerPuntosDetallados(float distanciaImagenes);

    void extraerPuntosBordes(float distanciaImagenes);

    void exportarPuntosDetallados(const string& rutaDetallados);

    void exportarPuntosBordes(const string& rutaBordes);

    void cargarArchivoDetallados(const string& rutaDetallados);

    void cargarArchivoBordes(const string& rutaBordes);
};

#endif //PROYECTO_RECONSTRUCCION_PROCESADOR_H
