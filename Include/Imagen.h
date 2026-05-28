#ifndef IMAGEN_H
#define IMAGEN_H

#include <iostream>
#include "Capa.h"

using namespace std;

// Nodo para la lista de capas dentro de la imagen
// Se usa un enlace simple como opción permitida [cite: 154]
struct NodoCapaImagen {
    Capa* capa; // Apuntador a la capa original, NO una copia 
    NodoCapaImagen* siguiente;

    NodoCapaImagen(Capa* c) : capa(c), siguiente(nullptr) {}
};

class Imagen {
public:
    int idImagen;
    NodoCapaImagen* cabezaCapas;

    Imagen(int id) {
        idImagen = id;
        cabezaCapas = nullptr;
    }

    // El orden en el que se insertan las capas indica cómo se superponen 
    void agregarCapa(Capa* c) {
        NodoCapaImagen* nuevo = new NodoCapaImagen(c);
        if (cabezaCapas == nullptr) {
            cabezaCapas = nuevo;
        } else {
            NodoCapaImagen* aux = cabezaCapas;
            while (aux->siguiente != nullptr) {
                aux = aux->siguiente;
            }
            aux->siguiente = nuevo;
        }
        cout << "Capa " << c->idCapa << " enlazada a la Imagen " << idImagen << endl;
    }
    
    // Método extra para imprimir las capas que conforman la imagen
    void mostrarCapas() {
        NodoCapaImagen* aux = cabezaCapas;
        cout << "Imagen " << idImagen << " compuesta por capas: ";
        while (aux != nullptr) {
            cout << aux->capa->idCapa << " ";
            aux = aux->siguiente;
        }
        cout << endl;
    }
};

#endif