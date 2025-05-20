// Nombre: Pablo, Apellidos: Martín Palomino, Titulación: GIM
// email: pablomarpa@correo.ugr.es, DNI o pasaporte: 75930976H
// *********************************************************************
// **
// ** Asignatura: INFORMÁTICA GRÁFICA
// ** 
// ** Gestión de materiales y texturas (implementación)
// ** Copyright (C) 2016-2024 Carlos Ureña
// **
// ** Implementación de:
// **    + clase 'Textura' (y derivadas 'TexturaXY', 'TexturaXZ')
// **    + clase 'Material' 
// **    + clase 'FuenteLuz'
// **    + clase 'ColFuentesLuz' (y la clase derivada 'Col2Fuentes')
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

#include "aplicacion-ig.h"
#include "materiales-luces.h"

using namespace std ;

const bool trazam = false ;

// **********************************************************************

Textura::Textura( const std::string & nombreArchivoJPG )
{
   // COMPLETAR: práctica 4: cargar imagen de textura
   // (las variables de instancia están inicializadas en la decl. de la clase)
   // El nombre del archivo debe convertirse a una cadena (char *) acabada en 
   // 0 tradicional en C. Para eso debe usarse el método 'c_str' de la clase 
   // 'std::string'.
   // El nombre del archivo debe ir sin el 'path', la función 'LeerArchivoJPG' lo 
   // busca en 'materiales/imgs' y si no está se busca en 'archivos-alumno'
   // .....
   imagen = LeerArchivoJPEG(nombreArchivoJPG.c_str(),ancho,alto);
}

// ---------------------------------------------------------------------

//----------------------------------------------------------------------

void Textura::enviar()
{
   // COMPLETAR: práctica 4: enviar la imagen de textura a la GPU
   // y configurar parámetros de la textura (glTexParameter)
   // .......
   glGenTextures( 1, &ident_textura);
   // Activa textura con identificador ’idTex’ :
   glActiveTexture(GL_TEXTURE0);
   // Activa textura con identificador nombre_tex
   glBindTexture( GL_TEXTURE_2D, ident_textura );
   // Envia la textura a la GPU
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, imagen );
   // Generar mipmaps (versiones a resolución reducida)
   glGenerateMipmap( GL_TEXTURE_2D );
   // Selección de texels para texturas cercanas (magnificadas)
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   // Selección de texels para texturas lejanas (minimizadas)
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
   // Selección de texels con coord. de textura fuera de rango
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

   enviada = true;
}

//----------------------------------------------------------------------

Textura::~Textura( )
{
   using namespace std ;
   cout << "destruyendo textura...imagen ==" << imagen << endl ;
   if ( imagen != nullptr )
      delete [] imagen ;

   imagen = nullptr ;
   cout << "hecho (no hecho!)" << endl << flush ;
}

//----------------------------------------------------------------------
// por ahora, se asume la unidad de texturas #0

void Textura::activar(  )
{
   using namespace std ;
   assert( aplicacionIG != nullptr );
   Cauce * cauce = aplicacionIG->cauce ; assert( cauce != nullptr );

   // COMPLETAR: práctica 4: enviar la textura a la GPU (solo la primera vez) y activarla
   // .......
   if (!enviada) {
      enviar();
   }

   cauce->fijarEvalText(true, ident_textura);
   cauce->fijarTipoGCT(modo_gen_ct, coefs_s, coefs_t);

}
// *********************************************************************
// texturaXY
TexturaXY::TexturaXY( const std::string & nom ): Textura(nom) {
   modo_gen_ct = mgct_coords_objeto;
   coefs_s[0] = 1.0f;
   coefs_s[1] = 0.0f;
   coefs_s[2] = 0.0f;
   coefs_s[3] = 0.0f;
   coefs_t[0] = 0.0f;
   coefs_t[1] = 1.0f;
   coefs_t[2] = 0.0f;
   coefs_t[3] = 0.0f;
}
// *********************************************************************
// texturaXZ
TexturaXZ::TexturaXZ( const std::string & nom ): Textura(nom) {
   modo_gen_ct = mgct_coords_objeto;
   coefs_s[0] = 1.0f;
   coefs_s[1] = 0.0f;
   coefs_s[2] = 0.0f;
   coefs_s[3] = 0.0f;
   coefs_t[0] = 0.0f;
   coefs_t[1] = 0.0f;
   coefs_t[2] = 1.0f;
   coefs_t[3] = 0.0f;
}
// *********************************************************************
// texturaBeth


