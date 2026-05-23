#ifndef USUARIO_H
#define USUARIO_H

#include <iostream>
#include <string>

using namespace std;

// Nodo para la lista simple de imágenes que le pertenecen al usuario
struct NodoImagenUsuario {
    int idImagen;
    NodoImagenUsuario* siguiente;

    NodoImagenUsuario(int id) : idImagen(id), siguiente(nullptr) {}
};

class Usuario {
public:
    string nombre;
    NodoImagenUsuario* cabezaImagenes;

    Usuario(string n) {
        nombre = n;
        cabezaImagenes = nullptr;
    }

    // Inserta al final de la lista simple
    void agregarImagen(int idImagen) {
        NodoImagenUsuario* nuevo = new NodoImagenUsuario(idImagen);
        if (cabezaImagenes == nullptr) {
            cabezaImagenes = nuevo;
        } else {
            NodoImagenUsuario* aux = cabezaImagenes;
            while (aux->siguiente != nullptr) {
                aux = aux->siguiente;
            }
            aux->siguiente = nuevo;
        }
        cout << "Imagen " << idImagen << " asociada al usuario " << nombre << endl;
    }

    void mostrarImagenes() {
        cout << "Imágenes de " << nombre << ": ";
        NodoImagenUsuario* aux = cabezaImagenes;
        if (!aux) {
            cout << "Ninguna." << endl;
            return;
        }
        while (aux != nullptr) {
            cout << aux->idImagen << " ";
            aux = aux->siguiente;
        }
        cout << endl;
    }
};

#endif
