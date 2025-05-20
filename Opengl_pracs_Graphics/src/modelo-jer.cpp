// Nombre: Pablo, Apellidos: Martín Palomino, Titulación: GIM
// email: pablomarpa@correo.ugr.es, DNI o pasaporte: 75930976H
#include "malla-ind.h"
#include "modelo-jer.h"
#include "malla-revol.h"

Cabeza::Cabeza(const int num_verts_per,const unsigned nperfiles){
    std::vector<glm::vec3> perfil;
    int num_seg=num_verts_per-1;
    for(int i=0;i<=num_seg;i++){
        glm::vec3 vector(cos(-M_PI/2+i*M_PI*1/float(num_seg)),sin(-M_PI/2+i*M_PI*1/float(num_seg)),0);
        perfil.push_back(vector);
    }

    inicializar(perfil,nperfiles);
}

CilindroCuerpo::CilindroCuerpo(const int num_verts_per, const unsigned nperfiles){

   std::vector<glm::vec3> perfil;
   int num_seg=num_verts_per-1;
   for(int i=0;i<=num_seg;i++){
      glm::vec3 vector(1.0,i*(1/float(num_seg)),0);
      perfil.push_back(vector);
   }

   inicializar(perfil,nperfiles);
}

SemiEsfera::SemiEsfera(const int num_verts_per, const unsigned nperfiles){

   std::vector<glm::vec3> perfil;
   int num_seg=num_verts_per-1;
   for(int i=0;i<=num_seg;i++){
      glm::vec3 vector(cos(-M_PI/2+i*(1/float(num_seg))*M_PI/2),sin(-M_PI/2+i*(1/float(num_seg))*M_PI/2),0);
      perfil.push_back(vector);
   }

   inicializar(perfil,nperfiles);
}

Mochila::Mochila(){
    agregar(scale(vec3(0.8,0.8,0.8)));
    agregar( new Cubo24());
}

BaseCuerpo::BaseCuerpo(){
    ponerNombre("Base");
    NodoGrafoEscena *semiesferaArriba= new NodoGrafoEscena();
    NodoGrafoEscena *semiesferaAbajo= new NodoGrafoEscena();

    semiesferaAbajo->agregar(scale(vec3(1.0,0.72,1.0)));
    semiesferaAbajo->agregar(new SemiEsfera(25,25));

    semiesferaArriba->agregar(translate(vec3(0.0,1.0,0.0)));
    semiesferaArriba->agregar(rotate(float(M_PI),vec3(1.0,0.0,0.0)));
    semiesferaArriba->agregar(new SemiEsfera(25,25));

    agregar(new Cilindro(25,25));
    agregar(semiesferaAbajo);
    agregar(semiesferaArriba);
}

BasePierna::BasePierna(){

    ponerNombre("Pierna");
    NodoGrafoEscena *semiesferaArriba= new NodoGrafoEscena();
    NodoGrafoEscena *semiesferaAbajo= new NodoGrafoEscena();

    semiesferaAbajo->agregar(scale(vec3(1.0,0.5,1.0)));
    semiesferaAbajo->agregar(new SemiEsfera(25,25));

    semiesferaArriba->agregar(translate(vec3(0.0,1.0,0.0)));
    semiesferaArriba->agregar(rotate(float(M_PI),vec3(1.0,0.0,0.0)));
    semiesferaArriba->agregar(new SemiEsfera(25,25));

    agregar(new CilindroCuerpo(25,25));
    agregar(semiesferaAbajo);
    agregar(semiesferaArriba);
}

BaseGafas::BaseGafas(){

    ponerNombre("Gafas");
    NodoGrafoEscena *semiesferaArriba= new NodoGrafoEscena();

    semiesferaArriba->agregar(translate(vec3(0.0,1.0,0.0)));
    semiesferaArriba->agregar(rotate(float(M_PI),vec3(1.0,0.0,0.0)));
    semiesferaArriba->agregar(new SemiEsfera(25,25));

    agregar(translate(vec3(-0.5,0.0,0.0)));
    agregar(rotate(float(M_PI),vec3(1.0,1.0,0.0)));
    
    agregar(new CilindroCuerpo(25,25));
    agregar(new SemiEsfera(25,25));
    agregar(semiesferaArriba);
}

