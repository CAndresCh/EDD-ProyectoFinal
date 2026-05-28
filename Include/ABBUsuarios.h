#ifndef ABB_USUARIOS_H
#define ABB_USUARIOS_H

#include <iostream>
#include <fstream>
#include <string>
#include "Usuario.h"

using namespace std;

struct NodoABBUsuario {
    Usuario* usuario;
    NodoABBUsuario* left;
    NodoABBUsuario* right;

    NodoABBUsuario(Usuario* u) {
        usuario = u;
        left = right = nullptr;
    }
};

class ABBUsuarios {
private:
    NodoABBUsuario* root;

    NodoABBUsuario* insert(NodoABBUsuario* node, Usuario* nuevoUsuario) {
        if (node == nullptr) {
            return new NodoABBUsuario(nuevoUsuario);
        }
        if (nuevoUsuario->nombre < node->usuario->nombre) {
            node->left = insert(node->left, nuevoUsuario);
        } else if (nuevoUsuario->nombre > node->usuario->nombre) {
            node->right = insert(node->right, nuevoUsuario);
        }
        return node;
    }

    NodoABBUsuario* search(NodoABBUsuario* node, string nombreBuscado) {
        if (node == nullptr || node->usuario->nombre == nombreBuscado) {
            return node;
        }
        if (nombreBuscado < node->usuario->nombre) {
            return search(node->left, nombreBuscado);
        }
        return search(node->right, nombreBuscado);
    }

    void inorden(NodoABBUsuario* node) {
        if (node == nullptr) return;
        inorden(node->left);
        cout << "- " << node->usuario->nombre << endl;
        inorden(node->right);
    }

    // Método recursivo para escribir los nodos y sus conexiones en formato Graphviz
    void escribirNodosDot(ofstream& archivo, NodoABBUsuario* node) {
        if (node == nullptr) return;

        string idSeguro = node->usuario->nombre;

        // Declarar el nodo actual encerrado en comillas por si tiene guiones o caracteres especiales
        archivo << "    \"node_" << idSeguro << "\" [label=\"{ " << node->usuario->nombre << " }\"];" << endl;

        // Conexión con el hijo izquierdo
        if (node->left != nullptr) {
            string idLeft = node->left->usuario->nombre;
            archivo << "    \"node_" << idSeguro << "\" -> \"node_" << idLeft << "\";" << endl;
            escribirNodosDot(archivo, node->left);
        }
        
        // Conexión con el hijo derecho
        if (node->right != nullptr) {
            string idRight = node->right->usuario->nombre;
            archivo << "    \"node_" << idSeguro << "\" -> \"node_" << idRight << "\";" << endl;
            escribirNodosDot(archivo, node->right);
        }
    }

    NodoABBUsuario* minValueNode(NodoABBUsuario* node) {
        NodoABBUsuario* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    NodoABBUsuario* remove(NodoABBUsuario* node, string nombre, bool& eliminado) {
        if (node == nullptr) return node;

        if (nombre < node->usuario->nombre) {
            node->left = remove(node->left, nombre, eliminado);
        } else if (nombre > node->usuario->nombre) {
            node->right = remove(node->right, nombre, eliminado);
        } else {
            eliminado = true;
            
            // Caso 1 y 2: Sin hijos o con un solo hijo
            if (node->left == nullptr) {
                NodoABBUsuario* temp = node->right;
                delete node->usuario; 
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                NodoABBUsuario* temp = node->left;
                delete node->usuario;
                delete node;
                return temp;
            }

            // Caso 3: Nodo con dos hijos
            NodoABBUsuario* temp = minValueNode(node->right);
            
            Usuario* uTemp = node->usuario;
            node->usuario = temp->usuario;
            temp->usuario = uTemp;

            node->right = remove(node->right, temp->usuario->nombre, eliminado);
        }
        return node;
    }

public:
    ABBUsuarios() {
        root = nullptr;
    }

    void insert(Usuario* nuevoUsuario) {
        root = insert(root, nuevoUsuario);
        cout << "Usuario " << nuevoUsuario->nombre << " insertado en el ABB." << endl;
    }

    Usuario* search(string nombreBuscado) {
        NodoABBUsuario* resultado = search(root, nombreBuscado);
        if (resultado) return resultado->usuario;
        return nullptr; // No se encontró
    }

    void mostrarInorden() {
        cout << "--- Directorio de Usuarios ---" << endl;
        inorden(root);
        cout << "------------------------------" << endl;
    }

    // Método principal para generar la imagen del ABB
    void graficarArbolUsuarios() {
        ofstream archivo("reporte_usuarios.dot");
        if (!archivo.is_open()) {
            cout << "Error al crear el archivo .dot" << endl;
            return;
        }

        // Encabezado del archivo Graphviz
        archivo << "digraph ArbolUsuarios {" << endl;
        archivo << "    rankdir=TB;" << endl; // Top to Bottom
        archivo << "    node [shape=record, style=filled, fillcolor=lightblue];" << endl;
        
        // Llamada al recorrido recursivo si el árbol no está vacío
        if (root != nullptr) {
            escribirNodosDot(archivo, root);
        } else {
            archivo << "    vacio [label=\"Arbol de Usuarios Vacio\"];" << endl;
        }

        // Cierre del archivo
        archivo << "}" << endl;
        archivo.close();

        // Ejecutar Graphviz desde la consola de C++
        system("dot -Tpng reporte_usuarios.dot -o reporte_usuarios.png");
        cout << ">> Reporte generado exitosamente: reporte_usuarios.png" << endl;
    }

    bool eliminarUsuario(string nombre) {
        bool eliminado = false;
        root = remove(root, nombre, eliminado);
        return eliminado;
    }
};

#endif
