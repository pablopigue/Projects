// Nombre: Pablo, Apellidos: Martín Palomino, Titulación: GIM
// email: pablomarpa@correo.ugr.es, DNI o pasaporte: 75930976H
// *********************************************************************
// **
// ** Asignatura: INFORMÁTICA GRÁFICA
// ** 
// ** Mallas indexadas (implementación). Clase 'MallaInd' y derivadas.
// ** Copyright (C) 2016-2024 Carlos Ureña
// **
// ** Implementación de las clases 
// **        + MallaInd: malla indexada de triángulos (derivada de Objeto3D)
// **        + MallaPLY: malla indexada de triángulos, leída de un PLY (derivada de MallaInd)
// **        + algunas clases derivadas de MallaInd.
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
#include "aplicacion-ig.h"
#include "malla-ind.h"   // declaración de 'ContextoVis'
#include "lector-ply.h"
#include "seleccion.h"   // para 'ColorDesdeIdent' 


// *****************************************************************************
// funciones auxiliares

// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // nombre por defecto
   ponerNombre("malla indexada, anónima");
}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;
}

//-----------------------------------------------------------------------------
// calcula la tabla de normales de triángulos una sola vez, si no estaba calculada

void MallaInd::calcularNormalesTriangulos()
{

   
   // si ya está creada la tabla de normales de triángulos, no es necesario volver a crearla
   const unsigned nt = triangulos.size() ;
   assert( 1 <= nt );
   if ( 0 < nor_tri.size() )
   {
      assert( nt == nor_tri.size() );
      return ;
   }

   // COMPLETAR: Práctica 4: creación de la tabla de normales de triángulos
   // ....
   for (int i = 0; i < nt; i++) {
      glm::vec3 p = vertices[triangulos[i][0]];
      glm::vec3 q = vertices[triangulos[i][1]];
      glm::vec3 r = vertices[triangulos[i][2]];

      glm::vec3 a = q - p;
      glm::vec3 b = r - p;

      glm::vec3 m_c = cross(a, b);

      glm::vec3 n_c;
      if (length(m_c) != 0.0)
         n_c = normalize(m_c);
      else
         n_c = glm::vec3(0.0,0.0,0.0);

      nor_tri.push_back(n_c);
   }
}


// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcularNormales()
{
   using namespace glm ;
   // COMPLETAR: en la práctica 4: calculo de las normales de la malla
   // se debe invocar en primer lugar 'calcularNormalesTriangulos'
   // .......
   calcularNormalesTriangulos();

   nor_ver = std::vector<vec3>(vertices.size(), vec3(0.0, 0.0, 0.0));

   for (int i = 0; i < triangulos.size(); i++) {
      for (int j = 0; j < 3; j++) {
         unsigned indice_vertice = triangulos[i][j];

         nor_ver[indice_vertice] = nor_ver[indice_vertice] + nor_tri[i];
      }
   }

   for (int i = 0; i < nor_ver.size(); i++)
      if (length(nor_ver[i]) != 0.0)
         nor_ver[i] = normalize(nor_ver[i]);

}


// --------------------------------------------------------------------------------------------

