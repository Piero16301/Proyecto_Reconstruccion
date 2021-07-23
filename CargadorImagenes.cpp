#include "CargadorImagenes.h"

CImg <char> CargadorImagenes::binarizar(CImg<> &imagen, int umbral) const {
    CImg <char> R(imagen.width(),imagen.height());
    for(int i=0;i< imagen.width();i++) {
        for (int j = 0; j < imagen.height(); j++) {
            int r = (int)imagen(i, j, 0);
            int g = (int)imagen(i, j, 1);
            int b = (int)imagen(i, j, 2);
            if ((r + g + b) / 3 > umbral) {
                R(i, j) = (char)0;
            } else {
                R(i, j) = (char)255;
            }
            // Poner en negro los bordes
            if (i >= 440 || i <= 10 || j <= 10 || j >= 500) {
                R(i, j) = (char)255;
            }
            // Eliminar puntos de ruido de la derecha para las primeras 8 imagenes
            if (i >= 420 && this->cantidadImagenes <= 8) {
                R(i, j) = (char)255;
            }
        }
    }
    return R;
}

void CargadorImagenes::leerImagenes(const string& rutasImagenes, int umbral) {
    ifstream archivoRutas(rutasImagenes.c_str());
    string ruta;
    while (getline(archivoRutas, ruta)) {
        CImg <float> imagenInicial(ruta.c_str());
        CImg <char> imagenFinal = binarizar(imagenInicial, umbral);
        this->imagenes.emplace_back(imagenFinal);
        this->ancho = imagenFinal.width();
        this->alto = imagenFinal.height();
        this->cantidadImagenes++;
    }
    archivoRutas.close();
}

void CargadorImagenes::extraerPuntos() {
    for (int imagen = 0; imagen < this->cantidadImagenes; imagen++) {
        CImg <char> imagenActual = this->imagenes[imagen];
        vector <Punto3D> puntosImagen;
        cimg_forXY(imagenActual, x, y) {
            if (to_string(imagenActual.atXYZC(x, y, 1, 1))[0] != to_string(imagenActual.atXYZC(x + 1, y, 1, 1))[0]) {
                if (to_string(imagenActual.atXYZC(x, y, 1, 1))[0] == '0') {
                    Punto3D punto3D((float)x, (float)y, (float)imagen);
                    puntosImagen.emplace_back(punto3D);
                } else {
                    Punto3D punto3D((float)x + 1, (float)y, (float)imagen);
                    puntosImagen.emplace_back(punto3D);
                }
            }
        }
        this->puntos.emplace_back(puntosImagen);
        cout << "Termino extraer puntos imagen " << imagen << endl;
        this->imagenes[imagen].display();
    }
}