TexturaBeth::TexturaBeth(int n) : Textura("text-madera.jpg"){

   modo_gen_ct = mgct_coords_objeto;
   if(n==0){
      coefs_s[0] = 0.0f;
      coefs_s[1] = 1.0f;
      coefs_s[2] = 0.0f;
      coefs_s[3] = 0.0f;
      coefs_t[0] = 1.0f;
      coefs_t[1] = 0.0f;
      coefs_t[2] = 0.0f;
      coefs_t[3] = 0.0f;
   }else{
      coefs_s[0] = 1.0f;
      coefs_s[1] = 0.0f;
      coefs_s[2] = 0.0f;
      coefs_s[3] = 0.0f;
      coefs_t[0] = 0.0f;
      coefs_t[1] = 1.0f;
      coefs_t[2] = 0.0f;
      coefs_t[3] = 0.0f;
   }

}
// *********************************************************************
// crea un material usando un color plano y los coeficientes de las componentes

Material::Material( const float p_k_amb, const float p_k_dif,
                    const float p_k_pse, const float p_exp_pse )
{
   textura  = nullptr ;
   k_amb    = p_k_amb ;
   k_dif    = p_k_dif ;
   k_pse    = p_k_pse ;
   exp_pse  = p_exp_pse ;
}

//----------------------------------------------------------------------
// crea un material usando una textura y los coeficientes de las componentes

Material::Material( Textura * p_textura, const float p_k_amb, const float p_k_dif,
                    const float p_k_pse, const float p_exp_pse )
{
   textura  = p_textura ;  assert( textura != nullptr );
   k_amb    = p_k_amb ;
   k_dif    = p_k_dif ;
   k_pse    = p_k_pse ;
   exp_pse  = p_exp_pse ; assert( 0.5 <= exp_pse ); 
}
//----------------------------------------------------------------------

Material::~Material()
{
   if ( textura != nullptr )
   {  delete textura ;
      textura = nullptr ;
   }
}
//----------------------------------------------------------------------

void Material::ponerNombre( const std::string & nuevo_nombre )
{
   nombre_mat = nuevo_nombre ;
}
//----------------------------------------------------------------------

std::string Material::nombre() const
{
   return nombre_mat ;
}
//----------------------------------------------------------------------

void Material::activar( )
{
   using namespace std ;
   assert( aplicacionIG != nullptr );
   Cauce * cauce = aplicacionIG->cauce ; assert( cauce != nullptr );

   // COMPLETAR: práctica 4: activar un material
   // .....

   if (textura != nullptr)
      textura->activar();
   else
      cauce->fijarEvalText(false);

   assert(exp_pse >= 1.0);

   cauce->fijarParamsMIL(k_amb, k_dif, k_pse, exp_pse);

}
//**********************************************************************

FuenteLuz::FuenteLuz( GLfloat p_longi_ini, GLfloat p_lati_ini, const glm::vec3 & p_color )
{
   if ( trazam )
      cout << "creando fuente de luz." <<  endl << flush ;

   // inicializar parámetros de la fuente de luz
   longi_ini = p_longi_ini ;
   lati_ini  = p_lati_ini  ;
   longi     = longi_ini ;
   lati      = lati_ini ;
   color     = p_color ;
}
//----------------------------------------------------------------------

// incrementar o decrementa la longitud de una fuente de luz
// @param incre (float) incremento del ángulo de longitud (en grados)
//
void FuenteLuz::actualizarLongi( const float incre )
{
   longi = longi + incre ;
   using namespace std ;
   cout << "actualizado angulo de 'longitud' de una fuente de luz, nuevo == " << longi << " grados." << endl ;
}
//----------------------------------------------------------------------

// incrementar o decrementa la latitud de una fuente de luz
// @param incre (float) incremento del ángulo de latitud (en grados)
//
void FuenteLuz::actualizarLati( const float incre )
{
   lati = lati + incre ;
   using namespace std ;
   cout << "actualizado angulo de 'latitud' de una fuente de luz, nuevo == " << lati << " grados." << endl ;
}

//**********************************************************************

ColFuentesLuz::ColFuentesLuz()
{
   max_num_fuentes = -1 ;
}
//----------------------------------------------------------------------

void ColFuentesLuz::insertar( FuenteLuz * pf )  // inserta una nueva
{
   assert( pf != nullptr );

   //pf->ind_fuente = vpf.size() ;
   vpf.push_back( pf ) ;
}
//----------------------------------------------------------------------
// activa una colección de fuentes de luz en el cauce actual

