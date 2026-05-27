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
    cout << "5. Ver imagen y arbol de capas combinados" << endl;
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
        case 5: {
            int idImg;
            cout << "Ingrese el ID de la imagen: ";
            cin >> idImg;
            Imagen* imgEncontrada = listaImagenes->buscar(idImg);
            if (imgEncontrada) {
                arbolCapas->graficarImagenYArbol(imgEncontrada);
            } else {
                cout << "La imagen " << idImg << " no existe en el sistema." << endl;
            }
            break;
        }
        default:
            cout << "Opcion no valida." << endl;
    }
}

void menuCRUD() {
    int opcion, opSub;
    cout << "\n--- GESTION DE USUARIOS E IMAGENES (CRUD) ---" << endl;
    cout << "1. Operaciones de Usuarios" << endl;
    cout << "2. Operaciones de Imagenes" << endl;
    cout << "Elija una opcion: ";
    cin >> opcion;

    if (opcion == 1) {
        cout << "\n1. Agregar Usuario | 2. Modificar Usuario | 3. Eliminar Usuario" << endl;
        cout << "Elija una opcion: ";
        cin >> opSub;
        
        if (opSub == 1) {
            string nombre;
            cout << "Ingrese el nombre del nuevo usuario: ";
            cin >> nombre;
            if (arbolUsuarios->search(nombre) == nullptr) {
                arbolUsuarios->insert(new Usuario(nombre));
            } else {
                cout << "[!] El usuario ya existe." << endl;
            }
        } 
        else if (opSub == 2) {
            string nombreAntiguo, nombreNuevo;
            cout << "Ingrese el nombre del usuario a modificar: ";
            cin >> nombreAntiguo;
            Usuario* user = arbolUsuarios->search(nombreAntiguo);
            if (user != nullptr) {
                cout << "Ingrese el nuevo nombre para el usuario: ";
                cin >> nombreNuevo;
                if (arbolUsuarios->search(nombreNuevo) == nullptr) {
                    NodoImagenUsuario* aux = user->cabezaImagenes;
                    Usuario* nuevoUser = new Usuario(nombreNuevo);
                    while (aux != nullptr) {
                        nuevoUser->agregarImagen(aux->idImagen);
                        aux = aux->siguiente;
                    }
                    arbolUsuarios->eliminarUsuario(nombreAntiguo);
                    arbolUsuarios->insert(nuevoUser);
                    cout << ">> Usuario modificado con exito." << endl;
                } else {
                    cout << "[!] El nuevo nombre ya esta ocupado." << endl;
                }
            } else {
                cout << "[!] El usuario no existe." << endl;
            }
        } 
        else if (opSub == 3) {
            string nombre;
            cout << "Ingrese el nombre del usuario a eliminar: ";
            cin >> nombre;
            if (arbolUsuarios->eliminarUsuario(nombre)) {
                cout << ">> Usuario eliminado del sistema." << endl;
            } else {
                cout << "[!] No se encontro el usuario." << endl;
            }
        }
    } 
    else if (opcion == 2) {
        cout << "\n1. Agregar Imagen a Usuario | 2. Eliminar Imagen de Usuario" << endl;
        cout << "Elija una opcion: ";
        cin >> opSub;

        if (opSub == 1) {
            string nombre;
            int idNuevaImg;
            cout << "Ingrese el nombre del usuario: ";
            cin >> nombre;
            Usuario* user = arbolUsuarios->search(nombre);
            if (user != nullptr) {
                cout << "Ingrese el ID de la nueva imagen (debe ser unico): ";
                cin >> idNuevaImg;
                if (listaImagenes->buscar(idNuevaImg) == nullptr) {
                    Imagen* nuevaImg = new Imagen(idNuevaImg);
                    listaImagenes->insertar(nuevaImg);
                    user->agregarImagen(idNuevaImg);
                } else {
                    cout << "[!] El ID de imagen ya existe en el sistema." << endl;
                }
            } else {
                cout << "[!] El usuario no existe." << endl;
            }
        } 
        else if (opSub == 2) {
            string nombre;
            int idImgEliminar;
            cout << "Ingrese el nombre del usuario: ";
            cin >> nombre;
            Usuario* user = arbolUsuarios->search(nombre);
            if (user != nullptr) {
                cout << "Ingrese el ID de la imagen a eliminar: ";
                cin >> idImgEliminar;
                if (user->eliminarImagenReferencia(idImgEliminar)) {
                    listaImagenes->eliminar(idImgEliminar);
                    cout << ">> Imagen eliminada de la cuenta del usuario y de la memoria general." << endl;
                } else {
                    cout << "[!] El usuario no tiene asignada esa imagen." << endl;
                }
            } else {
                cout << "[!] El usuario no existe." << endl;
            }
        }
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
                int opGen;
                cout << "\n--- GENERACION DE IMAGENES ---" << endl;
                cout << "1. Por recorrido limitado (ABB Capas)" << endl;
                cout << "2. Por lista de imagenes (Circular Doble)" << endl;
                cout << "3. Por capa individual" << endl;
                cout << "4. Por usuario" << endl;
                cout << "Elija una opcion: ";
                cin >> opGen;

                if (opGen == 1) {
                    int tipoRecorrido, numCapas;
                    cout << "1. Preorden | 2. Inorden | 3. Postorden" << endl;
                    cout << "Tipo de recorrido: ";
                    cin >> tipoRecorrido;
                    cout << "Numero de capas a utilizar: ";
                    cin >> numCapas;

                    Imagen* imgGenerada = nullptr;
                    if (tipoRecorrido == 1) imgGenerada = arbolCapas->obtenerImagenPreorden(numCapas);
                    else if (tipoRecorrido == 2) imgGenerada = arbolCapas->obtenerImagenInorden(numCapas);
                    else if (tipoRecorrido == 3) imgGenerada = arbolCapas->obtenerImagenPostorden(numCapas);

                    if (imgGenerada && imgGenerada->cabezaCapas != nullptr) {
                        motorGrafico->generarDesdeImagen(imgGenerada, "recorrido_limitado");
                        delete imgGenerada; 
                    } else {
                        cout << "No se pudo generar. Verifique si el arbol de capas tiene datos." << endl;
                    }

                } else if (opGen == 2) {
                    int idBuscado;
                    cout << "Ingrese el ID de la imagen a generar: ";
                    cin >> idBuscado;
                    Imagen* imgEncontrada = listaImagenes->buscar(idBuscado);
                    if (imgEncontrada) {
                        motorGrafico->generarDesdeImagen(imgEncontrada, "imagen_generada_" + to_string(idBuscado));
                    } else {
                        cout << "La imagen no existe." << endl;
                    }

                } else if (opGen == 3) {
                    int idCapa;
                    cout << "Ingrese el ID de la capa: ";
                    cin >> idCapa;
                    Capa* capaEncontrada = arbolCapas->search(idCapa);
                    if (capaEncontrada) {
                        Imagen* imgTmp = new Imagen(0);
                        imgTmp->agregarCapa(capaEncontrada);
                        motorGrafico->generarDesdeImagen(imgTmp, "capa_individual_" + to_string(idCapa));
                        delete imgTmp;
                    } else {
                        cout << "La capa no existe." << endl;
                    }

                } else if (opGen == 4) {
                    string nomUsuario;
                    cout << "Ingrese el nombre del usuario: ";
                    cin >> nomUsuario;
                    Usuario* userEncontrado = arbolUsuarios->search(nomUsuario);
                    if (userEncontrado) {
                        userEncontrado->mostrarImagenes();
                        int idImgUser;
                        cout << "Ingrese el ID de la imagen a graficar de la lista anterior: ";
                        cin >> idImgUser;
                        Imagen* imgDelUser = listaImagenes->buscar(idImgUser);
                        if (imgDelUser) {
                            motorGrafico->generarDesdeImagen(imgDelUser, "imagen_usuario_" + nomUsuario);
                        } else {
                            cout << "Esa imagen no existe en el sistema general." << endl;
                        }
                    } else {
                        cout << "El usuario no existe." << endl;
                    }
                } else {
                    cout << "Opcion invalida." << endl;
                }
                break;
            }
            case 3:
                menuReportes();
                break;
            case 4:
                menuCRUD();
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
