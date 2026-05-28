#ifndef GENERADOR_H
#define GENERADOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Imagen.h"
#include "MatrizDispersa.h"

using namespace std;

class Generador {
private:
    // Se aumentó el límite para evitar que se corten las imágenes
    const int MAX_FILAS = 100;
    const int MAX_COLS = 100;

public:
    void generarDesdeImagen(Imagen* img, string nombreArchivoSalida) {
        if (!img || !img->cabezaCapas) {
            cout << "La imagen esta vacia o no existe." << endl;
            return;
        }

        int maxFilaUsada = 0;
        int maxColUsada = 0;

        // 1. Crear un lienzo virtual en blanco (transparente)
        // Usamos vector para no desbordar la memoria (Stack Overflow)
        vector<vector<string>> lienzo(MAX_FILAS, vector<string>(MAX_COLS, "white"));

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
                        if (r > maxFilaUsada) maxFilaUsada = r;
                        if (c > maxColUsada) maxColUsada = c;
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

        // Se genera la tabla dinámicamente solo hasta donde haya píxeles
        for (int i = 0; i <= maxFilaUsada; i++) {
            archivo << "            <TR>" << endl;
            for (int j = 0; j <= maxColUsada; j++) {
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