void MallaInd::visualizarGL( )
{
   // comprobar algunas precondiciones básicas
   using namespace std ;
   assert( aplicacionIG != nullptr );
   Cauce * cauce = aplicacionIG->cauce ; assert( cauce != nullptr );
   CError();

   // si la malla no vértices o no tiene triángulos, imprimir advertencia y salir.
   if ( triangulos.size() == 0 || vertices.size() == 0 )
   {  cout << "advertencia: intentando dibujar malla vacía '" << leerNombre() << "'" << endl << flush ;
      return ;
   }

   // COMPLETAR: práctica 1: cambiar color del cauce
   //
   // Si el objeto tiene un color asignado (se comprueba con 'tieneColor')
   //    - hacer push del color actual del cauce
   //    - fijar el color en el cauce usando el color del objeto (se lee con 'leerColor()')
   
   if(tieneColor()){
      cauce->pushColor();
      cauce->fijarColor(leerColor());
   }

   // COMPLETAR: práctica 1: crear el descriptor de VAO, si no está creado
   //  Si el puntero 'dvao' es nulo, crear el descriptor de VAO
   //   * en primer lugar se crea el descriptor de VAO, con su constructor 
   //     (se le pasa como parámetro la tabla de posiciones y el número de atributos del cauce). 
   //   * se añade el descriptor de VBO con la tabla de índices (la tabla de triángulos),
   //   * finalmente se añaden al VAO los descriptores VBOs con tablas de atributos 
   //     que no estén vacías
   //  Si el VAO ya está creado, (dvao no nulo), no hay que hacer nada.
   //
   if(dvao==nullptr){
      dvao= new DescrVAO(numero_atributos_cauce,new DescrVBOAtribs(ind_atrib_posiciones,vertices));
      if ( triangulos.size() > 0 )
         dvao->agregar( new DescrVBOInds( triangulos ));
      if ( col_ver.size() > 0)
         dvao->agregar( new DescrVBOAtribs( ind_atrib_colores, col_ver ));
      if ( nor_ver.size() > 0)
         dvao->agregar( new DescrVBOAtribs( ind_atrib_normales, nor_ver ));
      if ( cc_tt_ver.size() > 0 )
         dvao->agregar( new DescrVBOAtribs( ind_atrib_coord_text, cc_tt_ver ));
   }

   // COMPLETAR: práctica 1: visualizar el VAO usando el método 'draw' de 'DescrVAO'
   dvao->draw(GL_TRIANGLES); 

   // COMPLETAR: práctica 1: restaurar color anterior del cauce 
   //
   // Si el objeto tiene un color asignado (se comprueba con 'tieneColor')
   //    - hacer 'pop' del color actual del cauce
  if(tieneColor()){
      cauce->popColor();
   }

}


// -----------------------------------------------------------------------------
// Visualizar el objeto con OpenGL
// usa las tablas de normales, colores y coordenadas de textura, si no están vacías.
      
void MallaInd::visualizarGeomGL( )
{
   // Comprobar que el descriptor de VAO ya está creado
   // (es decir, este método únicamente se podrá invocar después de que 
   // se haya llamado a 'visualizaGL')
   
   assert( dvao != nullptr );

   // COMPLETAR: práctica 1: visualizar únicamente la geometría del objeto 
   // 
   //    1. Desactivar todas las tablas de atributos del VAO (que no estén vacías)
      /*if ( vertices.size() > 0 )
         dvao->habilitarAtrib(0,false); Esto no es un atributo de vertices*/
      if ( col_ver.size() > 0)
         dvao->habilitarAtrib(ind_atrib_colores,false);
      if ( nor_ver.size() > 0)
         dvao->habilitarAtrib(ind_atrib_normales,false);
      if ( cc_tt_ver.size() > 0 )
         dvao->habilitarAtrib(ind_atrib_coord_text,false);

   //    2. Dibujar la malla (únicamente visualizará los triángulos), se usa el método 'draw' del VAO (dvao)
      dvao->draw(GL_TRIANGLES);

   //    3. Volver a activar todos los atributos para los cuales la tabla no esté vacía
      /*if ( vertices.size() > 0 )
         dvao->habilitarAtrib(0,true); Esto no es un atributo de vertices*/
      if ( col_ver.size() > 0)
         dvao->habilitarAtrib(ind_atrib_colores,true);
      if ( nor_ver.size() > 0)
         dvao->habilitarAtrib(ind_atrib_normales,true);
      if ( cc_tt_ver.size() > 0 )
         dvao->habilitarAtrib(ind_atrib_coord_text,true);
   // ....

}

