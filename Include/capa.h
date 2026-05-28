#ifndef CAPA_H
#define CAPA_H

#include "MatrizDispersa.h"

class Capa {
public:
    int idCapa;
    MatrizDispersa* pixeles;

    Capa(int id) {
        idCapa = id;
        pixeles = new MatrizDispersa();
    }
};

#endif