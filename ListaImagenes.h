#ifndef LISTA_IMAGENES_H
#define LISTA_IMAGENES_H

#include <iostream>
#include "Imagen.h"

using namespace std;

struct NodoImagen {
    Imagen* imagen;
    NodoImagen* siguiente;
    NodoImagen* anterior;

    NodoImagen(Imagen* img) : imagen(img), siguiente(nullptr), anterior(nullptr) {}
};

class ListaCircularImagenes {
private:
    NodoImagen* cabeza;

public:
    ListaCircularImagenes() {
        cabeza = nullptr;
    }

    // Inserta manteniendo la lista ordenada por ID 
    void insertar(Imagen* img) {
        NodoImagen* nuevo = new NodoImagen(img);

        if (cabeza == nullptr) {
            cabeza = nuevo;
            cabeza->siguiente = cabeza;
            cabeza->anterior = cabeza;
            cout << "Imagen " << img->idImagen << " insertada como cabeza." << endl;
            return;
        }

        // Buscar la posición correcta para mantener el orden
        NodoImagen* actual = cabeza;
        
        // Caso 1: Va antes de la cabeza actual
        if (img->idImagen < cabeza->imagen->idImagen) {
            NodoImagen* ultimo = cabeza->anterior;
            nuevo->siguiente = cabeza;
            nuevo->anterior = ultimo;
            cabeza->anterior = nuevo;
            ultimo->siguiente = nuevo;
            cabeza = nuevo; // Movemos la cabeza
        } 
        // Caso 2: Va en medio o al final
        else {
            while (actual->siguiente != cabeza && actual->siguiente->imagen->idImagen < img->idImagen) {
                actual = actual->siguiente;
            }
            // Inserción doblemente enlazada
            nuevo->siguiente = actual->siguiente;
            nuevo->anterior = actual;
            actual->siguiente->anterior = nuevo;
            actual->siguiente = nuevo;
        }
        cout << "Imagen " << img->idImagen << " insertada correctamente." << endl;
    }

    // Buscar una imagen por su ID para generación de imágenes u operaciones CRUD [cite: 211, 221]
    Imagen* buscar(int id) {
        if (cabeza == nullptr) return nullptr;
        
        NodoImagen* actual = cabeza;
        do {
            if (actual->imagen->idImagen == id) {
                return actual->imagen;
            }
            actual = actual->siguiente;
        } while (actual != cabeza);
        
        return nullptr; // No se encontró
    }
};

#endif