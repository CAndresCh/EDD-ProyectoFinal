#ifndef LISTA_IMAGENES_H
#define LISTA_IMAGENES_H

#include <iostream>
#include <fstream>
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

    void graficarListaImagenes() {
        ofstream archivo("reporte_imagenes.dot");
        if (!archivo.is_open()) {
            cout << "Error al crear el archivo .dot de imagenes" << endl;
            return;
        }

        archivo << "digraph ListaImagenes {" << endl;
        archivo << "    rankdir=LR;" << endl; // De izquierda a derecha
        archivo << "    node [shape=box, style=filled, fillcolor=lightgreen];" << endl;

        if (cabeza == nullptr) {
            archivo << "    vacia [label=\"Lista de Imagenes Vacia\"];" << endl;
            archivo << "}" << endl;
            archivo.close();
            return;
        }

        NodoImagen* actual = cabeza;
        do {
            // Crear el nodo actual
            archivo << "    node_" << actual->imagen->idImagen << " [label=\"Imagen " << actual->imagen->idImagen << "\"];" << endl;
            
            // Enlace al siguiente
            archivo << "    node_" << actual->imagen->idImagen << " -> node_" << actual->siguiente->imagen->idImagen << ";" << endl;
            // Enlace al anterior
            archivo << "    node_" << actual->siguiente->imagen->idImagen << " -> node_" << actual->imagen->idImagen << ";" << endl;
            
            actual = actual->siguiente;
        } while (actual != cabeza);

        archivo << "}" << endl;
        archivo.close();

        system("dot -Tpng reporte_imagenes.dot -o reporte_imagenes.png");
        cout << ">> Reporte generado exitosamente: reporte_imagenes.png" << endl;
    }

    bool eliminar(int id) {
        if (cabeza == nullptr) return false;

        NodoImagen* actual = cabeza;
        bool encontrado = false;

        do {
            if (actual->imagen->idImagen == id) {
                encontrado = true;
                break;
            }
            actual = actual->siguiente;
        } while (actual != cabeza);

        if (!encontrado) return false;

        // Caso único: Es el único nodo en la lista circular
        if (actual->siguiente == actual && actual->anterior == actual) {
            cabeza = nullptr;
        } else {
            // Desenlazar
            actual->anterior->siguiente = actual->siguiente;
            actual->siguiente->anterior = actual->anterior;
            if (actual == cabeza) {
                cabeza = actual->siguiente;
            }
        }
        delete actual->imagen; // Liberar la estructura interna de la imagen
        delete actual;        // Liberar el nodo de la lista
        return true;
    }

};

#endif