// -----------------------------------------------------------------------------
// Visualizar las normales del objeto, si no tiene tabla de normales imprime 
// advertencia y no hace nada.

void MallaInd::visualizarNormalesGL(  )
{
   using namespace std ;
   assert( aplicacionIG != nullptr );
   Cauce * cauce = aplicacionIG->cauce ; assert( cauce != nullptr );

   if ( nor_ver.size() == 0 )
   {
      cout << "Advertencia: intentando dibujar normales de una malla que no tiene tabla (" << leerNombre() << ")." << endl ;
      return ;
   }  

   if( nor_ver.size() != vertices.size() )
   {
      cout << "Error visu. normales: tabla de normales no vacía y de tamaño distinto a la de vértices." << endl ;
      cout << "Nombre del objeto        : " << leerNombre() << endl ;
      cout << "Tamaño tabla vértices    : " << vertices.size() << endl ;
      cout << "Tamaño tabla de normales : " << nor_ver.size() << endl ;
      exit(1);
   }
   CError();

   // COMPLETAR: práctica 4: visualizar las normales del objeto MallaInd
   // 
   // *1* Si el puntero al descriptor de VAO de normales ('dvao_normales') es nulo, 
   //    debemos de crear dicho descriptor, con estos pasos:
   //
   //       * Para cada posición 'v_i' de un vértice en el vector 'vertices':
   //             - Leer la correspondiente normal 'n_i' del vector de normales ('nor_ver').
   //             - Añadir 'v_i' al vector 'segmentos_normales'.
   //             - Añadir 'v_i+a*n_i' al vector 'segmentos_normales'.
   //
   //       * Crear el objeto descriptor del VAO de normales, para ello se usa el vector 
   //          'segmentos_normales' y se tiene en cuenta que esa descriptor únicamente gestiona 
   //          una tabla de atributos de vértices (la de posiciones, ya que las otras no se 
   //          necesitan).
   // 
   // *2* Visualizar el VAO de normales, usando el método 'draw' del descriptor, con el 
   //       tipo de primitiva 'GL_LINES'.

   //  ..........
   if (dvao_normales == nullptr) {

      for (int i = 0; i < vertices.size(); i++) {
         segmentos_normales.push_back(vertices[i]);
         segmentos_normales.push_back(vertices[i] + (0.35f * nor_ver[i]));
      }

      dvao_normales = new DescrVAO(numero_atributos_cauce, new DescrVBOAtribs(ind_atrib_posiciones, segmentos_normales));
   }

   dvao_normales->draw(GL_LINES);
}

// -----------------------------------------------------------------------------
// visualizar el objeto en 'modo seleccion', es decir, sin iluminación y con los colores 
// basados en los identificadores de los objetos
void MallaInd::visualizarModoSeleccionGL() 
{

   using namespace std ;
   assert( aplicacionIG != nullptr );
   Cauce * cauce = aplicacionIG->cauce ; assert( cauce != nullptr );

   // COMPLETAR: práctica 5: visualizar la malla en modo selección 
   //
   // Se debe escribir código para visualizar únicamente la geometría, pero usando el color 
   // obtenido a partir del identificador. El código da estos pasos:
   // 
   // 1. Leer el identificador del objeto (con 'leerIdentificador'). Si el objeto tiene 
   //    identificador (es decir, si su identificador no es -1)
   //       + Hacer push del color del cauce, con 'pushColor'.
   //       + Fijar el color del cauce (con 'fijarColor') usando un color obtenido a 
   //         partir del identificador (con 'ColorDesdeIdent'). 
   // 2. Invocar 'visualizarGeomGL' para visualizar la geometría.
   // 3. Si tiene identificador: hacer pop del color, con 'popColor'.
   //
   int id = leerIdentificador();
   if(id!=-1){
      cauce->pushColor();
      cauce->fijarColor(ColorDesdeIdent(id));
   }

   visualizarGeomGL();

   if(id!=-1){
      cauce->popColor();
   }

}


