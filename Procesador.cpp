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
    this->puntosDetallados.resize(this->cantidadImagenes);
    this->puntosBordes.resize(this->cantidadImagenes);
    archivoRutas.close();
}

void Procesador::extraerPuntosDetallados(float distanciaImagenes) {
    cout << "=============== Extrayendo puntos detallados ===============" << endl;
    int cantidadTotal = 0;
    //double inicio = omp_get_wtime();
    auto inicio = chrono::high_resolution_clock::now();
//#pragma omp parallel for default(none) shared(distanciaImagenes, cantidadTotal, cout)
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
        this->puntosDetallados[imagen] = puntosImagen;
        cantidadTotal += (int)puntosImagen.size();
        cout << "Se han extraido " << setw(6) << puntosImagen.size() << " puntos de la imagen " << setw(2)
             << imagen + 1 << endl;
    }
    //double fin = omp_get_wtime();
    auto fin = chrono::high_resolution_clock::now();
    chrono::duration <double> duracion = fin - inicio;
    cout << "\nSe han extraido " << setw(6) << cantidadTotal << " puntos en " << duracion.count()
         << " segundos" << endl << endl;
}

void Procesador::extraerPuntosBordes(float distanciaImagenes) {
    cout << "=============== Extrayendo puntos de bordes ===============" << endl;
    int cantidadTotal = 0;
    //double inicio = omp_get_wtime();
    auto inicio = chrono::high_resolution_clock::now();
//#pragma omp parallel for default(none) shared(distanciaImagenes, cantidadTotal, cout)
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
        this->puntosBordes[imagen] = puntosImagen;
        cantidadTotal += (int)puntosImagen.size();
        cout << "Se han extraido " << setw(6) << puntosImagen.size() << " puntos de la imagen " << setw(2)
             << imagen + 1 << endl;
    }
    //double fin = omp_get_wtime();
    auto fin = chrono::high_resolution_clock::now();
    chrono::duration <double> duracion = fin - inicio;
    cout << "\nSe han extraido " << setw(6) << cantidadTotal << " puntos en " << duracion.count()
         << " segundos" << endl;
}

void Procesador::exportarPuntosDetallados(const string &rutaDetallados) {
    cout << "\nExportando puntos detallados..." << endl;
    fstream archivoPuntosDetallados;
    archivoPuntosDetallados.open(rutaDetallados.c_str(), ios::out);
    archivoPuntosDetallados << this->cantidadImagenes << endl;
    for (auto &imagen : this->puntosDetallados) {
        archivoPuntosDetallados << imagen.size() << endl;
    }
    for (auto &imagen : this->puntosDetallados) {
        for (auto &punto : imagen) {
            archivoPuntosDetallados << punto.coordenadaX << ' ' << punto.coordenadaY << ' ' << punto.coordenadaZ << endl;
        }
    }
    archivoPuntosDetallados.close();
}

void Procesador::exportarPuntosBordes(const string &rutaBordes) {
    cout << "\nExportando puntos bordes..." << endl;
    fstream archivoPuntosBordes;
    archivoPuntosBordes.open(rutaBordes.c_str(), ios::out);
    archivoPuntosBordes << this->cantidadImagenes << endl;
    for (auto &imagen : this->puntosBordes) {
        archivoPuntosBordes << imagen.size() <<  endl;
    }
    for (auto &imagen : this->puntosBordes) {
        for (auto &punto : imagen) {
            archivoPuntosBordes << punto.coordenadaX << ' ' << punto.coordenadaY << ' ' << punto.coordenadaZ << endl;
        }
    }
    archivoPuntosBordes.close();
}

void Procesador::cargarArchivoDetallados(const string &rutaDetallados) {
    cout << "\nLeyendo puntos detallados..." << endl;
    ifstream archivoPuntosDetallados;
    archivoPuntosDetallados.open(rutaDetallados.c_str(), ios::in);
    string lineaLeidaDetallados;

    int cantidadElementos = 0;
    getline(archivoPuntosDetallados, lineaLeidaDetallados);
    sscanf(lineaLeidaDetallados.c_str(), "%d", &cantidadElementos);
    this->cantidadImagenes = cantidadElementos;

    vector <int> cantidadPuntosImagenes;
    for (int i = 0; i < cantidadElementos; i++) {
        int cantidadPuntos = 0;
        getline(archivoPuntosDetallados, lineaLeidaDetallados);
        sscanf(lineaLeidaDetallados.c_str(), "%d", &cantidadPuntos);
        cantidadPuntosImagenes.emplace_back(cantidadPuntos);
    }

    for (auto &elementoImagen : cantidadPuntosImagenes) {
        vector <Punto3D> puntosImagenDetallados;
        for (int puntosImagen = 0; puntosImagen < elementoImagen; puntosImagen++) {
            getline(archivoPuntosDetallados, lineaLeidaDetallados);
            float x, y, z;
            sscanf(lineaLeidaDetallados.c_str(), "%f %f %f", &x, &y, &z);
            Punto3D punto3D(x, y, z);
            puntosImagenDetallados.emplace_back(punto3D);
        }
        this->puntosDetallados.emplace_back(puntosImagenDetallados);
    }
    archivoPuntosDetallados.close();
}

void Procesador::cargarArchivoBordes(const string &rutaBordes) {
    cout << "\nLeyendo puntos bordes..." << endl;
    ifstream archivoPuntosBordes;
    archivoPuntosBordes.open(rutaBordes.c_str(), ios::in);
    string lineaLeidaBordes;

    int cantidadElementos = 0;
    getline(archivoPuntosBordes, lineaLeidaBordes);
    sscanf(lineaLeidaBordes.c_str(), "%d", &cantidadElementos);
    this->cantidadImagenes = cantidadElementos;

    vector <int> cantidadPuntosImagenes;
    for (int i = 0; i < cantidadElementos; i++) {
        int cantidadPuntos = 0;
        getline(archivoPuntosBordes, lineaLeidaBordes);
        sscanf(lineaLeidaBordes.c_str(), "%d", &cantidadPuntos);
        cantidadPuntosImagenes.emplace_back(cantidadPuntos);
    }

    for (auto &elementoImagen : cantidadPuntosImagenes) {
        vector <Punto3D> puntosImagenDetallados;
        for (int puntosImagen = 0; puntosImagen < elementoImagen; puntosImagen++) {
            getline(archivoPuntosBordes, lineaLeidaBordes);
            float x, y, z;
            sscanf(lineaLeidaBordes.c_str(), "%f %f %f", &x, &y, &z);
            Punto3D punto3D(x, y, z);
            puntosImagenDetallados.emplace_back(punto3D);
        }
        this->puntosBordes.emplace_back(puntosImagenDetallados);
    }
    archivoPuntosBordes.close();
}
