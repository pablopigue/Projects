// Nombre: Pablo, Apellidos: Martín Palomino, Titulación: GIM
// email: pablomarpa@correo.ugr.es, DNI o pasaporte: 75930976H
#include "examen-ec-p123.h"

Ejer1::Ejer1(){

    vertices.push_back({0,0,0});//0
    vertices.push_back({2,0,0});//1
    vertices.push_back({3,0,0});//2
    vertices.push_back({4,0,0});//3
    vertices.push_back({5,0,0});//4
    vertices.push_back({6,0,0});//5

    vertices.push_back({0,0,1});//6
    vertices.push_back({2,0,1});//7
    vertices.push_back({3,1.5,1});//8
    vertices.push_back({4,1.5,1});//9
    vertices.push_back({5,1.5,1});//10
    vertices.push_back({6,1.5,1});//11

    triangulos= {{0,1,6},{1,6,7},{1,2,7},{2,7,8},{2,3,8},{3,8,9},{3,4,9},{4,9,10},{4,5,10},{5,10,11}};

    
    col_ver ={{0.25,0.25,0.25},{0.25,0.25,0.25},{1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,1.0},
    {0.25,0.25,0.25},{0.25,0.25,0.25},{1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,1.0}};
    


}

Ejer2::Ejer2(int n){
    assert( n>1);

    float grado=2*M_PI/float(n);
    //La idea es rotar toda la figura y unirla como antes
    //pero he tenido problemas rotando la figura y haciendo los triangulos
    for(int i=0;i< n;i++){
        vertices.push_back({0,0,0});//0
        vertices.push_back({2*cos(grado*i),sin(grado*i),0});//1
        vertices.push_back({3*cos(grado*i),sin(grado*i),0});//2
        vertices.push_back({4*cos(grado*i),sin(grado*i),0});//3
        vertices.push_back({5*cos(grado*i),sin(grado*i),0});//4
        vertices.push_back({6*cos(grado*i),sin(grado*i),0});//5

        vertices.push_back({0,0,1});//6
        vertices.push_back({2*cos(grado*i),0,1});//7
        vertices.push_back({3*cos(grado*i),1.5*sin(grado*i),1});//8
        vertices.push_back({4*cos(grado*i),1.5*sin(grado*i),1});//9
        vertices.push_back({5*cos(grado*i),1.5*sin(grado*i),1});//10
        vertices.push_back({6*cos(grado*i),1.5*sin(grado*i),1});//11
    }

    for(int i=0;i<n-1;i++){
        triangulos.push_back({i*6,i*6+1,(i+1)*6});
        triangulos.push_back({i*6+1,(i+1)*6,(i+1)*6+1});

        triangulos.push_back({i*6+1,i*6+2,(i+1)*6+1});

        triangulos.push_back({i*6+2,(i+1)*6+1,(i+1)*6+2});

        triangulos.push_back({i*6+2,i*6+3,(i+1)*6+2});

        triangulos.push_back({i*6+3,(i+1)*6+2,(i+1)*6+3});

        triangulos.push_back({i*6+3,i*6+4,(i+1)*6+3});

        triangulos.push_back({i*6+4,(i+1)*6+3,(i+1)*6+4});

        triangulos.push_back({i*6+4,i*6+5,(i+1)*6+4});

        triangulos.push_back({i*6+5,(i+1)*6+4,(i+1)*6+5});
    }
}

Ejer3::Ejer3(float w,float h ){
    assert( w>1 and h<0.3);
    ponerNombre("Examen");
    altura=h;
    //La idea es que como solo tenemos un único cubo voy haciendo transformaciones primero
    //en una transformacion de el cubo y despues en los dos a la vez (que es la misma instancia de cubo)
    //teniendo en cuenta las restricciones del problema por lo que cuando escalo los dos realmente deshago
    //el escalado del de arriba y cuando escalo el de abajo por lo tanto (que es el total) hago una traslacion
    //del de arriba para mantenerlo pegado al de abajo
    //El problema es que no he tomado bien las traslaciones y por  lo tanto no esta en el borde completamente
    //y tampoco sigue la tapa correctamente
    NodoGrafoEscena *cubo=new NodoGrafoEscena();


    cubo->agregar(new Cubo());

    unsigned indice_escala= agregar(glm::scale(glm::vec3(1.0,1.0,1.0)));
    agregar(glm::scale(glm::vec3(w,h,1.0)));
    agregar(cubo);
    agregar(glm::scale(glm::vec3(1/w*0.2,1/h*0.2,0.2)));

    unsigned indice_counterescala= agregar(glm::scale(glm::vec3(1.0,1.0,1.0)));
    unsigned indice_tras= agregar(glm::translate(glm::vec3(0.0,0.0,0.0)));
    unsigned indice_rot= agregar(glm::rotate(0.0f,glm::vec3(0.0,1.0,0.0)));
    agregar(glm::translate(glm::vec3(2*w,0,0)));
    agregar(glm::translate(glm::vec3(0.0,2,0.0)));
    agregar(cubo);

    rot_cuboarriba=leerPtrMatriz(indice_rot);
    traslacion=leerPtrMatriz(indice_tras);
    escalado_cub_abajo=leerPtrMatriz(indice_escala);
    counterescalado=leerPtrMatriz(indice_counterescala);
}

unsigned Ejer3::leerNumParametros() const{
   return NUM_PARAMS;
}

void Ejer3::actualizarEstadoParametro( const unsigned iParam, const float tSec ){
   switch (iParam)
   {
   case 0:
        *rot_cuboarriba=glm::rotate(float(tSec*3*M_PI),glm::vec3(0.0,1.0,0.0));
      break;
         
   case 1:
        *escalado_cub_abajo=glm::scale(glm::vec3(1.0,float(glm::abs(cos(tSec*M_PI))),1.0));
        *counterescalado=glm::scale(glm::vec3(1.0,1/float(glm::abs(cos(tSec*M_PI))),1.0));
        *traslacion=glm::translate(glm::vec3(0.0,2*(-3*altura+float(glm::abs(cos(tSec*M_PI)))),0.0));
      break;
   
   default:
      break;
   }
}