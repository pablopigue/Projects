// Nombre: Pablo, Apellidos: Martín Palomino, Titulación: GIM
// email: pablomarpa@correo.ugr.es, DNI o pasaporte: 75930976H
// *********************************************************************
// **
// ** Asignatura: INFORMÁTICA GRÁFICA
// ** 
// ** Mallas indexadas creadas por revolución de un perfil (implementación). Clase 'MallaRevol' y derivadas.
// ** Copyright (C) 2016-2024 Carlos Ureña
// **
// ** Implementación de las clases 
// **    + MallaRevol: malla indexada de triángulos obtenida por 
// **      revolución de un perfil (derivada de MallaInd)
// **    + MallaRevolPLY: malla indexada de triángulos, obtenida 
// **      por revolución de un perfil leído de un PLY (derivada de MallaRevol)
// **    + algunas clases derivadas de MallaRevol
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
#include "lector-ply.h"
#include "malla-revol.h"

using namespace std ;

// *****************************************************************************




// Método que crea las tablas de vértices, triángulos, normales y cc.de.tt.
// a partir de un perfil y el número de copias que queremos de dicho perfil.
void MallaRevol::inicializar
(
   const std::vector<glm::vec3> & perfil,     // tabla de vértices del perfil original
   const unsigned               num_copias  // número de copias del perfil
)
{
   using namespace glm ;
   vector<vec3> normales_arista;

   for (unsigned i = 0; i < perfil.size()-1; i++) {
      float v_1 = (perfil[i+1] - perfil[i])[0];
      float v_2 = (perfil[i+1] - perfil[i])[1];
      vec3 norm(vec3(v_2, -v_1, 0.0f));

      if (length(norm) != 0.0)
         norm = normalize(norm);
      
      normales_arista.push_back(norm);
   }

   vector<vec3> normales_vert;

   normales_vert.push_back(normales_arista[0]);
   for (unsigned i = 1; i < perfil.size()-1; i++) {
      normales_vert.push_back(normalize(normales_arista[i-1] + normales_arista[i]));
   }

   normales_vert.push_back(normales_arista[perfil.size() - 2]);

   //Vectores d y t
   vector<float> d, t, sumas_parciales;
   float suma_total;

   for (unsigned i = 0; i < perfil.size()-1; i++) {
      d.push_back(sqrt(length(perfil[i+1] - perfil[i])));
   }

   sumas_parciales.push_back(0.0f);
   for (unsigned i = 1; i < perfil.size(); i++) {
      sumas_parciales.push_back(sumas_parciales[i-1] + d[i-1]);
   }

   suma_total = sumas_parciales[perfil.size()-1];

   t.push_back(0.0f);
   for (unsigned i = 1; i < perfil.size(); i++)
      t.push_back(sumas_parciales[i] / suma_total);
   
   // COMPLETAR: práctica 2: implementar algoritmo de creación de malla de revolución
   //
   // Escribir el algoritmo de creación de una malla indexada por revolución de un 
   // perfil, según se describe en el guion de prácticas.
   //
   // ............................... 
   for(unsigned i=0;i<num_copias;i++){
      for(unsigned j=0;j<perfil.size();j++){
         double giro= (M_PI*2*i)/(num_copias-1);
         glm::vec3 vertice_girado={perfil[j].x*cos(giro)+perfil[j].z*sin(giro),perfil[j].y,perfil[j].x*(-sin(giro)+perfil[j].z)};
         vertices.push_back(vertice_girado);

         vec3 aux = vec3(normales_vert[j][0] * cos(giro), normales_vert[j][1], -normales_vert[j][0] * sin(giro));
         if (length(aux) != 0.0)
            normalize(aux);
         nor_ver.push_back(aux);
         
         cc_tt_ver.push_back({float(i) / (num_copias-1), 1-t[j]});
      }
   }

   for (unsigned i = 0; i < num_copias-1; i++) {
      for (unsigned j = 0; j < perfil.size() - 1; j++) {
         int k = i * perfil.size() + j;

         triangulos.push_back({k, k + perfil.size(), k + perfil.size() + 1});
         triangulos.push_back({k, k + perfil.size() + 1, k + 1});
      }
   }



}

// -----------------------------------------------------------------------------
// constructor, a partir de un archivo PLY

MallaRevolPLY::MallaRevolPLY
(
   const std::string & nombre_arch,
   const unsigned      nperfiles
)
{
   ponerNombre( std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ));
   // COMPLETAR: práctica 2: crear la malla de revolución
   // Leer los vértice del perfil desde un PLY, después llamar a 'inicializar'
   // ...........................
   vector<glm::vec3> perfil;
   LeerVerticesPLY(nombre_arch,perfil);
   inicializar(perfil,nperfiles);


}

