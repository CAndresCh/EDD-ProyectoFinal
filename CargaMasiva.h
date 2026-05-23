#ifndef CARGA_MASIVA_H
#define CARGA_MASIVA_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "ABBCapas.h"
#include "ListaImagenes.h"
#include "ABBUsuarios.h"

using namespace std;

class CargaMasiva {
public:
    void cargarCapas(string ruta, ABBCapas* arbolCapas) {
        ifstream archivo(ruta);
        string linea;
        
        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo de capas." << endl;
            return;
        }
        while (getline(archivo, linea)) {

        }
        archivo.close();
        cout << "Carga masiva de capas finalizada." << endl;
    }

    // Método para leer archivos .im
    void cargarImagenes(string ruta, ListaCircularImagenes* listaImagenes, ABBCapas* arbolCapas) {
        ifstream archivo(ruta);
        string linea;
        
        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo de imagenes." << endl;
            return;
        }

        while (getline(archivo, linea)) {
   
        }
        archivo.close();
        cout << "Carga masiva de imagenes finalizada." << endl;
    }

    // Método para leer archivos .usr
    void cargarUsuarios(string ruta, ABBUsuarios* arbolUsuarios) {
        ifstream archivo(ruta);
        string linea;
        
        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo de usuarios." << endl;
            return;
        }

        while (getline(archivo, linea)) {

        }
        archivo.close();
        cout << "Carga masiva de usuarios finalizada." << endl;
    }
};

#endif
