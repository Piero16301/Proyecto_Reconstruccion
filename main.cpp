#include "CargadorImagenes.h"

int main() {
    CargadorImagenes lectorImagenes;
    lectorImagenes.leerImagenes("../imagenes.txt", 100);
    /*for (auto &it : lectorImagenes.imagenes) {
        cout << "Negro : '" << to_string(it(30,230, 1, 1)) << "'" << endl;
        cout << "Blanco: '" << to_string(it(270,90, 1, 1)) << "'" << endl;
        cout << "Punto : '" << to_string(it(0,0,1,1)) << "'" << endl;
        it.display();
    }*/
    lectorImagenes.extraerPuntos();
    cout << "FIN" << endl;
    return 0;
}
