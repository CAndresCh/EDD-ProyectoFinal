#include <iostream>
#include <string>
#include "ABBCapas.h"
#include "ListaImagenes.h"
#include "ABBUsuarios.h"
#include "CargaMasiva.h"

using namespace std;

// Instancias globales 
ABBCapas* arbolCapas = new ABBCapas();
ListaCircularImagenes* listaImagenes = new ListaCircularImagenes();
ABBUsuarios* arbolUsuarios = new ABBUsuarios();
CargaMasiva* gestorArchivos = new CargaMasiva();

void menuCargaMasiva() {
    int opcion;
    string ruta;
    cout << "\n--- MENU CARGA MASIVA ---" << endl;
    cout << "1. Cargar Capas (.cap)" << endl;
    cout << "2. Cargar Imagenes (.im)" << endl;
    cout << "3. Cargar Usuarios (.usr)" << endl;
    cout << "Elija una opcion: ";
    cin >> opcion;

    cout << "Ingrese la ruta del archivo: ";
    cin >> ruta;

    switch (opcion) {
        case 1:
            gestorArchivos->cargarCapas(ruta, arbolCapas);
            break;
        case 2:
            gestorArchivos->cargarImagenes(ruta, listaImagenes, arbolCapas);
            break;
        case 3:
            gestorArchivos->cargarUsuarios(ruta, arbolUsuarios);
            break;
        default:
            cout << "Opcion no valida." << endl;
    }
}

int main() {
    int opcionP;

    do {
        cout << "\n========================================" << endl;
        cout << " GENERADOR DE IMAGENES POR CAPAS " << endl;
        cout << "========================================" << endl;
        cout << "1. Carga Masiva" << endl;
        cout << "2. Generacion de Imagenes" << endl;
        cout << "3. Reportes (Estado de la memoria)" << endl;
        cout << "4. Gestion de Usuarios e Imagenes (CRUD)" << endl;
        cout << "5. Salir" << endl;
        cout << "Elija una opcion: ";
        cin >> opcionP;

        switch (opcionP) {
            case 1:
                menuCargaMasiva();
                break;
            case 2:
                cout << "Modulo en construccion..." << endl;
                break;
            case 3:
                cout << "Modulo en construccion..." << endl;
                break;
            case 4:
                cout << "Modulo en construccion..." << endl;
                break;
            case 5:
                cout << "Saliendo del sistema..." << endl;
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo." << endl;
        }
    } while (opcionP != 5);

    return 0;
}
