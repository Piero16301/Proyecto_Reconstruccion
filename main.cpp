#include "CargadorImagenes.h"

int main() {
    CargadorImagenes lectorImagenes;
    lectorImagenes.leerImagenes("../imagenes.txt", 100);
    /*for (auto &it : lectorImagenes.imagenes) {
        cout << "Negro : '" << to_string(it.atXYZC(242,34, 1, 1)) << "'" << endl;
        cout << "Blanco: '" << to_string(it.atXYZC(243,34, 1, 1)) << "'" << endl;
        cout << "Punto : '" << to_string(it.atXYZC(244,34, 1, 1)) << "'" << endl;
        it.display();
    }*/
    lectorImagenes.extraerPuntos();
    cout << "FIN" << endl;
    return 0;
}
