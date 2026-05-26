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

        bool leyendoCapa = false;
        Capa* capaActual = nullptr;
        
        while (getline(archivo, linea)) {
            // Limpiar espacios y saltos de línea innecesarios
            if (linea.empty() || linea == "\r" || linea == "\n") continue;

            if (!leyendoCapa) {
                // Buscamos el inicio de una capa (ej. "0 {")
                size_t posLlaveAbre = linea.find('{');
                if (posLlaveAbre != string::npos) {
                    string idStr = linea.substr(0, posLlaveAbre);
                    // Limpiamos espacios en blanco del ID si los hay
                    idStr.erase(0, idStr.find_first_not_of(" \t"));
                    idStr.erase(idStr.find_last_not_of(" \t") + 1);

                    try {
                        int idCapa = stoi(idStr);
                        capaActual = new Capa(idCapa);
                        leyendoCapa = true;
                    } catch (...) {
                        cout << "Error leyendo ID de capa: " << idStr << endl;
                    }
                }
            } else {
                // Buscamos si es el fin de la capa actual
                if (linea.find('}') != string::npos) {
                    if (capaActual != nullptr) {
                        arbolCapas->insert(capaActual);
                        capaActual = nullptr;
                    }
                    leyendoCapa = false;
                    continue;
                }

                // Estamos dentro de una capa, leemos formato: fila, columna, color;
                size_t posPuntoComa = linea.find(';');
                if (posPuntoComa != string::npos) {
                    string datosPixel = linea.substr(0, posPuntoComa);
                    stringstream ss(datosPixel);
                    string filaStr, colStr, colorStr;

                    // Separar por comas
                    if (getline(ss, filaStr, ',') && getline(ss, colStr, ',') && getline(ss, colorStr)) {
                        try {
                            int f = stoi(filaStr);
                            int c = stoi(colStr);
                            
                            // Limpiar posibles espacios extra en el color
                            colorStr.erase(0, colorStr.find_first_not_of(" \t"));
                            colorStr.erase(colorStr.find_last_not_of(" \t") + 1);

                            if (capaActual != nullptr) {
                                capaActual->pixeles->insert(f, c, colorStr);
                            }
                        } catch (...) {
                            cout << "Error al procesar pixel: " << datosPixel << endl;
                        }
                    }
                }
            }

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
            if (linea.empty() || linea == "\r" || linea == "\n") continue;

            // Encontrar las llaves
            size_t posLlaveAbre = linea.find('{');
            size_t posLlaveCierra = linea.find('}');

            if (posLlaveAbre == string::npos || posLlaveCierra == string::npos) continue;

            // Extraer el ID de la imagen (
            string idImagenStr = linea.substr(0, posLlaveAbre);
            // Extraer los IDs de las capas 
            string capasStr = linea.substr(posLlaveAbre + 1, posLlaveCierra - posLlaveAbre - 1);

            try {
                int idImagen = stoi(idImagenStr);
                Imagen* nuevaImagen = new Imagen(idImagen);

                // Separar los IDs de las capas por coma
                if (!capasStr.empty()) {
                    stringstream ss(capasStr);
                    string idCapaStr;
                    while (getline(ss, idCapaStr, ',')) {
                        int idCapa = stoi(idCapaStr);
                        
                        // Buscar el puntero de la capa real en nuestro árbol
                        Capa* capaEncontrada = arbolCapas->search(idCapa);
                        if (capaEncontrada) {
                            nuevaImagen->agregarCapa(capaEncontrada);
                        } else {
                            cout << "   [!] Advertencia: Capa " << idCapa 
                                 << " no existe. No se agrego a la imagen " << idImagen << endl;
                        }
                    }
                }
                
                // Insertar la imagen ensamblada a la lista circular
                listaImagenes->insertar(nuevaImagen);
                
            } catch (...) {
                cout << "Error procesando linea: " << linea << endl;
            }
   
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
            // Ignorar líneas vacías
            if (linea.empty() || linea == "\r" || linea == "\n") continue;

            // Encontrar la posición de los dos puntos
            size_t posDosPuntos = linea.find(':');
            if (posDosPuntos == string::npos) continue; // Si no hay ':', ignorar línea

            // Extraer el nombre de usuario
            string nombreUsuario = linea.substr(0, posDosPuntos);
            Usuario* nuevoUsuario = new Usuario(nombreUsuario);

            // Extraer lo que hay después de los ':' (las imágenes y el ';')
            string resto = linea.substr(posDosPuntos + 1);
            
            // Quitar el ';' del final
            size_t posPuntoComa = resto.find(';');
            if (posPuntoComa != string::npos) {
                resto = resto.substr(0, posPuntoComa);
            }

            // Si quedaron números (hay imágenes), las separamos por coma
            if (!resto.empty()) {
                stringstream ss(resto);
                string idImgStr;
                while (getline(ss, idImgStr, ',')) {
                    try {
                        int idImg = stoi(idImgStr);
                        nuevoUsuario->agregarImagen(idImg);
                    } catch (...) {
                        // Ignorar si hay un error al convertir a número
                    }
                }
            }

            // Insertar el usuario completo al ABB
            arbolUsuarios->insert(nuevoUsuario);

        }
        archivo.close();
        cout << "Carga masiva de usuarios finalizada." << endl;
    }
};

#endif
