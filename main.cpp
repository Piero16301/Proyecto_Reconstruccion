#include "ProcesadorImagenes.h"

void mostrarImagenes(ProcesadorImagenes &procesadorImagenes) {
    for (auto &it : procesadorImagenes.imagenes) {
        cout << "Negro : '" << to_string(it.atXYZC(242,34, 1, 1)) << "'" << endl;
        cout << "Blanco: '" << to_string(it.atXYZC(243,34, 1, 1)) << "'" << endl;
        cout << "Punto : '" << to_string(it.atXYZC(244,34, 1, 1)) << "'" << endl;
        it.display();
    }
}

int main() {
    ProcesadorImagenes procesadorImagenes;
    procesadorImagenes.leerImagenes("../imagenes.txt", 100);
    // mostrarImagenes(procesadorImagenes);
    procesadorImagenes.extraerPuntos(7.0);
    procesadorImagenes.exportarPuntos("../../Puntos/puntos.txt");

    cout << "FIN" << endl;
    return 0;
}