// ****************************************************************************
// Clase 'MallaPLY'

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
   ponerNombre( std::string("malla leída del archivo '") + nombre_arch + "'" );

   // COMPLETAR: práctica 2: leer archivo PLY e inicializar la malla
   // ..........................
   LeerPLY(nombre_arch,vertices,triangulos);


   // COMPLETAR: práctica 4: invocar  a 'calcularNormales' para el cálculo de normales
   // .................
   calcularNormales();

}

void MallaInd::asignarColor(){
   int n=0.6*vertices.size();
   int m=vertices.size()-n;
   col_ver.clear();
   for(int i=0; i < n;i++){
      col_ver.push_back(glm::vec3(0.5,0.5,0.5));
   }

   for(int i=0; i < m;i++){
      col_ver.push_back(glm::vec3(0.2,0.2,0.2));
   }
}

// ****************************************************************************
// Clase 'Cubo

Cubo::Cubo()
:  MallaInd( "cubo 8 vértices" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;

      //Método realizado para prácticar de cara al examen
      //asignarColor();

      calcularNormales();

}

Tetraedro::Tetraedro()
:  MallaInd( "tetraedro 4 vértices" )
{

   vertices =
      {  { 0, +0.5, 0 }, // 0
         { -0.5, 0, -0.5 }, // 1
         { +0.5, 0, +0.5 }, // 2
         { -0.5, 0, +0.5 }, // 3
      } ;



   triangulos =
      {  {0,1,3},
         {0,2,3},
         {1,2,3},
         {0,1,2}
      } ;
   calcularNormales();
}

CuboColores::CuboColores()
:  MallaInd( "cubo 8 vértices con colores" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;


   col_ver={
      {0,0,0},
      {0,0,1},
      {0,1,0},
      {0,1,1},
      {1,0,0},
      {1,0,1},
      {1,1,0},
      {1,1,1},

   };



}

//Remirar
EstrellaZ::EstrellaZ(int n)
:  MallaInd( "Estrella" )
{
   double centro_x=0.5;
   double centro_y=0.5;
   double centro_z=0;
   double circunf = M_PI*2/n;
   double radio=0.5;

   vertices.push_back({centro_x,centro_y,centro_z});
   col_ver.push_back({1.0,1.0,1.0});


   for(int i=0;i<2*n;i++){
      if(i%2==0){
         glm::vec3 vecaux(centro_x + radio*cos(circunf * double(i)/2),centro_y + radio*sin(circunf*double(i)/2),centro_z);
         vertices.push_back(vecaux);
      }else{
         glm::vec3 vecaux2(centro_x + (radio/2)*cos(circunf * double(i)/2),centro_y + (radio/2)*sin(circunf*double(i)/2),centro_z);
         vertices.push_back(vecaux2);
      }
   }

/**/
   for(int i=0;i<2*n;i++){
      triangulos.push_back({0,i,i+1});
   }
   triangulos.push_back({0,2*n,1});

   for(int i=0;i<2*n;i++){
      col_ver.push_back(vertices[i+1]);
   }


}

//Lo entregue con las proporciones mal
CasaX::CasaX()
:  MallaInd( "Casa" )
{

   vertices =
      {  { 0.0, 0.2, 0.2 }, // 0
         { 0.0, 0.2, 0.8 }, // 1
         { 0, +0.8, 0.2 }, // 2
         { 0.0, +0.8, +0.8 }, // 3
         { +1.0, 0.2, 0.2 }, // 4
         { +1.0, 0.2, +0.8 }, // 5
         { +1.0, +0.8, 0.2 }, // 6
         { +1.0, +0.8, +0.8 }, // 7
         { +1.0,+1.0,0.5},
         {0.0,+1.0,0.5}

      } ;



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)


         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3} , // Z+ (+1)
         {2,3,9},{7,8,6},
         {8,9,6},{9,8,3},
         {3,7,8},{9,2,6}

      } ;
   int tam = vertices.size();
   for(int i=0; i < tam;i++){
      col_ver.push_back(vertices[i]);
   }

}

