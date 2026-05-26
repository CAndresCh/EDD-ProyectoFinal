#include <iostream>
#include <string>
#include "ABBCapas.h"
#include "ListaImagenes.h"
#include "ABBUsuarios.h"
#include "CargaMasiva.h"
#include "Generador.h"

using namespace std;

// Instancias globales 
ABBCapas* arbolCapas = new ABBCapas();
ListaCircularImagenes* listaImagenes = new ListaCircularImagenes();
ABBUsuarios* arbolUsuarios = new ABBUsuarios();
CargaMasiva* gestorArchivos = new CargaMasiva();
Generador* motorGrafico = new Generador();

void menuCargaMasiva() {
    int opcion;
    string rutaBase = "C:\\Users\\carlo\\Desktop\\Proyecto final estructura de datos\\";
    cout << "\n--- MENU CARGA MASIVA ---" << endl;
    cout << "1. Cargar Capas (.cap)" << endl;
    cout << "2. Cargar Imagenes (.im)" << endl;
    cout << "3. Cargar Usuarios (.usr)" << endl;
    cout << "4. Cargar TODOS los archivos de una vez" << endl;
    cout << "Elija una opcion: ";
    cin >> opcion;

    switch (opcion) {
        case 1:
            gestorArchivos->cargarCapas(rutaBase + "capas.cap", arbolCapas);
            break;
        case 2:
            gestorArchivos->cargarImagenes(rutaBase + "imagenes.im", listaImagenes, arbolCapas);
            break;
        case 3:
            gestorArchivos->cargarUsuarios(rutaBase + "usuarios.usr", arbolUsuarios);
            break;
        case 4:
            cout << "\nIniciando carga completa automatica..." << endl;
            gestorArchivos->cargarCapas(rutaBase + "capas.cap", arbolCapas);
            gestorArchivos->cargarImagenes(rutaBase + "imagenes.im", listaImagenes, arbolCapas);
            gestorArchivos->cargarUsuarios(rutaBase + "usuarios.usr", arbolUsuarios);
            break;
        default:
            cout << "Opcion no valida." << endl;
    }
}

void menuReportes() {
    int opcion;
    cout << "\n--- MENU DE REPORTES ---" << endl;
    cout << "1. Ver arbol de usuarios" << endl;
    cout << "2. Ver arbol de capas" << endl;
    cout << "3. Ver lista de imagenes" << endl;
    cout << "4. Ver capa especifica (Matriz Dispersa)" << endl;
    cout << "Elija una opcion: ";
    cin >> opcion;

    switch (opcion) {
        case 1:
            arbolUsuarios->graficarArbolUsuarios();
            break;
        case 2:
            arbolCapas->graficarArbolCapas();
            break;
        case 3:
            listaImagenes->graficarListaImagenes();
            break;
        case 4: {
            int idCapa;
            cout << "Ingrese el ID de la capa a graficar: ";
            cin >> idCapa;
            Capa* capaBuscada = arbolCapas->search(idCapa);
            if (capaBuscada) {
                capaBuscada->pixeles->graficarMatriz(idCapa); 
            } else {
                cout << "La capa " << idCapa << " no existe en el sistema." << endl;
            }
            break;
        }
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
            case 2: {
                int idBuscado;
                cout << "\n--- GENERACION POR LISTA DE IMAGENES ---" << endl;
                cout << "Ingrese el ID de la imagen a generar: ";
                cin >> idBuscado;
                
                Imagen* imgEncontrada = listaImagenes->buscar(idBuscado); 
                if (imgEncontrada) {
                    string nombreSalida = "imagen_generada_" + to_string(idBuscado);
                    motorGrafico->generarDesdeImagen(imgEncontrada, nombreSalida); 
                } else {
                    cout << "La imagen con ID " << idBuscado << " no se encuentra en el sistema." << endl;
                }
                break;
            }
            case 3:
                menuReportes();
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
