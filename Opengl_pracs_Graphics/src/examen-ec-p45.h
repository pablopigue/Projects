// Nombre: Pablo, Apellidos: Martín Palomino, Titulación: GIM
// email: pablomarpa@correo.ugr.es, DNI o pasaporte: 75930976H
#ifndef EXAMEN_EC_P45_HPP
#define EXAMEN_EC_P45_HPP
#include "malla-ind.h"
#include "grafo-escena.h"
class Piramide : public MallaInd
{
   public:
      Piramide();
};

class NodoPiramide: public NodoGrafoEscena{  
   public:
      NodoPiramide();
};

class Triangulo : public MallaInd
{
   public:
      Triangulo(glm::vec3 v1,glm::vec3 v2, glm::vec3 v3);

};

class MiTriangulo: public NodoGrafoEscena{  
   public:
      MiTriangulo(glm::vec3 v1,glm::vec3 v2, glm::vec3 v3,int n);
      bool cuandoClick(const glm::vec3 & centro_wc) override;
   protected:
      int pos=0;
};

class grafoexp5: public NodoGrafoEscena{
    public:
        grafoexp5();
};

#endif