MallaTriangulo::MallaTriangulo()
:  MallaInd( "Triangulo" )
{

   vertices =
      {  { 0, +sqrt(2), 0 }, // 0
         { -0.5, 0, 0 }, // 1
         { +0.5, 0, 0 }, // 2
      } ;



   triangulos =
      {  {0,1,2},
      } ;

}

//Modificado Respecto entrega pues lo tenia leyendo mal enunciado
MallaCuadrado::MallaCuadrado()
:  MallaInd( "Cuadrado" )
{

   vertices =
      {  { -1.0,-1.0 , 0.0 }, // 0
         { -1.0, 1.0, 0.0 }, // 1
         { +1.0, -1.0, 0.0 }, // 2
         { +1.0,1.0,0.0}
      } ;



   triangulos =
      {  {0,3,1},
         {0,3,2}
      } ;

}

MallaPiramideL::MallaPiramideL()
:  MallaInd( "PiramideL" )
{

   vertices =
      {  {0,0,0},
         {-1.0,0,0},
         {-1.0,0,-1.0},
         {+1.0,0,-1.0},
         {+1.0,0,+1.0},
         {0,0,+1.0},
         {0,+1.0,0}
      } ;



   triangulos =
      {  {0,1,6},
         {0,5,6},
         {1,2,6},
         {2,3,6},
         {4,3,6},
         {4,5,6}
      } ;

}

PiramideEstrellaZ::PiramideEstrellaZ(int n)
:  MallaInd( "PiramideEstrella" )
{
   double centro_x=0.5;
   double centro_y=0.5;
   double centro_z=0;
   double circunf = M_PI*2/n;
   double radio=0.5;

   vertices.push_back({centro_x,centro_y,centro_z});
   col_ver.push_back({1.0,1.0,1.0});


   for(int i=0;i<2*n;i++){
      if(i%2==0){
         glm::vec3 vecaux(centro_x + radio*cos(circunf * double(i)/2),centro_y + radio*sin(circunf*double(i)/2),centro_z);
         vertices.push_back(vecaux);
      }else{
         glm::vec3 vecaux2(centro_x + (radio/2)*cos(circunf * double(i)/2),centro_y + (radio/2)*sin(circunf*double(i)/2),centro_z);
         vertices.push_back(vecaux2);
      }
   }

/**/
   for(int i=0;i<2*n;i++){
      triangulos.push_back({0,i,i+1});
   }
   triangulos.push_back({0,2*n,1});

   //añado piramide
   vertices.push_back({0.5,0.5,0.5});

   for(int i=0;i<2*n;i++){
      triangulos.push_back({2*n+1,i,i+1});
   }
   triangulos.push_back({2*n+1,2*n,1});

   for(int i=0;i<2*n+1;i++){
      col_ver.push_back(vertices[i+1]);
   }
   
}

PiramideEstrellaZ2::PiramideEstrellaZ2(int n)
:  MallaInd( "PiramideEstrella2" )
{
   double centro_x=0.5;
   double centro_y=0.5;
   double centro_z=0;
   double circunf = M_PI*2/n;
   double radio=0.5;

   vertices.push_back({centro_x,centro_y,centro_z});
   col_ver.push_back({1.0,1.0,1.0});


   for(int i=0;i<2*n;i++){
      glm::vec3 vecaux(centro_x + radio*cos(circunf * double(i)/2),centro_y + radio*sin(circunf*double(i)/2),centro_z);
      vertices.push_back(vecaux);
   }

   //añado piramide
   vertices.push_back({0.5,0.5,0.5});

   for(int i=0;i<2*n;i++){
      if(i%2==0)
         triangulos.push_back({2*n+1,i,0});
   }
   triangulos.push_back({2*n+1,2*n,0});

   for(int i=0;i<2*n+1;i++){
      col_ver.push_back(vertices[i+1]);
   }
   
}

