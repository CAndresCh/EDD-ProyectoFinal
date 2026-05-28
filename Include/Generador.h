#ifndef GENERADOR_H
#define GENERADOR_H

#include <iostream>
#include <fstream>
#include <string>
#include "Imagen.h"
#include "MatrizDispersa.h"

using namespace std;

class Generador {
private:
    const int MAX_FILAS = 20;
    const int MAX_COLS = 20;

public:
    void generarDesdeImagen(Imagen* img, string nombreArchivoSalida) {
        if (!img || !img->cabezaCapas) {
            cout << "La imagen esta vacia o no existe." << endl;
            return;
        }

        // 1. Crear un lienzo virtual en blanco (transparente)
        string lienzo[MAX_FILAS][MAX_COLS];
        for (int i = 0; i < MAX_FILAS; i++) {
            for (int j = 0; j < MAX_COLS; j++) {
                lienzo[i][j] = "white"; 
            }
        }

        // 2. Superponer las capas
        NodoCapaImagen* auxCapa = img->cabezaCapas;
        while (auxCapa != nullptr) {
            Capa* capaActual = auxCapa->capa;
            
            // Recorrer la matriz dispersa de esta capa
            for (int r = 0; r < MAX_FILAS; r++) {
                for (int c = 0; c < MAX_COLS; c++) {
                    NodoMatriz* pixel = capaActual->pixeles->search(r, c);
                    if (pixel != nullptr && pixel->color != "") {
                        lienzo[r][c] = pixel->color;
                    }
                }
            }
            auxCapa = auxCapa->siguiente;
        }

        // 3. Escribir el archivo Graphviz con formato de Tabla HTML
        string nombreDot = nombreArchivoSalida + ".dot";
        ofstream archivo(nombreDot);
        if (!archivo.is_open()) {
            cout << "Error al crear el archivo .dot de la imagen" << endl;
            return;
        }

        archivo << "digraph ImagenGenerada {" << endl;
        archivo << "    node [shape=none, margin=0];" << endl;
        archivo << "    img [label=<" << endl;
        archivo << "        <TABLE BORDER=\"0\" CELLBORDER=\"0\" CELLSPACING=\"0\" CELLPADDING=\"15\">" << endl;

        for (int i = 0; i < MAX_FILAS; i++) {
            archivo << "            <TR>" << endl;
            for (int j = 0; j < MAX_COLS; j++) {
                archivo << "                <TD BGCOLOR=\"" << lienzo[i][j] << "\"></TD>" << endl;
            }
            archivo << "            </TR>" << endl;
        }

        archivo << "        </TABLE>" << endl;
        archivo << "    >];" << endl;
        archivo << "}" << endl;
        archivo.close();

        // 4. Compilar la imagen
        string nombrePng = nombreArchivoSalida + ".png";
        string comando = "dot -Tpng " + nombreDot + " -o " + nombrePng;
        system(comando.c_str());
        
        cout << ">> Imagen " << img->idImagen << " generada exitosamente: " << nombrePng << endl;
    }
};

#endif