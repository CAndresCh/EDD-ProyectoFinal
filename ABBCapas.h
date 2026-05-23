#ifndef ABB_CAPAS_H
#define ABB_CAPAS_H

#include <iostream>
#include "Capa.h"

using namespace std;

struct NodoABB {
    Capa* capa;
    NodoABB* left;
    NodoABB* right;

    NodoABB(Capa* c) {
        capa = c;
        right = left = nullptr;
    }
};

class ABBCapas {
private:
    NodoABB* root;

    NodoABB* insert(NodoABB* node, Capa* nuevaCapa) {
        if (node == nullptr) {
            return new NodoABB(nuevaCapa);
        }
        if (nuevaCapa->idCapa < node->capa->idCapa)
            node->left = insert(node->left, nuevaCapa);
        if (nuevaCapa->idCapa > node->capa->idCapa)
            node->right = insert(node->right, nuevaCapa);
        return node;
    }

    NodoABB* search(NodoABB* node, int idBuscado) {
        if (node == nullptr || node->capa->idCapa == idBuscado)
            return node;
        
        if (idBuscado < node->capa->idCapa)
            return search(node->left, idBuscado);
        
        return search(node->right, idBuscado);
    }

    void inorden(NodoABB* node) {
        if (node == nullptr) return;
        inorden(node->left);
        cout << "Capa ID: " << node->capa->idCapa << " " << endl;
        inorden(node->right);
    }

public:
    ABBCapas() {
        root = nullptr;
    }

    void insert(Capa* nuevaCapa) {
        root = insert(root, nuevaCapa);
        cout << "Capa " << nuevaCapa->idCapa << " insertada en el ABB." << endl;
    }

    Capa* search(int idBuscado) {
        NodoABB* resultado = search(root, idBuscado);
        if (resultado) return resultado->capa;
        return nullptr;
    }

    void inorden() {
        cout << "--- Listado de Capas (Inorden) ---" << endl;
        inorden(root);
        cout << "----------------------------------" << endl;
    }
};

#endif