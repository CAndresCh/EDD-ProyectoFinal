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

    void graficarMatriz(int idCapa) {
        ofstream archivo("reporte_capa_" + to_string(idCapa) + ".dot");
        if (!archivo.is_open()) {
            cout << "Error al crear el archivo .dot de la capa." << endl;
            return;
        }

        archivo << "digraph MatrizCapa {" << endl;
        archivo << "    node [shape=box, style=filled, fillcolor=white];" << endl;
        archivo << "    rankdir=LR;" << endl; // De izquierda a derecha por defecto

        // 1. Nodo principal de la matriz
        archivo << "    M0 [label=\"Capa " << idCapa << "\", fillcolor=black, fontcolor=white];" << endl;

        // 2. Graficar Cabeceras de Columnas (Horizontales)
        Header* auxCol = colHeaderRoot;
        if (auxCol) {
            archivo << "    M0 -> C" << auxCol->index << ";" << endl;
            archivo << "    { rank=same; M0; ";
            Header* temp = auxCol;
            while (temp) {
                archivo << "C" << temp->index << "; ";
                temp = temp->next;
            }
            archivo << "}" << endl; // Cerramos el rank=same
            
            while (auxCol) {
                archivo << "    C" << auxCol->index << " [label=\"C" << auxCol->index << "\", fillcolor=lightgray];" << endl;
                if (auxCol->next) {
                    archivo << "    C" << auxCol->index << " -> C" << auxCol->next->index << ";" << endl;
                    archivo << "    C" << auxCol->next->index << " -> C" << auxCol->index << ";" << endl; // Doble enlace visual
                }
                auxCol = auxCol->next;
            }
        }

        // 3. Graficar Cabeceras de Filas (Verticales) y Nodos Internos
        Header* auxFila = rowHeaderRoot;
        if (auxFila) {
            archivo << "    M0 -> F" << auxFila->index << ";" << endl;
            
            while (auxFila) {
                archivo << "    F" << auxFila->index << " [label=\"F" << auxFila->index << "\", fillcolor=lightgray];" << endl;
                if (auxFila->next) {
                    archivo << "    F" << auxFila->index << " -> F" << auxFila->next->index << ";" << endl;
                    archivo << "    F" << auxFila->next->index << " -> F" << auxFila->index << ";" << endl;
                }

                // Nodos de esta fila
                NodoMatriz* nodoActual = auxFila->firstNode;
                if (nodoActual) {
                    archivo << "    F" << auxFila->index << " -> N_" << nodoActual->row << "_" << nodoActual->col << ";" << endl;
                    archivo << "    N_" << nodoActual->row << "_" << nodoActual->col << " -> F" << auxFila->index << ";" << endl;

                    // Agrupar la fila en la misma altura (rank=same)
                    archivo << "    { rank=same; F" << auxFila->index << "; ";
                    NodoMatriz* tempRank = nodoActual;
                    while (tempRank) {
                        archivo << "N_" << tempRank->row << "_" << tempRank->col << "; ";
                        tempRank = tempRank->right;
                    }
                    archivo << "}" << endl;

                    while (nodoActual) {
                        // El nodo muestra su color hexadecimal y se pinta de ese color
                        archivo << "    N_" << nodoActual->row << "_" << nodoActual->col 
                                << " [label=\"" << nodoActual->color 
                                << "\", fillcolor=\"" << nodoActual->color << "\", fontcolor=white];" << endl;

                        // Enlaces a la derecha
                        if (nodoActual->right) {
                            archivo << "    N_" << nodoActual->row << "_" << nodoActual->col << " -> N_" << nodoActual->right->row << "_" << nodoActual->right->col << ";" << endl;
                            archivo << "    N_" << nodoActual->right->row << "_" << nodoActual->right->col << " -> N_" << nodoActual->row << "_" << nodoActual->col << ";" << endl;
                        }
                        nodoActual = nodoActual->right;
                    }
                }
                auxFila = auxFila->next;
            }
        }

        // 4. Enlaces hacia abajo (Columnas)
        auxCol = colHeaderRoot;
        while (auxCol) {
            NodoMatriz* nodoActual = auxCol->firstNode;
            if (nodoActual) {
                archivo << "    C" << auxCol->index << " -> N_" << nodoActual->row << "_" << nodoActual->col << ";" << endl;
                archivo << "    N_" << nodoActual->row << "_" << nodoActual->col << " -> C" << auxCol->index << ";" << endl;
                
                while (nodoActual) {
                    if (nodoActual->down) {
                        archivo << "    N_" << nodoActual->row << "_" << nodoActual->col << " -> N_" << nodoActual->down->row << "_" << nodoActual->down->col << ";" << endl;
                        archivo << "    N_" << nodoActual->down->row << "_" << nodoActual->down->col << " -> N_" << nodoActual->row << "_" << nodoActual->col << ";" << endl;
                    }
                    nodoActual = nodoActual->down;
                }
            }
            auxCol = auxCol->next;
        }

        archivo << "}" << endl;
        archivo.close();

        // Compilar usando system
        string comando = "dot -Tpng reporte_capa_" + to_string(idCapa) + ".dot -o reporte_capa_" + to_string(idCapa) + ".png";
        system(comando.c_str());
        
        cout << ">> Reporte de capa generado exitosamente: reporte_capa_" << idCapa << ".png" << endl;
    }
};

#endif