RejillaY::RejillaY(int m,int n)
:  MallaInd( "RejillaY" )
{
   assert( (m>1 and n>1));
   int longitud1=n-1;
   int longitud2=m-1;

   for(int i=0;i<=longitud1;i++){
      for(int j=0;j<=longitud2;j++){
         vertices.push_back({i/double(longitud1),j/double(longitud2),0});
      }
   }

   for(int i=0;i<longitud1;i++){
      for(int j=0;j<longitud2;j++){
         triangulos.push_back({i*m+j,(i+1)*m+j,(i+1)*m+j+1});
         triangulos.push_back({i*m+j,i*m+j+1,(i+1)*m+j+1});
         
      }
   }

   for(int i=0;i<vertices.size();i++){
      col_ver.push_back(vertices[i]);
   }



}

Torre::Torre(int n)
:  MallaInd( "Torre" )
{
   vertices.push_back({0,0,0});
   vertices.push_back({+1.0,0,0});
   vertices.push_back({0,0,+1.0});
   vertices.push_back({+1.0,0,+1.0});
   
   for(int i=1; i<=n;i++){
      vertices.push_back({0,i,0});
      vertices.push_back({+1.0,i,0});
      vertices.push_back({0,i,+1.0});
      vertices.push_back({+1.0,i,+1.0});

      
   }
   const int constante=4;
   
   for(int i=0;i<n;i++){
      
      triangulos.push_back({i*constante,i*constante+1,(i+1)*constante});
      triangulos.push_back({i*constante+1,(i+1)*constante+1,(i+1)*constante});

      triangulos.push_back({i*constante+2,i*constante+3,(i+1)*constante+2});
      triangulos.push_back({i*constante+3,(i+1)*constante+3,(i+1)*constante+2});

      triangulos.push_back({i*constante,(i+1)*constante,(i+1)*constante +2});
      triangulos.push_back({i*constante+2,(i+1)*constante+2,i*constante});

      triangulos.push_back({i*constante+1,(i+1)*constante+1,(i+1)*constante +3});
      triangulos.push_back({i*constante+3,(i+1)*constante+3,i*constante+1});
      
      
   }


}

PiramideBaseRara::PiramideBaseRara(){
   vertices.push_back({0,0,0});
   vertices.push_back({+1.0,0,0});
   vertices.push_back({0,0,+1.0});
   vertices.push_back({+1.0,0,+1.0});
   vertices.push_back({0.5,0.5,0.5});

   triangulos.push_back({0,1,4});
   triangulos.push_back({0,2,4});
   triangulos.push_back({2,3,4});
   triangulos.push_back({3,1,4});

   

}

Practicando::Practicando(){

   vertices.push_back({0,0,0});//0
   vertices.push_back({1,0,0});//1
   vertices.push_back({2,0,0});//2
   vertices.push_back({3,0,0});//3
   vertices.push_back({4,0,0});//4

   vertices.push_back({0,0,1});//5
   vertices.push_back({1,0,1});//6
   vertices.push_back({2,1,1});//7
   vertices.push_back({3,0,1});//8
   vertices.push_back({4,0,1});//9

   triangulos= {{0,1,5},{1,5,6},{1,2,6},{2,6,7},{2,3,7},{3,7,8},{3,4,8},{4,8,9}};

   for(int i=0;i<vertices.size();i++){
      if(vertices[i].y>0){
         col_ver.push_back({0.5,0.5,0.5});
      }else{
         col_ver.push_back({0,0,0});
      }
   }


   

}

