// Nombre: Pablo, Apellidos: Martín Palomino, Titulación: GIM
// email: pablomarpa@correo.ugr.es, DNI o pasaporte: 75930976H
#ifndef MODELO_JER_HPP
#define MODELO_JER_HPP
#include "grafo-escena.h"
#include "malla-ind.h"
#include "malla-revol.h"
using namespace std;
using namespace glm;

class Cabeza: public MallaRevol{
    public:
        Cabeza(const int num_verts_per,const unsigned nperfiles);
};

class CilindroCuerpo: public MallaRevol{
    public:
        CilindroCuerpo(const int num_verts_per,const unsigned nperfiles);
};

class SemiEsfera : public MallaRevol
{
   public:
      SemiEsfera(const int num_verts_per,const unsigned nperfiles);
};


class Mochila : public NodoGrafoEscena
{
   public:
      Mochila();
};

class BaseCuerpo: public NodoGrafoEscena{
    public:
        //unsigned leerNumParametros() const;
        //virtual void actualizarEstadoParametro( const unsigned iParam, const float tSec );
        BaseCuerpo(); 
};

class BasePierna: public NodoGrafoEscena{
    public:
        //unsigned leerNumParametros() const;
        //virtual void actualizarEstadoParametro( const unsigned iParam, const float tSec );
        BasePierna(); 
};

class BaseGafas: public NodoGrafoEscena{
    public:
        //unsigned leerNumParametros() const;
        //virtual void actualizarEstadoParametro( const unsigned iParam, const float tSec );
        BaseGafas(); 
};

class BaseAnteBrazo: public NodoGrafoEscena{
    public:
        //unsigned leerNumParametros() const;
        //virtual void actualizarEstadoParametro( const unsigned iParam, const float tSec );
        BaseAnteBrazo(); 
};

class CuerpoAmongus: public NodoGrafoEscena{
    protected:
        const unsigned NUM_PARAMS=5;
        mat4 * mov_pierna1=nullptr;
        mat4 * mov_pierna2=nullptr;
        mat4 * mov_mochila=nullptr;
        mat4 * sc_mochila=nullptr;
        mat4 * mov_cuerpoeEntero=nullptr;
        mat4 * mov_articula=nullptr;
        mat4 * tras_brazo=nullptr;
    public:
        unsigned leerNumParametros() const;
        virtual void actualizarEstadoParametro( const unsigned iParam, const float tSec );
        CuerpoAmongus(); 
};


#endif