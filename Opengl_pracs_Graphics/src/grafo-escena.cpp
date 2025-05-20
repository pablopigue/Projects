// Nombre: Pablo, Apellidos: Martín Palomino, Titulación: GIM
// email: pablomarpa@correo.ugr.es, DNI o pasaporte: 75930976H
// *********************************************************************
// **
// ** Asignatura: INFORMÁTICA GRÁFICA
// **
// ** Gestión de grafos de escena (implementación). Clase 'NodoGrafoEscena' y relacionadas.
// ** Copyright (C) 2016-2024 Carlos Ureña
// **
// ** Implementación de:
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

#include "ig-aux.h"
#include "grafo-escena.h"
#include "aplicacion-ig.h"
#include "seleccion.h" // para 'ColorDesdeIdent'

// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE(Objeto3D *pObjeto)
{
   assert(pObjeto != nullptr);
   tipo = TipoEntNGE::objeto;
   objeto = pObjeto;
}
// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE(const glm::mat4 &pMatriz)
{
   tipo = TipoEntNGE::transformacion;
   matriz = new glm::mat4(); // matriz en el heap, puntero propietario
   *matriz = pMatriz;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE(Material *pMaterial)
{
   assert(pMaterial != nullptr);
   tipo = TipoEntNGE::material;
   material = pMaterial;
}

// -----------------------------------------------------------------------------
// Destructor de una entrada

EntradaNGE::~EntradaNGE()
{
   /**  no fnciona debido a que se hacen copias (duplicados) de punteros
   if ( tipo == TipoEntNGE::transformacion )
   {
      assert( matriz != NULL );
      delete matriz ;
      matriz = NULL ;
   }
   * **/
}

// *****************************************************************************
// Nodo del grafo de escena: contiene una lista de entradas
// *****************************************************************************

NodoGrafoEscena::NodoGrafoEscena()
{
}

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL()
{
   using namespace std;
   assert(aplicacionIG != nullptr);

   // cout << "Entra " << __FUNCTION__ << ": " << leerNombre() << endl ;

   // comprobar que hay un cauce y una pila de materiales y recuperarlos.
   Cauce *cauce = aplicacionIG->cauce;
   assert(cauce != nullptr);
   PilaMateriales *pila_materiales = aplicacionIG->pila_materiales;
   assert(pila_materiales != nullptr);

   // COMPLETAR: práctica 3: implementar la visualización del nodo
   //
   // Se deben de recorrer las entradas y llamar recursivamente de visualizarGL, pero
   // teniendo en cuenta que, al igual que el método visualizarGL de las mallas indexadas,
   // si el nodo tiene un color, debemos de cambiar el color del cauce (y hacer push/pop).
   // Además, hay que hacer push/pop de la pila de modelado.
   // Así que debemos de dar estos pasos:
   //
   // 1. Si el objeto tiene un color asignado (se comprueba con 'tieneColor')
   //     - hacer push del color actual del cauce (con 'pushColor') y después
   //     - fijar el color en el cauce (con 'fijarColor'), usando el color del objeto (se lee con 'leerColor()')
   if (tieneColor())
   {
      cauce->pushColor();
      cauce->fijarColor(leerColor());
   }

   if (aplicacionIG->iluminacion)
   {
      pila_materiales->push();
   }
   // 2. Guardar copia de la matriz de modelado (con 'pushMM'),
   cauce->pushMM();
   // 3. Para cada entrada del vector de entradas:
   //     - si la entrada es de tipo objeto: llamar recursivamente a 'visualizarGL'
   //     - si la entrada es de tipo transformación: componer la matriz (con 'compMM')
   int tam = entradas.size();
   for (int i = 0; i < tam; i++)
   {
      if (entradas[i].tipo == TipoEntNGE::objeto)
      {
         entradas[i].objeto->visualizarGL();
      }
      else if (entradas[i].tipo == TipoEntNGE::transformacion)
      {
         cauce->compMM(*(entradas[i].matriz));
      }
      else if (entradas[i].tipo == TipoEntNGE::material and aplicacionIG->iluminacion)
      {
         pila_materiales->activar(entradas[i].material);
      }
   }
   // 4. Restaurar la copia guardada de la matriz de modelado (con 'popMM')
   cauce->popMM();
   // 5. Si el objeto tiene color asignado:
   //     - restaurar el color original a la entrada (con 'popColor')
   if (tieneColor())
   {
      cauce->popColor();
   }

   // COMPLETAR: práctica 4: añadir gestión de los materiales cuando la iluminación está activada
   //
   // Si 'apl->iluminacion' es 'true', se deben de gestionar los materiales:
   //
   //   1. al inicio, hacer 'push' de la pila de materiales (guarda material actual en la pila)
   //   2. si una entrada es de tipo material, activarlo usando a pila de materiales
   //   3. al finalizar, hacer 'pop' de la pila de materiales (restaura el material activo al inicio)

   // ......

   if (aplicacionIG->iluminacion)
   {
      pila_materiales->pop();
   }
}

// *****************************************************************************
// visualizar pura y simplemente la geometría, sin colores, normales, coord. text. etc...

void NodoGrafoEscena::visualizarGeomGL()
{
   using namespace std;
   // comprobar que hay un cauce
   assert(aplicacionIG != nullptr);
   Cauce *cauce = aplicacionIG->cauce;
   assert(cauce != nullptr);

   // COMPLETAR: práctica 3: implementar la visualización del nodo (ignorando colores)
   //
   // Este método hace un recorrido de las entradas del nodo, parecido a 'visualizarGL', pero más simple,
   // Se dan estos pasos:
   //
   // 1. Guardar copia de la matriz de modelado (con 'pushMM'),
   cauce->pushMM();
   // 2. Para cada entrada del vector de entradas:
   //         - Si la entrada es de tipo objeto: llamar recursivamente a 'visualizarGeomGL'.
   //         - Si la entrada es de tipo transformación: componer la matriz (con 'compMM').
   int tam = entradas.size();
   for (int i = 0; i < tam; i++)
   {
      if (entradas[i].tipo == TipoEntNGE::objeto)
      {
         entradas[i].objeto->visualizarGeomGL();
      }
      else if (entradas[i].tipo == TipoEntNGE::transformacion)
      {
         cauce->compMM(*(entradas[i].matriz));
      }
   }
   //   3. Restaurar la copia guardada de la matriz de modelado (con 'popMM')
   cauce->popMM();
   // .......
}

// -----------------------------------------------------------------------------
// Visualizar las normales de los objetos del nodo

void NodoGrafoEscena::visualizarNormalesGL()
{
   using namespace std;

   // comprobar que hay un cauce
   assert(aplicacionIG != nullptr);
   Cauce *cauce = aplicacionIG->cauce;
   assert(cauce != nullptr);

   // COMPLETAR: práctica 4: visualizar las normales del nodo del grafo de escena
   //
   // Este método hace un recorrido de las entradas del nodo, parecido a 'visualizarGL', teniendo
   // en cuenta estos puntos:
   //
   // - usar push/pop de la matriz de modelado al inicio/fin (al igual que en visualizatGL)
   // - recorrer las entradas, llamando recursivamente a 'visualizarNormalesGL' en los nodos u objetos hijos
   // - ignorar el color o identificador del nodo (se supone que el color ya está prefijado antes de la llamada)
   // - ignorar las entradas de tipo material, y la gestión de materiales (se usa sin iluminación)

   // .......
   cauce->pushMM();
   for (int i = 0; i < entradas.size(); i++)
   {
      if (entradas[i].tipo == TipoEntNGE::objeto)
         entradas[i].objeto->visualizarNormalesGL();
      else if (entradas[i].tipo == TipoEntNGE::transformacion)
         cauce->compMM(*entradas[i].matriz);
   }

   cauce->popMM();
}

// -----------------------------------------------------------------------------
// visualizar el objeto en 'modo seleccion', es decir, sin iluminación y con los colores
// basados en los identificadores de los objetos
void NodoGrafoEscena::visualizarModoSeleccionGL()
{
   using namespace std;
   assert(aplicacionIG != nullptr);
   Cauce *cauce = aplicacionIG->cauce;
   assert(cauce != nullptr);

   // COMPLETAR: práctica 5: visualizar este nodo en modo selección.
   //
   // Se debe escribir código para dar estos pasos:
   //
   // 2. Leer identificador (con 'leerIdentificador'), si el identificador no es -1
   //      + Guardar una copia del color actual del cauce (con 'pushColor')
   //      + Fijar el color del cauce de acuerdo al identificador, (usar 'ColorDesdeIdent').
   // 3. Guardar una copia de la matriz de modelado (con 'pushMM')
   // 4. Recorrer la lista de nodos y procesar las entradas transformación o subobjeto:
   //      + Para las entradas subobjeto, invocar recursivamente a 'visualizarModoSeleccionGL'
   //      + Para las entradas transformación, componer la matriz (con 'compMM')
   // 5. Restaurar la matriz de modelado original (con 'popMM')
   // 6. Si el identificador no es -1, restaurar el color previo del cauce (con 'popColor')
   //
   // ........

   int id = leerIdentificador();

   if (id != -1)
   {
      cauce->pushColor();
      cauce->fijarColor(ColorDesdeIdent(id));
   }

   cauce->pushMM();

   for (int i = 0; i < entradas.size(); i++)
   {
      if (entradas[i].tipo == TipoEntNGE::objeto)
         entradas[i].objeto->visualizarModoSeleccionGL();
      else if (entradas[i].tipo == TipoEntNGE::transformacion)
         cauce->compMM(*entradas[i].matriz);
   }

   cauce->popMM();

   if (id != -1)
      cauce->popColor();
}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica (de cualqiuer tipo de entrada)

unsigned NodoGrafoEscena::agregar(const EntradaNGE &entrada)
{
   // COMPLETAR: práctica 3: agregar la entrada al nodo, devolver índice de la entrada agregada
   // ........
   entradas.push_back(entrada);
   return entradas.size() - 1; // sustituir por lo que corresponda ....
}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar(Objeto3D *pObjeto)
{
   return agregar(EntradaNGE(pObjeto));
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar(const glm::mat4 &pMatriz)
{
   return agregar(EntradaNGE(pMatriz));
}
// ---------------------------------------------------------------------
// material (copia solo puntero)
unsigned NodoGrafoEscena::agregar(Material *pMaterial)
{
   return agregar(EntradaNGE(pMaterial));
}

// devuelve el puntero a la matriz en la i-ésima entrada
glm::mat4 *NodoGrafoEscena::leerPtrMatriz(unsigned indice)
{
   // COMPLETAR: práctica 3: leer un puntero a una matriz en una entrada de un nodo
   //
   // Devolver el puntero a la matriz en la entrada indicada por 'indice'.
   // Debe de dar error y abortar si:
   //   - el índice está fuera de rango
   //   - la entrada no es de tipo transformación
   //   - el puntero a la matriz es nulo
   //
   assert(indice < entradas.size());
   assert(entradas[indice].tipo == TipoEntNGE::transformacion);
   assert(entradas[indice].matriz != nullptr);
   return entradas[indice].matriz;
}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC()
{
   using namespace std;
   using namespace glm;

   // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
   //    en coordenadas de objeto (hay que hacerlo recursivamente)
   //   (si el centro ya ha sido calculado, no volver a hacerlo)
   // ........

   if (centro_calculado)
      return;

   int contadorCentros = 0;
   mat4 matrizModelado(1.0f);
   vec3 centroAux = vec3(0.0, 0.0, 0.0);

   for (unsigned int i = 0; i < entradas.size(); i++)
   {
      if (entradas[i].tipo == TipoEntNGE::transformacion)
      {
         matrizModelado = matrizModelado * (*entradas[i].matriz);
      }
      else if (entradas[i].tipo == TipoEntNGE::objeto)
      {
         entradas[i].objeto->calcularCentroOC();
         centroAux = centroAux + vec3(matrizModelado * vec4(entradas[i].objeto->leerCentroOC(), 1.0f));
         contadorCentros++;
      }
   }

   ponerCentroOC(centroAux/float(contadorCentros));
   centro_calculado = true;
}
// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto(
    const int ident_busc,       // identificador a buscar
    const glm::mat4 &mmodelado, // matriz de modelado
    Objeto3D **objeto,          // (salida) puntero al puntero al objeto
    glm::vec3 &centro_wc        // (salida) centro del objeto en coordenadas del mundo
)
{
   using namespace std;
   using namespace glm;

   assert(0 < ident_busc);

   // COMPLETAR: práctica 5: buscar un sub-objeto con un identificador
   // Se deben de dar estos pasos:

   // 1. calcula el centro del objeto, (solo la primera vez)
   // ........

   calcularCentroOC();
   // 2. si el identificador del nodo es el que se busca, ya está (terminar)
   // ........

   if (ident_busc == leerIdentificador())
   {
      *objeto = this;
      centro_wc = leerCentroOC();
      return true;
   }
   // 3. El nodo no es el buscado: buscar recursivamente en los hijos
   //    (si alguna llamada para un sub-árbol lo encuentra, terminar y devolver 'true')
   // ........
   mat4 matrizmod = mmodelado;

   for (int i = 0; i < entradas.size(); i++)
   {
      if (entradas[i].tipo == TipoEntNGE::objeto)
      {
         if (entradas[i].objeto->buscarObjeto(ident_busc, matrizmod, objeto, centro_wc))
            return true;
      }
      else if (entradas[i].tipo == TipoEntNGE::transformacion)
         matrizmod = matrizmod * (*entradas[i].matriz);
   }
   // ni este nodo ni ningún hijo es el buscado: terminar
   return false;
}

GrafoEstrellaX::GrafoEstrellaX(unsigned n)
{
   ponerNombre("Estrella X");

   NodoGrafoEscena *estrella = new NodoGrafoEscena();

   estrella->agregar(glm::scale(glm::vec3(2.6, 2.6, 1)));
   estrella->agregar(glm::translate(glm::vec3(-0.5, -0.5, 0)));

   estrella->agregar(new EstrellaZ(n));

   NodoGrafoEscena *cono = new NodoGrafoEscena();

   cono->agregar(glm::rotate(float(M_PI * 0.5), glm::vec3(0.0, 0.0, 1.0))); // PORQUE SI NO NO ME CUADRAN LOS CONOS
   cono->agregar(glm::translate(glm::vec3(0.0, 1.3, 0.0)));
   cono->agregar(glm::scale(glm::vec3(0.14, 0.15, 0.14)));

   cono->agregar(new Cono(5, 10));

   float grado = 2 * M_PI / n;

   unsigned ind_giro = agregar(glm::rotate(0.0f, glm::vec3(1.0, 0.0, 0.0)));

   for (int i = 0; i < n; i++)
   {
      agregar(glm::rotate(grado, glm::vec3(0.0, 0.0, 1.0)));
      agregar(cono);
   }

   agregar(estrella);

   mov_estrella = leerPtrMatriz(ind_giro);
}

unsigned GrafoEstrellaX::leerNumParametros() const
{
   return 1;
}

void GrafoEstrellaX::actualizarEstadoParametro(const unsigned iParam, const float tSec)
{
   switch (iParam)
   {
   case 0:
      *mov_estrella = glm::rotate(float(2.5 * 2 * M_PI * tSec), glm::vec3(0.0, 0.0, 1.0));
      break;

   default:
      break;
   }
}

GrafoCubos::GrafoCubos()
{
   ponerNombre("GrafoCubos");

   NodoGrafoEscena *cuboAbaseDeRejillas = new NodoGrafoEscena();
   cuboAbaseDeRejillas->agregar(glm::translate(glm::vec3(0.0, 0.0, 0.5)));
   cuboAbaseDeRejillas->agregar(glm::translate(glm::vec3(-0.5, -0.5, 0)));
   cuboAbaseDeRejillas->agregar(new RejillaY(10, 10));

   NodoGrafoEscena *cubopeq = new NodoGrafoEscena();

   unsigned ind_cubo = cubopeq->agregar(glm::rotate(0.0f, glm::vec3(0.0, 0.0, 1.0)));
   cubopeq->agregar(glm::translate(glm::vec3(0.0, 0.0, 0.93)));
   cubopeq->agregar(glm::scale(glm::vec3(0.2, 0.2, 0.4)));
   cubopeq->agregar(new Cubo());

   for (int i = 0; i < 4; i++)
   {
      agregar(glm::rotate(float(M_PI * 0.5), glm::vec3(0.0, 1.0, 0.0)));
      agregar(cuboAbaseDeRejillas);
      agregar(cubopeq);
   }

   agregar(glm::rotate(float(M_PI * 0.5), glm::vec3(1.0, 0.0, 0.0)));
   agregar(cuboAbaseDeRejillas);
   agregar(cubopeq);

   agregar(glm::rotate(float(M_PI), glm::vec3(1.0, 0.0, 0.0)));
   agregar(cuboAbaseDeRejillas);
   agregar(cubopeq);

   mov_cubos = cubopeq->leerPtrMatriz(ind_cubo);
}

unsigned GrafoCubos::leerNumParametros() const
{
   return 1;
}

void GrafoCubos::actualizarEstadoParametro(const unsigned iParam, const float tSec)
{
   switch (iParam)
   {
   case 0:
      *mov_cubos = glm::rotate(float(2 * M_PI * tSec), glm::vec3(0.0, 0.0, 1.0));
      break;

   default:
      break;
   }
}

GrafoFlechas::GrafoFlechas()
{
   ponerNombre("GrafoFlechas");

   NodoGrafoEscena *flecha = new NodoGrafoEscena();
   flecha->agregar(glm::scale(glm::vec3(0.2, 0.2, 0.2)));
   flecha->agregar(new Flecha3D(2, 2, 20));

   agregar(flecha);

   agregar(glm::rotate(float(M_PI / 2), glm::vec3(1.0, 0.0, 0.0)));
   agregar(flecha);

   agregar(glm::rotate(float(-M_PI / 2), glm::vec3(0.0, 0.0, 1.0)));
   agregar(flecha);
}

// EXAMEN DE OTRO AÑO
CubosConPalos::CubosConPalos()
{
   ponerNombre("Coso");
   NodoGrafoEscena *cubo1 = new NodoGrafoEscena();
   NodoGrafoEscena *cubo2 = new NodoGrafoEscena();
   NodoGrafoEscena *palo1 = new NodoGrafoEscena();
   NodoGrafoEscena *palo2 = new NodoGrafoEscena();
   NodoGrafoEscena *conjuntopalos = new NodoGrafoEscena();
   NodoGrafoEscena *conjuntopaloscubo = new NodoGrafoEscena();

   cubo1->agregar(new Cubo());

   unsigned indice_cubo2 = cubo2->agregar(glm::rotate(float(0), glm::vec3(1.0, 0.0, 0.0)));
   cubo2->agregar(glm::scale(glm::vec3(0.5, 0.5, 0.5)));
   cubo2->agregar(new Cubo());

   palo1->agregar(glm::translate(glm::vec3(0.0, 0.5, 0.0)));
   palo1->agregar(glm::scale(glm::vec3(0.3, 0.6, 0.5)));
   palo1->agregar(new CilindroConTapa(20, 20));

   palo2->agregar(glm::translate(glm::vec3(0.0, 1.25, 0.0)));
   palo2->agregar(glm::rotate(float(M_PI / 2), glm::vec3(0.0, 0.0, 1.0)));
   palo2->agregar(glm::scale(glm::vec3(0.3, 0.6, 0.5)));
   palo2->agregar(new CilindroConTapa(20, 20));

   unsigned indice_palos = conjuntopalos->agregar(glm::rotate(float(0), glm::vec3(1.0, 0.0, 0.0)));
   conjuntopalos->agregar(palo1);
   conjuntopalos->agregar(palo2);

   unsigned indice_cubopalos = conjuntopaloscubo->agregar(glm::rotate(float(0), glm::vec3(1.0, 0.0, 0.0)));
   conjuntopaloscubo->agregar(glm::translate(glm::vec3(1, 1.5, 0)));
   conjuntopaloscubo->agregar(conjuntopalos);
   conjuntopaloscubo->agregar(cubo2);

   agregar(cubo1);
   agregar(conjuntopaloscubo);

   mov_cubos = cubo2->leerPtrMatriz(indice_cubo2);
   mov_palo = conjuntopalos->leerPtrMatriz(indice_palos);
   mov_cubopalo = conjuntopaloscubo->leerPtrMatriz(indice_cubopalos);
}

unsigned CubosConPalos::leerNumParametros() const
{
   return 3;
}

void CubosConPalos::actualizarEstadoParametro(const unsigned iParam, const float tSec)
{
   switch (iParam)
   {
   case 0:
      *mov_cubos = glm::rotate(float(3 * M_PI * tSec), glm::vec3(0.0, 1.0, 0.0));
      break;

   case 1:
      *mov_palo = glm::rotate(float(2 * M_PI * tSec), glm::vec3(0.0, 1.0, 0.0));
      break;

   case 2:
      *mov_cubopalo = glm::rotate(float(2 * M_PI * tSec), glm::vec3(0.0, 1.0, 0.0));
      break;
   default:
      break;
   }
}

Practicando3::Practicando3(int h, int a)
{
   ponerNombre("Coso");
   NodoGrafoEscena *cubo1 = new NodoGrafoEscena();
   NodoGrafoEscena *cubo2 = new NodoGrafoEscena();

   unsigned indice_esca = cubo1->agregar(glm::scale(glm::vec3(1.0, 1.0, 1.0)));
   cubo1->agregar(glm::translate(glm::vec3(1.0, 1.0, 0.0)));
   cubo1->agregar(new Cubo());

   unsigned indice_tra = cubo2->agregar(glm::translate(glm::vec3(0.0, 0.0, 0.0)));
   cubo2->agregar(glm::translate(glm::vec3(1.0, 3.0, 0.0)));
   cubo2->agregar(glm::translate(glm::vec3(-1.0, -1.0, 0.0)));
   unsigned indice_rot = cubo2->agregar(glm::rotate(0.0f, glm::vec3(1.0, 0.0, 0.0)));
   cubo2->agregar(glm::translate(glm::vec3(1.0, 1.0, 0.0)));
   cubo2->agregar(new Cubo());

   agregar(glm::scale(glm::vec3(a, float(h) / 2, 1.0)));
   agregar(cubo2);
   agregar(cubo1);

   rot_cubo = cubo2->leerPtrMatriz(indice_rot);
   escalado_cubopeq = cubo1->leerPtrMatriz(indice_esca);
   counter_escalado = cubo2->leerPtrMatriz(indice_tra);
}

unsigned Practicando3::leerNumParametros() const
{
   return 2;
}

void Practicando3::actualizarEstadoParametro(const unsigned iParam, const float tSec)
{
   switch (iParam)
   {
   case 0:
      *rot_cubo = glm::rotate(float(glm::abs(cos(tSec * M_PI))), glm::vec3(0.0, 0.0, 1.0));
      break;

   case 1:
      *escalado_cubopeq = glm::scale(glm::vec3(1.0, float(glm::abs(cos(tSec * M_PI))), 1.0));
      *counter_escalado = glm::translate(glm::vec3(0.0, 2 * (-1 + float(glm::abs(cos(tSec * M_PI)))), 0.0));
      break;

   default:
      break;
   }
}

Practicando4::Practicando4()
{
   ponerNombre("Coso");
   NodoGrafoEscena *pelota = new NodoGrafoEscena();
   NodoGrafoEscena *tapa = new NodoGrafoEscena();
   NodoGrafoEscena *caja = new NodoGrafoEscena();

   caja->agregar(glm::translate(glm::vec3(0.0, 0.0, 0.5)));
   caja->agregar(glm::translate(glm::vec3(-0.5, -0.5, 0)));
   caja->agregar(new RejillaY(5, 5));

   tapa->agregar(glm::rotate(float(M_PI * 1.5), glm::vec3(1.0, 0.0, 0.0)));
   tapa->agregar(glm::translate(glm::vec3(0.0, 0.0, 0.5)));
   tapa->agregar(glm::translate(glm::vec3(-0.5, -0.5, 0)));
   unsigned indice_rot = tapa->agregar(glm::rotate(0.0f, glm::vec3(1.0, 0.0, 0.0)));
   tapa->agregar(new RejillaY(5, 5));

   unsigned indice_tras = pelota->agregar(glm::translate(glm::vec3(0.0, 0.0, 0.0)));
   pelota->agregar(glm::scale(glm::vec3(0.5, 0.5, 0.5)));
   pelota->agregar(new Esfera(5, 10));
   agregar(pelota);
   agregar(tapa);

   agregar(caja);
   agregar(glm::rotate(float(M_PI / 2), glm::vec3(0.0, 1.0, 0.0)));
   agregar(caja);
   agregar(glm::rotate(float(M_PI / 2), glm::vec3(0.0, 1.0, 0.0)));
   agregar(caja);
   agregar(glm::rotate(float(M_PI / 2), glm::vec3(0.0, 1.0, 0.0)));
   agregar(caja);
   agregar(glm::rotate(float(M_PI / 2), glm::vec3(1.0, 0.0, 0.0)));
   agregar(caja);

   rotaciontapa = tapa->leerPtrMatriz(indice_rot);
   traslacionpelota = pelota->leerPtrMatriz(indice_tras);
}

unsigned Practicando4::leerNumParametros() const
{
   return 2;
}

void Practicando4::actualizarEstadoParametro(const unsigned iParam, const float tSec)
{
   switch (iParam)
   {
   case 0:
      *rotaciontapa = glm::rotate(float(glm::abs(cos(tSec * M_PI))), glm::vec3(1.0, 0.0, 0.0));
      break;

   case 1:
      *traslacionpelota = glm::translate(glm::vec3(0.0, glm::abs(0.4 * cos(tSec * M_PI)), 0.0));
      break;

   default:
      break;
   }
}

NodoCubo24::NodoCubo24()
{
   Textura *text = new Textura("window-icon.jpg");
   Material *mate = new Material(text, 0.8, 0.7, 0.3, 50.0);
   agregar(mate);
   agregar(new Cubo24());
}

NodoDiscoP4::NodoDiscoP4()
{
   ponerNombre("Nodo ejercicio adicional práctica 4, examen 27 enero");
   Textura *text = new Textura("cuadricula.jpg");
   Material *mate = new Material(text, 0.8, 0.7, 0.3, 50.0);
   agregar(mate);
   agregar(new MallaDiscoP4());
}

NodoDiscoP42::NodoDiscoP42()
{
   ponerNombre("Nodo ejercicio adicional práctica 4, examen 27 enero");
   Textura *text = new Textura("cuadricula.jpg");
   Material *mate = new Material(text, 0.8, 0.7, 0.3, 50.0);
   agregar(mate);
   agregar(new MallaDiscoP42());
}

BethovenP4::BethovenP4(int n)
{
   ponerNombre("BethovenP4");
   Textura *text = new TexturaBeth(n);

   Material *mate = new Material(text, 0.75, 0.65, 0.25, 50.0);
   agregar(mate);
   agregar(new MallaPLY("beethoven"));
}


MiEsferaE1::MiEsferaE1(int i, int j)
{
   fil = i;
   col = j;

   agregar(new Esfera(30, 30));
}



bool MiEsferaE1::cuandoClick(const glm::vec3 &centro_oc)
{
   std::cout << "Mi fila es " << fil << " mi col es " << col << std::endl;
   return true;
}

GrafoEsferasP5::GrafoEsferasP5()
{
   const unsigned
       n_filas_esferas = 8,
       n_esferas_x_fila = 5;
   const float
       e = 0.4 / n_esferas_x_fila;
   int aux=1;
   agregar(glm::scale(glm::vec3(e, e, e)));
   for (unsigned i = 0; i < n_filas_esferas; i++)
   {
      NodoGrafoEscena *fila_esferas = new NodoGrafoEscena();
      fila_esferas->ponerIdentificador(aux);
      aux++;
      for (unsigned j = 0; j < n_esferas_x_fila; j++)
      {
         MiEsferaE1 *esfera = new MiEsferaE1(i, j);
         esfera->ponerIdentificador(aux);
         aux++;
         fila_esferas->agregar(glm::translate(glm::vec3(2.2, 0.0, 0.0)));
         fila_esferas->agregar(esfera);
      }
      agregar(fila_esferas);
      agregar(glm::translate(glm::vec3(0.0, 0.0, 5.0)));
   }
}

//REVISAR PORQUE ESTO NO ME VA
MiEsferaE2::MiEsferaE2(int i, int j)
{
   fil = i;
   col = j;

   ponerColor(glm::vec3(1.0,1.0,1.0));
   agregar(new Esfera(30, 30));
   
}



bool MiEsferaE2::cuandoClick(const glm::vec3 &centro_oc)
{
   if(leerColor()==glm::vec3(1.0,1.0,1.0)){
      ponerColor(glm::vec3(1.0,0.0,0.0));
   }else{
      ponerColor(glm::vec3(1.0,1.0,1.0));
   }
   return true;
}

GrafoEsferasP52::GrafoEsferasP52()
{
   const unsigned
       n_filas_esferas = 8,
       n_esferas_x_fila = 5;
   const float
       e = 0.4 / n_esferas_x_fila;
   int aux=1;
   agregar(glm::scale(glm::vec3(e, e, e)));
   for (unsigned i = 0; i < n_filas_esferas; i++)
   {
      NodoGrafoEscena *fila_esferas = new NodoGrafoEscena();
      for (unsigned j = 0; j < n_esferas_x_fila; j++)
      {
         MiEsferaE2 *esfera = new MiEsferaE2(i, j);
         esfera->ponerIdentificador(aux);
         aux++;
         fila_esferas->agregar(glm::translate(glm::vec3(2.2, 0.0, 0.0)));
         fila_esferas->agregar(esfera);
      }
      agregar(fila_esferas);
      agregar(glm::translate(glm::vec3(0.0, 0.0, 5.0)));
   }
}