Cilindro::Cilindro(const int num_verts_per, const unsigned nperfiles){
   ponerNombre("Cilindro");

   std::vector<glm::vec3> perfil;
   int num_seg=num_verts_per-1;
   for(int i=0;i<=num_seg;i++){
      glm::vec3 vector(1.0,i*(1/float(num_seg)),0);
      perfil.push_back(vector);
   }

   inicializar(perfil,nperfiles);
}

Cono::Cono(const int num_verts_per, const unsigned nperfiles){
   ponerNombre("Cono");

   std::vector<glm::vec3> perfil;
   int num_seg=num_verts_per-1;
   for(int i=0;i<=num_seg;i++){
      glm::vec3 vector(1/float(num_seg)*i,1-(1/float(num_seg)*i),0);
      perfil.push_back(vector);
   }

   inicializar(perfil,nperfiles);
}

Esfera::Esfera(const int num_verts_per, const unsigned nperfiles){
   ponerNombre("Esfera");

   std::vector<glm::vec3> perfil;
   int num_seg=num_verts_per-1;
   for(int i=0;i<=num_seg;i++){
      glm::vec3 vector(cos(-M_PI/2+i*M_PI*1/float(num_seg)),sin(-M_PI/2+i*M_PI*1/float(num_seg)),0);
      perfil.push_back(vector);
   }

   inicializar(perfil,nperfiles);
}

//EXAMEN2023P2
Flecha3D::Flecha3D(int n, int m, int nRev){
   ponerNombre("Flecha3D");

   std::vector<glm::vec3> perfil;

   int num_seg=20-1;

   for(int i=0;i<=num_seg;i++){
      glm::vec3 vector(0.5,i*(m/float(num_seg)),0);
      perfil.push_back(vector);
   }


   for(int i=0;i<=num_seg;i++){
      glm::vec3 vector(m-(n*i)/float(num_seg),m+(n*i)/float(num_seg),0);
      perfil.push_back(vector);
   }
   inicializar(perfil,nRev);
}

//EXamen2019P123 random
Prisma::Prisma(int n,int nRev){

   int num_seg=n-1;
   std::vector<glm::vec3> perfil;

   for(int i=0;i<=num_seg;i++){
      glm::vec3 vector(1/float(num_seg)*i,1-(1/float(num_seg)*i),0);
      perfil.push_back(vector);
   }
   
   for(int i=0;i<=num_seg;i++){
      glm::vec3 vector(1-(1/float(num_seg)*i),-1/float(num_seg)*i,0);
      perfil.push_back(vector);
   }

   inicializar(perfil,nRev);
}

//Examen2019 otro
Fuente::Fuente(int nRev){

   int num_seg=20-1;
   float radio1=0.6;
   float radio2=0.4;
   std::vector<glm::vec3> perfil;
   for(int i=0;i<=num_seg;i++){
      glm::vec3 vector(radio1*cos(-M_PI/2+i*(M_PI*0.25)*1/float(num_seg)),radio1*sin(-M_PI/2+i*(M_PI*0.25)*1/float(num_seg)),0);
      perfil.push_back(vector);
   }

   float desplazamientox=radio1*cos(-M_PI/2+(M_PI*0.25));
   for(int i=0;i<=num_seg;i++){
      glm::vec3 vector(desplazamientox+radio2*cos(-M_PI/2+i*(M_PI*0.25)*1/float(num_seg)),radio2*sin(-M_PI/2+i*(M_PI*0.25)*1/float(num_seg)),0);
      perfil.push_back(vector);
   }
   float yultima=radio2*sin(-M_PI/2+(M_PI*0.25));
   perfil.push_back(glm::vec3(0.0,yultima,0.0));

   inicializar(perfil,nRev);
}

//ParaExamen2019ModeloAnimar

CilindroConTapa::CilindroConTapa(const int num_verts_per, const unsigned nperfiles){
   ponerNombre("Cilindro");

   std::vector<glm::vec3> perfil;
   int num_seg=num_verts_per-1;
   perfil.push_back(glm::vec3(0.0,0.0,0.0));

   for(int i=0;i<=num_seg;i++){
      glm::vec3 vector(1.0,i*(1/float(num_seg)),0);
      perfil.push_back(vector);
   }

   perfil.push_back(glm::vec3(0.0,1.0,0.0));

   inicializar(perfil,nperfiles);
}




