#ifndef ABB_CAPAS_H
#define ABB_CAPAS_H

#include <iostream>
#include <fstream>
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

    // Método recursivo privado para armar el .dot
    void escribirNodosDot(ofstream& archivo, NodoABB* node) {
        if (node == nullptr) return;

        // Usamos puertos <C0> (izquierdo) y <C1> (derecho) para que se grafique como un árbol binario real
        archivo << "    node_" << node->capa->idCapa << " [label=\"<C0>|Capa " << node->capa->idCapa << "|<C1>\"];" << endl;

        if (node->left != nullptr) {
            // La flecha sale desde el puerto izquierdo :C0
            archivo << "    node_" << node->capa->idCapa << ":C0 -> node_" << node->left->capa->idCapa << ";" << endl;
            escribirNodosDot(archivo, node->left);
        }
        
        if (node->right != nullptr) {
            // La flecha sale desde el puerto derecho :C1
            archivo << "    node_" << node->capa->idCapa << ":C1 -> node_" << node->right->capa->idCapa << ";" << endl;
            escribirNodosDot(archivo, node->right);
        }
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

    void graficarArbolCapas() {
        ofstream archivo("reporte_capas.dot");
        if (!archivo.is_open()) {
            cout << "Error al crear el archivo .dot de capas" << endl;
            return;
        }

        archivo << "digraph ArbolCapas {" << endl;
        archivo << "    rankdir=TB;" << endl; 
        archivo << "    node [shape=record, style=filled, fillcolor=lightyellow];" << endl;
        
        if (root != nullptr) {
            escribirNodosDot(archivo, root);
        } else {
            archivo << "    vacio [label=\"Arbol de Capas Vacio\"];" << endl;
        }

        archivo << "}" << endl;
        archivo.close();

        system("dot -Tpng reporte_capas.dot -o reporte_capas.png");
        cout << ">> Reporte generado exitosamente: reporte_capas.png" << endl;
    }
};

#endif