Practicando2::Practicando2(int n){

   vertices.push_back({0,0,0});//0
   vertices.push_back({1,0,0});//1
   vertices.push_back({2,1,0});//2
   vertices.push_back({3,0,0});//3
   vertices.push_back({4,0,0});//4

   for(int i=1;i<n;i++){
      vertices.push_back({0,0,i});//5
      vertices.push_back({1,0,i});//6
      vertices.push_back({2,1,i});//7
      vertices.push_back({3,0,i});//8
      vertices.push_back({4,0,i});//9
   }

   triangulos= {{0,1,5},{1,5,6},{1,2,6},{2,6,7},{2,3,7},{3,7,8},{3,4,8},{4,8,9}};

   for(int i=0;i<n-1;i++){
      triangulos.push_back({i*5,i*5+1,(i+1)*5});

      triangulos.push_back({i*5+1,(i+1)*5,(i+1)*5+1});

      triangulos.push_back({i*5+1,i*5+2,(i+1)*5+1});

      triangulos.push_back({i*5+2,(i+1)*5+1,(i+1)*5+2});

      triangulos.push_back({i*5+2,i*5+3,(i+1)*5+2});

      triangulos.push_back({i*5+3,(i+1)*5+2,(i+1)*5+3});

      triangulos.push_back({i*5+3,i*5+4,(i+1)*5+3});

      triangulos.push_back({i*5+4,(i+1)*5+3,(i+1)*5+4});

   }

   for(int i=0;i<vertices.size();i++){
      if(vertices[i].y>0){
         col_ver.push_back({0.5,0.5,0.5});
      }else{
         col_ver.push_back({0,0,0});
      }
   }


   

}

Cubo24::Cubo24() {
   vertices =
      {
         {+1.0, -1.0, +1.0}, // 0
         {+1.0, +1.0, +1.0}, // 1
         {+1.0, +1.0, -1.0}, // 2
         {+1.0, -1.0, -1.0}, // 3


         {-1.0, -1.0, +1.0}, // 4
         {-1.0, +1.0, +1.0}, // 5
         {+1.0, +1.0, +1.0}, // 6
         {+1.0, -1.0, +1.0}, // 7


         {-1.0, -1.0, -1.0}, // 8
         {-1.0, +1.0, -1.0}, // 9
         {-1.0, +1.0, +1.0}, // 10
         {-1.0, -1.0, +1.0}, // 11

         {+1.0, -1.0, -1.0}, // 12
         {+1.0, +1.0, -1.0}, // 13
         {-1.0, +1.0, -1.0}, // 14
         {-1.0, -1.0, -1.0}, // 15


         {+1.0, +1.0, +1.0}, // 16
         {-1.0, +1.0, +1.0}, // 17
         {-1.0, +1.0, -1.0}, // 18
         {+1.0, +1.0, -1.0}, // 19

         {+1.0, -1.0, +1.0}, // 20
         {-1.0, -1.0, +1.0}, // 21
         {-1.0, -1.0, -1.0}, // 22
         {+1.0, -1.0, -1.0}  // 23
      };

   triangulos =
      {
         {0, 3, 2},
         {0, 2, 1},

         {4, 7, 6},
         {4, 6, 5},

         {8, 11, 10},
         {8, 10, 9},

         {12, 15, 14},
         {12, 14, 13},

         {16, 19, 18},
         {16, 18, 17},

         {20, 22, 23},
         {20, 21, 22}
      };
      
   cc_tt_ver = {
      {0.0, 1.0},
      {0.0, 0.0},
      {1.0, 0.0},
      {1.0, 1.0},

      {0.0, 1.0},
      {0.0, 0.0},
      {1.0, 0.0},
      {1.0, 1.0},

      {0.0, 1.0},
      {0.0, 0.0},
      {1.0, 0.0},
      {1.0, 1.0},

      {0.0, 1.0},
      {0.0, 0.0},
      {1.0, 0.0},
      {1.0, 1.0},

      {0.0, 1.0},
      {0.0, 0.0},
      {1.0, 0.0},
      {1.0, 1.0},

      {0.0, 1.0},
      {0.0, 0.0},
      {1.0, 0.0},
      {1.0, 1.0}
   };

   calcularNormales();
}
MallaDiscoP4::MallaDiscoP4()
{
   ponerColor({1.0, 1.0, 1.0});
   const unsigned ni = 23, nj = 31;

   for (unsigned i = 0; i < ni; i++)
      for (unsigned j = 0; j < nj; j++)
      {
         const float
             fi = float(i) / float(ni - 1),
             fj = float(j) / float(nj - 1),
             ai = 2.0 * M_PI * fi,
             x = fj * cos(ai),
             y = fj * sin(ai),
             z = 0.0;
         vertices.push_back({x, y, z});
         cc_tt_ver.push_back({0.3*x + 0.4,0.3*y +0.4});
      }
   for (unsigned i = 0; i < ni - 1; i++)
      for (unsigned j = 0; j < nj - 1; j++)
      {
         triangulos.push_back({i * nj + j, i * nj + (j + 1), (i + 1) * nj + (j + 1)});
         triangulos.push_back({i * nj + j, (i + 1) * nj + (j + 1), (i + 1) * nj + j});
      }
   
   calcularNormales();
}