BaseAnteBrazo::BaseAnteBrazo(){

    ponerNombre("Brazo");
    NodoGrafoEscena *esfera= new NodoGrafoEscena();
    NodoGrafoEscena *biceps= new NodoGrafoEscena();

    
    esfera->agregar(translate(vec3(1.0,0.3,0.3)));
    esfera->agregar(scale(vec3(0.5,0.5,0.5)));
    esfera->agregar(new Esfera(25,25));

    biceps->agregar(translate(vec3(0.3,0.3,0.3)));
    biceps->agregar(rotate(float(M_PI),vec3(1.0,1.0,0.0)));
    biceps->agregar(scale(vec3(0.5,0.5,0.5)));
    biceps->agregar(new BaseCuerpo());


    agregar(biceps);
    agregar(esfera);
    

    
}

CuerpoAmongus::CuerpoAmongus(){
    int identificador_amongus=1;
    int identificador_pierna1=2;
    int identificador_pierna2=3;

    //Por defecto se que es -1 pero es para dejar claro que es mi intencion
    int identificador_cuerpo=-1;

    int identificador_mochila=5;
    int identificador_gafas=6;

    //Por defecto se que es -1 pero es para dejar claro que es mi intencion
    int identificador_antebrazo=-1;
    int identificador_articula=-1;

    int identificador_brazo=9;

    ponerNombre("CuerpoAmongus");
    ponerIdentificador(identificador_amongus);

    NodoGrafoEscena *pierna1=new NodoGrafoEscena();
    NodoGrafoEscena *pierna2=new NodoGrafoEscena();
    NodoGrafoEscena *cuerpo= new NodoGrafoEscena();
    NodoGrafoEscena *mochila= new NodoGrafoEscena();
    NodoGrafoEscena *gafas= new NodoGrafoEscena();

    NodoGrafoEscena *antebrazo= new NodoGrafoEscena();
    NodoGrafoEscena *articula= new NodoGrafoEscena();
    NodoGrafoEscena *brazo=new NodoGrafoEscena();

    antebrazo->ponerNombre("Antebrazo");
    antebrazo->ponerIdentificador(identificador_antebrazo);//se refiere a padre brazo
    antebrazo->agregar(translate(vec3(0.9,0.7,0.0)));
    antebrazo->ponerColor(vec3(1.0,0.254,0.012));
    antebrazo->agregar(scale(vec3(0.5,0.5,0.5)));
    antebrazo->agregar(new BaseAnteBrazo());

    articula->ponerNombre("Artícula");
    articula->ponerIdentificador(identificador_articula);//se refiere a padre brazo
    articula->ponerColor(vec3(1.0,0.254,0.012));
    articula->agregar(translate(vec3(1.6,0.83,0.15)));
    unsigned rot_articula=articula->agregar(rotate(0.0f,vec3(1.0,0.0,0.0)));
    articula->agregar(rotate(float(M_PI),vec3(1.0,1.0,0)));
    articula->agregar(scale(vec3(0.2,0.3,0.2)));
    articula->agregar(new BaseCuerpo());
    
    Textura *texturabrazo = new TexturaXZ("metal2.jpeg");
    Material *materialBrazo= new Material(texturabrazo,0.2,0.8,0.8,50);
    brazo->ponerNombre("Brazo");
    brazo->ponerIdentificador(identificador_brazo);
    unsigned ind_traslacion_brazo=brazo->agregar(translate(vec3(0.0,0.0,0.0)));
    brazo->agregar(materialBrazo);
    brazo->agregar(antebrazo);
    brazo->agregar(articula);
    
    
    Material *materialGafas= new Material(0.5,0.2,0.6,100);
    gafas->ponerColor(vec3(1.0,1.0,1.0));
    gafas->ponerNombre("Gafas");
    gafas->ponerIdentificador(identificador_gafas);
    gafas->agregar(scale(vec3(0.35,0.35,0.35)));
    gafas->agregar(translate(vec3(0.0,2.5,2.5)));
    gafas->agregar(materialGafas);
    gafas->agregar(new BaseGafas());
    
    Textura* texturamochila = new Textura("sus.jpg");
    Material* materialMochila = new Material(texturamochila, 0.8, 0.7, 0.3, 50.0);
    mochila->ponerNombre("Mochila");
    mochila->ponerIdentificador(identificador_mochila);
    unsigned ind_mochila_escala=mochila->agregar(scale(vec3(1,1,1)));
    unsigned ind_mochila=mochila->agregar(translate(vec3(0,0,0)));
    mochila->ponerColor(vec3(1.0,0.254,0.012));
    mochila->agregar(scale(vec3(0.7,0.5,0.5)));
    mochila->agregar(translate(vec3(0.0,0.7,-3.0)));
    mochila->agregar(materialMochila);
    mochila->agregar(new Mochila());

    Textura *texturapiernas = new TexturaXZ("metal.jpeg");

    Material *materialPiernas= new Material(texturapiernas,0.8,0.2,0.9,50);

    pierna1->ponerNombre("Pierna1");
    pierna1->ponerIdentificador(identificador_pierna1);
    unsigned ind_pierna1=pierna1->agregar(rotate(0.0f,vec3(0,1,0)));
    pierna1->ponerColor(vec3(1.0,0.304,0.012));
    pierna1->agregar(scale(vec3(0.3,0.6,0.3)));
    pierna1->agregar(translate(vec3(-2.0,-1.6,0.0)));
    pierna1->agregar(materialPiernas);
    pierna1->agregar(new BasePierna());

    pierna2->ponerNombre("Pierna2");
    pierna2->ponerIdentificador(identificador_pierna2);
    unsigned ind_pierna2=pierna2->agregar(rotate(0.0f,vec3(0,1,0)));
    pierna2->ponerColor(vec3(1.0,0.304,0.012));
    pierna2->agregar(scale(vec3(0.3,0.6,0.3)));
    pierna2->agregar(translate(vec3(+2.0,-1.6,0.0)));
    pierna2->agregar(materialPiernas);
    pierna2->agregar(new BasePierna());

    Material *materialCuerpo= new Material(0.8,0.6,0.1,3);
    cuerpo->ponerIdentificador(identificador_cuerpo); //asociado a padre
    cuerpo->ponerNombre("Cuerpo");
    cuerpo->ponerColor(vec3(1.0,0.204,0.012));
    cuerpo->agregar(materialCuerpo);
    cuerpo->agregar(new BaseCuerpo());

    
    unsigned ind_among=agregar(translate(vec3(0.0,0.0,0.0)));
    agregar(mochila);
    agregar(pierna1);
    agregar(pierna2);
    agregar(cuerpo);
    agregar(brazo);  
    agregar(gafas);

    mov_mochila=mochila->leerPtrMatriz(ind_mochila);
    sc_mochila=mochila->leerPtrMatriz(ind_mochila_escala);
    mov_cuerpoeEntero= leerPtrMatriz(ind_among);
    mov_pierna1=pierna1->leerPtrMatriz(ind_pierna1);
    mov_pierna2=pierna2->leerPtrMatriz(ind_pierna2); 
    mov_articula=articula->leerPtrMatriz(rot_articula);
    tras_brazo=brazo->leerPtrMatriz(ind_traslacion_brazo);
}

unsigned CuerpoAmongus::leerNumParametros() const{
    return NUM_PARAMS;
}

void CuerpoAmongus::actualizarEstadoParametro( const unsigned iParam, const float tSec ){
  
    switch (iParam)
    {
    
    case 0:
       *mov_cuerpoeEntero=translate(vec3(0,0,sin(2*M_PI*tSec)));
        break;
    case 1:
        *sc_mochila=scale(vec3(abs(cos(tSec))+0.5,abs(cos(tSec))+0.5,1));
        *mov_mochila=translate(vec3(0,0.3*sin(2.7*M_PI*tSec),0));
        
        break;
    case 2:
        *mov_pierna1=rotate(float(M_PI*0.25*cos(3*M_PI*tSec)),vec3(1,0,0));
        break;
    case 3:
        *mov_pierna2=rotate(float(M_PI*0.25*cos(3*M_PI*tSec + M_PI)),vec3(1,0,0));
        break;
    case 4:
        *tras_brazo=translate(vec3(0,0.2*sin(2.9*M_PI*tSec),0));
        *mov_articula=rotate(float(0.5*M_PI*cos(2*M_PI*tSec)),vec3(0,0,1));
        break;
    }
}


