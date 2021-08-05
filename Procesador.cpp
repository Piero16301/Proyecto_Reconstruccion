#include "Procesador.h"

CImg <char> Procesador::binarizar(CImg<> &imagen, int umbral) const {
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

void Procesador::leerImagenes(const string& rutasImagenes, int umbral) {
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

void Procesador::extraerPuntosDetallados(float distanciaImagenes) {
    cout << "=============== Extrayendo puntos detallados ===============" << endl;
    double inicio = omp_get_wtime();
//#pragma omp parallel for default(none) shared(distanciaImagenes, cout)
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
//#pragma omp critical
        this->puntosDetallados.insert(this->puntosDetallados.end(), puntosImagen.begin(), puntosImagen.end());
        int ultimaCantidad = (imagen == 0) ? 0 : this->cantidadPuntosDetallados.back();
        this->cantidadPuntosDetallados.emplace_back(ultimaCantidad + puntosImagen.size());
        cout << "Se han extraido " << setw(6) << puntosImagen.size() << " puntos de la imagen " << setw(2)
             << imagen + 1 << " usando el thread " << omp_get_thread_num() << endl;
    }
    double fin = omp_get_wtime();
    cout << "\nSe han extraido " << setw(6) << this->puntosDetallados.size() << " puntos en " << fin - inicio
         << " segundos" << endl << endl;
}

void Procesador::extraerPuntosBordes(float distanciaImagenes) {
    cout << "=============== Extrayendo puntos de bordes ===============" << endl;
    double inicio = omp_get_wtime();
//#pragma omp parallel for default(none) shared(distanciaImagenes, cout)
    for (int imagen = 0; imagen < this->cantidadImagenes; imagen++) {
        CImg <char> imagenActual = this->imagenes[imagen];
        vector <Punto3D> puntosImagen;
        for (int y = 0; y < 512; y++) {
            vector <Punto3D> puntosFila;
            for (int x = 0; x < 512; x++) {
                if (to_string(imagenActual.atXYZC(x, y, 1, 1))[0] == '0') {
                    if (puntosFila.empty() || puntosFila.size() == 1) {
                        Punto3D punto3D((float)x - 256, (float)y - 256, (float)imagen * distanciaImagenes);
                        puntosFila.emplace_back(punto3D);
                    } else {
                        puntosFila.pop_back();
                        Punto3D punto3D((float)x - 256, (float)y - 256, (float)imagen * distanciaImagenes);
                        puntosFila.emplace_back(punto3D);
                    }
                }
            }
            puntosImagen.insert(puntosImagen.end(), puntosFila.begin(), puntosFila.end());
        }
//#pragma omp critical
        this->puntosBordes.insert(this->puntosBordes.end(), puntosImagen.begin(), puntosImagen.end());
        int ultimaCantidad = (imagen == 0) ? 0 : this->cantidadPuntosBordes.back();
        this->cantidadPuntosBordes.emplace_back(ultimaCantidad + puntosImagen.size());
        cout << "Se han extraido " << setw(6) << puntosImagen.size() << " puntos de la imagen " << setw(2)
             << imagen + 1 << " usando el thread " << omp_get_thread_num() << endl;
    }
    double fin = omp_get_wtime();
    cout << "\nSe han extraido " << setw(6) << this->puntosBordes.size() << " puntos en " << fin - inicio
         << " segundos" << endl;
}

void Procesador::exportarPuntos(const string& rutaDetallados, const string& rutaBordes) {
    cout << "\nExportando puntos detallados..." << endl;
    fstream archivoPuntosDetallados;
    archivoPuntosDetallados.open(rutaDetallados.c_str(), ios::out);
    for (auto &punto : this->puntosDetallados) {
        archivoPuntosDetallados << punto.coordenadaX << ' ' << punto.coordenadaY << ' ' << punto.coordenadaZ << endl;
    }
    archivoPuntosDetallados.close();

    cout << "Exportando puntos bordes..." << endl;
    fstream archivoPuntosBordes;
    archivoPuntosBordes.open(rutaBordes.c_str(), ios::out);
    for (auto &punto : this->puntosBordes) {
        archivoPuntosBordes << punto.coordenadaX << ' ' << punto.coordenadaY << ' ' << punto.coordenadaZ << endl;
    }
    archivoPuntosBordes.close();

    cout << "Fin de la exportacion" << endl;
}

void Procesador::cargarArchivo(const string& rutaDetallados, const string& rutaBordes) {
    ifstream archivoPuntosDetallados;
    archivoPuntosDetallados.open(rutaDetallados.c_str(), ios::in);
    string lineaLeidaDetallados;
    vector <Punto3D> puntosImagenDetallados;
    while (!archivoPuntosDetallados.eof()) {
        getline(archivoPuntosDetallados, lineaLeidaDetallados);
        float x, y, z;
        sscanf(lineaLeidaDetallados.c_str(), "%f %f %f", &x, &y, &z);
        Punto3D punto3D(x, y, z);
        puntosImagenDetallados.emplace_back(punto3D);
    }
    this->puntosDetallados.insert(this->puntosDetallados.end(), puntosImagenDetallados.begin(), puntosImagenDetallados.end());
    archivoPuntosDetallados.close();

    ifstream archivoPuntosBordes;
    archivoPuntosBordes.open(rutaBordes.c_str(), ios::in);
    string lineaLeidaBordes;
    vector <Punto3D> puntosImagenBordes;
    while (!archivoPuntosBordes.eof()) {
        getline(archivoPuntosBordes, lineaLeidaBordes);
        float x, y, z;
        sscanf(lineaLeidaBordes.c_str(), "%f %f %f", &x, &y, &z);
        Punto3D punto3D(x, y, z);
        puntosImagenBordes.emplace_back(punto3D);
    }
    this->puntosBordes.insert(this->puntosBordes.end(), puntosImagenBordes.begin(), puntosImagenBordes.end());
    archivoPuntosBordes.close();
}
