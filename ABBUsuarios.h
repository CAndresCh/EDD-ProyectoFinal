#ifndef ABB_USUARIOS_H
#define ABB_USUARIOS_H

#include <iostream>
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
};

#endif