MallaDiscoP42::MallaDiscoP42()
{
   ponerColor({1.0, 1.0, 1.0});
   const unsigned ni = 23, nj = 31;

   for (unsigned i = 0; i < ni; i++)
      for (unsigned j = 0; j < nj; j++)
      {
         const float
             fi = float(i) / float(ni - 1),
             fj = float(j) / float(nj - 1),
             ai = 2.0 * M_PI * fi,
             x = fj * cos(ai),
             y = fj * sin(ai),
             z = 0.0;
         vertices.push_back({x, y, z});
         cc_tt_ver.push_back({fi,fj});
      }
   for (unsigned i = 0; i < ni - 1; i++)
      for (unsigned j = 0; j < nj - 1; j++)
      {
         triangulos.push_back({i * nj + j, i * nj + (j + 1), (i + 1) * nj + (j + 1)});
         triangulos.push_back({i * nj + j, (i + 1) * nj + (j + 1), (i + 1) * nj + j});
      }
   
   calcularNormales();
}

PiramidePract::PiramidePract(){
   vertices = {
      {1.0, 0.0, 1.0},
      {1.0, 0.0, -1.0},
      {-1.0, 0.0, -1.0},
      {-1.0, 0.0, 1.0},
      {0.0, 1.0, 0.0},

      {1.0, 0.0, 1.0},
      {1.0, 0.0, -1.0},
      {-1.0, 0.0, -1.0},
      {-1.0, 0.0, 1.0},
      {0.0, 1.0, 0.0},
   };

   triangulos = {
      {0, 1, 4},
      {2, 3, 4},
      
      {8, 5, 9},
      {6, 7, 9},
   };
 
   cc_tt_ver = {
      {0.0, 0.0},
      {1.0, 0.0},
      {1.0, 1.0},
      {0.0, 1.0},
      {0.5, 0.5},

      {1.0, 0.0},
      {1.0, 1.0},
      {0.0, 1.0},
      {0.0, 0.0},
      {0.5, 0.5},
   };

   // Rayas hacia el lado

   // cc_tt_ver = {
   //    {0.0, 0.0},
   //    {0.0, 1.0},
   //    {1.0, 0.5},

   //    {1.0, 0.5},
   //    {0.0, 0.0},
   //    {0.0, 1.0},

   //    {1.0, 0.5},
   //    {0.0, 1.0},
   //    {0.0, 0.0},

   //    {0.0, 0.0},
   //    {1.0, 0.5},
   //    {0.0, 1.0},
   // };

}
//00 arriba izq 10 arriba der 01 abajo iz 11 abajo der

// -----------------------------------------------------------------------------------------------

