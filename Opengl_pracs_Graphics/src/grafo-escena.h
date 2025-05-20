// Nombre: Pablo, Apellidos: Martín Palomino, Titulación: GIM
// email: pablomarpa@correo.ugr.es, DNI o pasaporte: 75930976H
// *********************************************************************
// **
// ** Asignatura: INFORMÁTICA GRÁFICA
// ** 
// ** Gestión de grafos de escena (declaraciones).  Clase 'NodoGrafoEscena' y relacionadas.
// ** Copyright (C) 2016-2024 Carlos Ureña
// **
// ** Declaraciones de: 
// **     + Clase 'NodoGrafoEscena' (derivada de 'Objeto3D')
// **     + Clase 'EntradaNGE' (una entrada de un nodo del grafo de escena)
// **     + Tipo enumerado 'TipoEntNGE' (tipo de entradas del nodo del grafo de escena)
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************

#ifndef GRAFO_ESCENA_HPP
#define GRAFO_ESCENA_HPP

//#include <tup_mat.h>   

#include "objeto3d.h"
#include "malla-ind.h" // para poder usar clase MallaInd
#include "materiales-luces.h"
#include "malla-revol.h"

//using namespace tup_mat ;

// *********************************************************************
// declaración adelantada de estructura para un nodo del grafo de escena

class NodoGrafoEscena ;

// *********************************************************************
// tipo enumerado con los tipos de entradas del nodo del grafo de escena

enum class TipoEntNGE { objeto, transformacion, material, noInicializado } ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

struct EntradaNGE
{
   TipoEntNGE tipo = TipoEntNGE::noInicializado ;   // objeto, transformacion, material
   union
   {  Objeto3D  * objeto = nullptr ;  // ptr. a un objeto (no propietario)
      glm::mat4 * matriz   ;  // ptr. a matriz 4x4 transf. (propietario)
      Material  * material ; // ptr. a material (no propietario)
   } ;
   // constructores (uno por tipo)
   EntradaNGE() = delete ; // prohibe constructor sin parámetros
   EntradaNGE( Objeto3D        * pObjeto   );      // (copia solo puntero)
   EntradaNGE( const glm::mat4 & pMatriz   ); // (crea copia en el heap)
   EntradaNGE( Material        * pMaterial );    // (copia solo puntero)
   ~EntradaNGE() ;
} ;

// *********************************************************************
// Nodo del grafo de escena: es un objeto 3D parametrizado, que contiene una lista de entradas

class NodoGrafoEscena : public Objeto3D
{
   protected:
   // COMPLETAR: práctica 3: definir variables y métodos privados del nodo
   // .......
   std::vector<EntradaNGE> entradas;
   bool centro_calculado =false;

   public:

   NodoGrafoEscena() ;

   // visualiza usando OpenGL
   virtual void visualizarGL(  ) ;

   // visualizar pura y simplemente la geometría, sin colores, normales, coord. text. etc...
   // (se supone que el estado de OpenGL está fijado antes de esta llamada de alguna forma adecuada)
   virtual void visualizarGeomGL(  ) ;

   // visualizar las normales de los objetos del nodo
   virtual void visualizarNormalesGL () ;

   // visualizar el objeto en 'modo seleccion', es decir, sin iluminación y con los colores 
   // basados en los identificadores de los objetos
   virtual void visualizarModoSeleccionGL()  ;

   // añadir una entrada al final, hace copia de la entrada
   // devuelve indice de la entrada dentro del vector de entradas
   unsigned agregar( const EntradaNGE & entrada );

   // construir una entrada y añadirla (al final)
   unsigned agregar( Objeto3D *        pObjeto ); // objeto (copia solo puntero)
   unsigned agregar( const glm::mat4 & pMatriz ); // matriz (copia objeto)
   unsigned agregar( Material *        pMaterial ); // material (copia solo puntero)

   // devuelve el puntero a la matriz en la i-ésima entrada
   glm::mat4 * leerPtrMatriz( unsigned iEnt );

   // método para buscar un objeto con un identificador
   virtual bool buscarObjeto( const int ident_busc, const glm::mat4 & mmodelado,
                    Objeto3D ** objeto, glm::vec3 & centro_wc )  ;

   // si 'centro_calculado' es 'false', recalcula el centro usando los centros
   // de los hijos (el punto medio de la caja englobante de los centros de hijos)
   virtual void calcularCentroOC() ;


} ;

// *********************************************************************

class GrafoEstrellaX: public NodoGrafoEscena{
   protected:
      glm::mat4 *mov_estrella=nullptr;
   
   public:
      GrafoEstrellaX(unsigned n);
      unsigned leerNumParametros() const;
      virtual void actualizarEstadoParametro( const unsigned iParam, const float tSec );
};

class GrafoCubos: public NodoGrafoEscena{
   protected:
      glm::mat4 *mov_cubos=nullptr;
   
   public:
      GrafoCubos();
      unsigned leerNumParametros() const;
      virtual void actualizarEstadoParametro( const unsigned iParam, const float tSec );
};

class GrafoFlechas: public NodoGrafoEscena{  
   public:
      GrafoFlechas();
};

class CubosConPalos: public NodoGrafoEscena{  
   protected:
      glm::mat4 *mov_cubos=nullptr;
      glm::mat4 *mov_palo=nullptr;
      glm::mat4 *mov_cubopalo=nullptr;
   public:
      CubosConPalos();
      unsigned leerNumParametros() const;
      virtual void actualizarEstadoParametro( const unsigned iParam, const float tSec );
};

class Practicando3: public NodoGrafoEscena{  
   protected:
      glm::mat4 *escalado_cubopeq=nullptr;
      glm::mat4 *rot_cubo=nullptr;
      glm::mat4 *counter_escalado=nullptr;
      float altura;
   public:
      Practicando3(int h,int a);
      unsigned leerNumParametros() const;
      virtual void actualizarEstadoParametro( const unsigned iParam, const float tSec );
};

class Practicando4: public NodoGrafoEscena{  
   protected:
      glm::mat4 *traslacionpelota=nullptr;
      glm::mat4 *rotaciontapa=nullptr;
   public:
      Practicando4();
      unsigned leerNumParametros() const;
      virtual void actualizarEstadoParametro( const unsigned iParam, const float tSec );
};

class NodoCubo24: public NodoGrafoEscena{  
   public:
      NodoCubo24();
};

class NodoDiscoP4: public NodoGrafoEscena{  
   public:
      NodoDiscoP4();
};

class NodoDiscoP42: public NodoGrafoEscena{  
   public:
      NodoDiscoP42();
};

class BethovenP4: public NodoGrafoEscena{  
   public:
      BethovenP4(int n);
};

class MiEsferaE1: public NodoGrafoEscena{  
   public:
      MiEsferaE1(int i,int j);
      bool cuandoClick(const glm::vec3 & centro_wc) override;
   protected:
      int fil=0;
      int col=0;
};

class MiEsferaE2: public NodoGrafoEscena{  
   public:
      MiEsferaE2(int i,int j);
      bool cuandoClick(const glm::vec3 & centro_wc) override;
   protected:
      int fil=0;
      int col=0;
};

class GrafoEsferasP5: public NodoGrafoEscena{  
   public:
      GrafoEsferasP5();
};

class GrafoEsferasP52: public NodoGrafoEscena{  
   public:
      GrafoEsferasP52();
};
#endif // GRAFO_ESCENA_HPP


