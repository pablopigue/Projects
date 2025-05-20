// Nombre: Pablo, Apellidos: Martín Palomino, Titulación: GIM
// email: pablomarpa@correo.ugr.es, DNI o pasaporte: 75930976H
#ifndef LATAPEONES_H
#define LATAPEONES_H

#include "grafo-escena.h"


class LataPeones : public NodoGrafoEscena {
    public:
        LataPeones();
};

class Lata : public NodoGrafoEscena {
    public:
        Lata(const std::string & nombre_arch);
};

class Peon : public NodoGrafoEscena {
    public:
        Peon(int nperfiles);
        bool cuandoClick(const glm::vec3 & centro_wc) override;

    protected:
        glm::mat4* mat_tras = nullptr;
        int aux=1;
};


class VariasLatasPeones : public NodoGrafoEscena {
    public:
        VariasLatasPeones();
};

#endif