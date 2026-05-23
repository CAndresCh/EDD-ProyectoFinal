#ifndef MATRIZ_DISPERSA_H
#define MATRIZ_DISPERSA_H

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

struct NodoMatriz {
    int row;
    int col;
    string color; 
    NodoMatriz* right;
    NodoMatriz* down;

    NodoMatriz(int r, int c, string v) : row(r), col(c), color(v), right(nullptr), down(nullptr) {}
};

struct Header {
    int index;
    Header* next;
    NodoMatriz* firstNode;

    Header(int idx) : index(idx), next(nullptr), firstNode(nullptr) {}
};

class MatrizDispersa {
private:
    Header* rowHeaderRoot;
    Header* colHeaderRoot;

    Header* getOrCreateHeader(Header* &root, int idx) {
        if (!root || idx < root->index) {
            Header* newHeader = new Header(idx);
            newHeader->next = root;
            root = newHeader;
            return root;
        }

        Header* aux = root;
        while(aux->next && aux->next->index <= idx) {
            aux = aux->next;
        }

        if(aux->index == idx) return aux;

        Header *newHeader = new Header(idx);
        newHeader->next = aux->next;
        aux->next = newHeader;
        return newHeader;
    }

public:
    MatrizDispersa() : rowHeaderRoot(nullptr), colHeaderRoot(nullptr) {}

    void insert(int r, int c, string color) {
        // Si el color viene vacío o es un indicador de borrado, lo eliminamos
        if (color == "" || color == "0") {
            remove(r, c);
            return;
        }

        Header* rh = getOrCreateHeader(rowHeaderRoot, r);
        Header* ch = getOrCreateHeader(colHeaderRoot, c);

        NodoMatriz* existing = search(r, c);
        if (existing) {
            existing->color = color;
            return;
        }
        
        NodoMatriz* newNode = new NodoMatriz(r, c, color);
        
        // Enlazar en Fila
        if(!rh->firstNode || rh->firstNode->col > c) {
            newNode->right = rh->firstNode;
            rh->firstNode = newNode;
        } else {
            NodoMatriz* temp = rh->firstNode;
            while (temp->right && temp->right->col < c) { 
                temp = temp->right;
            }
            newNode->right = temp->right;
            temp->right = newNode;
        }
        
        // Enlazar en Columna
        if(!ch->firstNode || ch->firstNode->row > r) {
            newNode->down = ch->firstNode;
            ch->firstNode = newNode;
        } else {
            NodoMatriz* temp = ch->firstNode;
            while (temp->down && temp->down->row < r) {
                temp = temp->down;
            }
            newNode->down = temp->down;
            temp->down = newNode;
        }
        cout << "Pixel insertado [" << r << "][" << c << "] = " << color << endl;
    } 

    NodoMatriz* search(int r, int c) {
        Header* currRow = rowHeaderRoot;
        while (currRow && currRow->index < r) currRow = currRow->next;
        if (currRow && currRow->index == r) {
            NodoMatriz* temp = currRow->firstNode;
            while (temp && temp->col < c) temp = temp->right;
            if (temp && temp->col == c) return temp;
        }
        return nullptr;
    }

    void remove(int r, int c) {
        Header* rh = rowHeaderRoot;
        while (rh && rh->index < r) rh = rh->next;
        Header* ch = colHeaderRoot;
        while (ch && ch->index < c) ch = ch->next;

        if (!rh || !ch) return;

        NodoMatriz* target = nullptr;
        NodoMatriz* prev = nullptr;
        NodoMatriz* curr = rh->firstNode;

        while (curr && curr->col < c) {
            prev = curr;
            curr = curr->right;
        }
        if (!curr || curr->col != c) return;

        target = curr;
        if (!prev) {
            rh->firstNode = target->right;
        } else {
            prev->right = target->right;
        }

        prev = nullptr;
        curr = ch->firstNode;

        while (curr && curr->row < r) {
            prev = curr;
            curr = curr->down;
        }
        if (!prev) {
            ch->firstNode = target->down;
        } else {
            prev->down = target->down;
        }
        delete target;
        cout << "Pixel eliminado: [" << r << "][" << c << "]" << endl;
    }
};

#endif