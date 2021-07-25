#include "ProcesadorImagenes.h"

CImg <char> ProcesadorImagenes::binarizar(CImg<> &imagen, int umbral) const {
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

void ProcesadorImagenes::leerImagenes(const string& rutasImagenes, int umbral) {
    ifstream archivoRutas(rutasImagenes.c_str());
    string ruta;
    while (getline(archivoRutas, ruta)) {
        CImg <float> imagenInicial(ruta.c_str());
        CImg <char> imagenFinal = binarizar(imagenInicial, umbral);
        this->imagenes.emplace_back(imagenFinal);
        this->cantidadImagenes++;
    }
    archivoRutas.close();
}

void ProcesadorImagenes::extraerPuntos(float distanciaImagenes) {
    int totalPuntos = 0;
    for (int imagen = 0; imagen < this->cantidadImagenes; imagen++) {
        CImg <char> imagenActual = this->imagenes[imagen];
        vector <Punto3D> puntosImagen;
        cimg_forXY(imagenActual, x, y) {
            if (to_string(imagenActual.atXYZC(x, y, 1, 1))[0] != to_string(imagenActual.atXYZC(x + 1, y, 1, 1))[0]) {
                if (to_string(imagenActual.atXYZC(x, y, 1, 1))[0] == '0') {
                    Punto3D punto3D((float)x - 256, (float)y - 256, (float)imagen * distanciaImagenes);
                    puntosImagen.emplace_back(punto3D);
                } else {
                    Punto3D punto3D((float)x + 1 - 256, (float)y - 256, (float)imagen * distanciaImagenes);
                    puntosImagen.emplace_back(punto3D);
                }
            }
        }
        this->puntos.emplace_back(puntosImagen);
        cout << "Se han extraido " << setw(6) << puntosImagen.size() << " puntos de la imagen " << imagen + 1 << endl;
        totalPuntos += (int)puntosImagen.size();
    }
    cout << "\nSe han extraido " << setw(6) << totalPuntos << " puntos en total" << endl;
}

void ProcesadorImagenes::exportarPuntos(const string& ruta) {
    cout << "\nExportando puntos..." << endl;
    fstream archivoPuntos;
    archivoPuntos.open(ruta.c_str(), ios::out);
    for (auto &imagen : this->puntos) {
        for (auto &punto : imagen) {
            archivoPuntos << punto.coordenadaX << ' ' << punto.coordenadaY << ' ' << punto.coordenadaZ << endl;
        }
    }
    archivoPuntos.close();
    cout << "Fin de la exportacion" << endl;
}

void ProcesadorImagenes::cargarArchivo(const string &rutaPuntos) {
    ifstream archivo;
    archivo.open(rutaPuntos.c_str(), ios::in);
    string lineaLeida;
    vector <Punto3D> puntosImagen;
    while (!archivo.eof()) {
        getline(archivo, lineaLeida);
        float x, y, z;
        sscanf(lineaLeida.c_str(), "%f %f %f", &x, &y, &z);
        Punto3D punto3D(x, y, z);
        puntosImagen.emplace_back(punto3D);
    }
    this->puntos.emplace_back(puntosImagen);
}