void ColFuentesLuz::activar( )
{
   using namespace std ;
   using namespace glm ;
   assert( aplicacionIG != nullptr );
   Cauce * cauce = aplicacionIG->cauce ; assert( cauce != nullptr );

   // COMPLETAR: práctica 4: activar una colección de fuentes de luz
   //   - crear un 'std::vector' con los colores y otro con las posiciones/direcciones,
   //   - usar el método 'fijarFuentesLuz' del cauce para activarlas
   // .....
   std::vector<glm::vec3> colores;
   std::vector<glm::vec4> direcciones;

   for(int i=0;i<vpf.size();i++){
      FuenteLuz *actu= vpf[i];
      float lon = actu->longi;
      float lat = actu->lati;

      float angulo1= lon*2*M_PI/360.0;
      float angulo2= lat*2*M_PI/360.0;

      /*
      float x= cos(angulo2)*cos(angulo1);
      float y= sin(angulo2);
      float z= cos(angulo2)*sin(angulo1);
      */
      //REVISAR EN ALGUN MOMENTO
      /*
      float x= cos(radians(lat))*cos(radians(lon));
      float y= sin(radians(lat));
      float z= cos(radians(lat))*sin(radians(lon));
      */
      float z= cos(radians(lat))*cos(radians(lon));
      float y= sin(radians(lat));
      float x= cos(radians(lat))*sin(radians(lon));

      colores.push_back(glm::vec3(actu->color[0],actu->color[1],actu->color[2]));

      direcciones.push_back(glm::vec4(x,y,z,0.0f));

   }

   cauce->fijarFuentesLuz(colores,direcciones);

}

// ---------------------------------------------------------------------
// pasa a la siguiente fuente de luz (si d==+1, o a la anterior (si d==-1))
// aborta si 'd' no es -1 o +1

void ColFuentesLuz::sigAntFuente( int d )
{
   assert( i_fuente_actual < vpf.size()) ;
   assert( d == 1 || d== -1 );
   i_fuente_actual = unsigned((int(i_fuente_actual+vpf.size())+d) % vpf.size()) ;
   cout << "fuente actual: " << (i_fuente_actual+1) << " / " << vpf.size() << endl ;
}

// ---------------------------------------------------------------------
// devuelve un puntero a la fuente de luz actual

FuenteLuz * ColFuentesLuz::fuenteLuzActual()
{
   assert( vpf[i_fuente_actual] != nullptr );
   return vpf[i_fuente_actual] ;
}
//----------------------------------------------------------------------

ColFuentesLuz::~ColFuentesLuz()
{
   for( unsigned i = 0 ; i < vpf.size() ; i++ )
   {
      assert( vpf[i] != NULL );
      delete vpf[i] ;
      vpf[i] = NULL ;
   }
}
//----------------------------------------------------------------------

void PilaMateriales::push()
{
   assert( actual != nullptr );
   vector_materiales.push_back( actual );
}
//----------------------------------------------------------------------

void PilaMateriales::activar( Material * nuevo_actual )
{
   assert( nuevo_actual != nullptr );
   if ( nuevo_actual != actual )
   {  
      actual = nuevo_actual ;
      actual->activar( ) ;
   }
}
//----------------------------------------------------------------------

void PilaMateriales::pop()
{
   const unsigned n = vector_materiales.size() ;
   assert( n > 0 );
   activar( vector_materiales[n-1] ) ;
   vector_materiales.pop_back() ;
}


//--------------------------------------------------------------------------
// actualizar una colección de fuentes en función de una tecla GLFW pulsada
// (se usa el código glfw de la tecla, se llama desde 'main.cpp' con L pulsada)
// devuelve 'true' sii se ha actualizado algo

bool ProcesaTeclaFuenteLuz( ColFuentesLuz * col_fuentes, int glfw_key )
{
   assert( col_fuentes != nullptr );

   FuenteLuz * fuente     = col_fuentes->fuenteLuzActual() ; assert( fuente != nullptr );
   bool        redib      = true ;
   const float delta_grad = 10.0 ; // incremento en grados para long. y lati.

   switch( glfw_key )
   {
      case GLFW_KEY_RIGHT_BRACKET : // tecla '+' en el teclado normal
      case GLFW_KEY_KP_ADD :
         col_fuentes->sigAntFuente( +1 );
         break ;
      case GLFW_KEY_SLASH :        // tecla con '-' y '_' en el teclado normal
      case GLFW_KEY_KP_SUBTRACT :
         col_fuentes->sigAntFuente( -1 );
         break ;
      case GLFW_KEY_LEFT :
         fuente->actualizarLongi( -delta_grad );
         break ;
      case GLFW_KEY_RIGHT :
         fuente->actualizarLongi( +delta_grad );
         break ;
      case GLFW_KEY_DOWN :
         fuente->actualizarLati( -delta_grad );
         break ;
      case GLFW_KEY_UP :
         fuente->actualizarLati( +delta_grad );
         break ;
      default :
         redib = false ;
         break ;
   }
   return redib ;
}

//-----------------------------------------------------------------------
// constructor de una colección de fuentes de luz sencilla que incluye
// dos fuentes de luz.

Col2Fuentes::Col2Fuentes()
{
   using namespace glm ;
   const float f0 = 0.7, f1 = 0.3 ;
   insertar( new FuenteLuz( +45.0, 60.0,  vec3 { f0, f0,     f0,    } ) );
   insertar( new FuenteLuz( -70.0, -30.0, vec3 { f1, f1*0.5, f1*0.5 } ) );

}
