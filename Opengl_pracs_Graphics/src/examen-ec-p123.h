// Nombre: Pablo, Apellidos: Martín Palomino, Titulación: GIM
// email: pablomarpa@correo.ugr.es, DNI o pasaporte: 75930976H
#ifndef EXAMEN_EC_P123_HPP
#define EXAMEN_EC_P123_HPP
#include "malla-ind.h"
#include "grafo-escena.h"

class Ejer1 : public MallaInd
{
   public:
        Ejer1();
};

class Ejer2 : public MallaInd
{
    public:
        Ejer2(int n);
};

class Ejer3 : public NodoGrafoEscena
{
    protected:
        const unsigned NUM_PARAMS=2;
        glm::mat4 * rot_cuboarriba=nullptr;
        glm::mat4 * escalado_cub_abajo=nullptr;
        glm::mat4 * traslacion=nullptr;
        glm::mat4 * counterescalado=nullptr;
    public:
        Ejer3(float w,float h);
        float altura;
        unsigned leerNumParametros() const;
        virtual void actualizarEstadoParametro( const unsigned iParam, const float tSec );
};

#endif
