// Nombre: Pablo, Apellidos: Martín Palomino, Titulación: GIM
// email: pablomarpa@correo.ugr.es, DNI o pasaporte: 75930976H
#include "latapeones.h"

using namespace std;
using namespace glm;

Peon::Peon(int nperfiles) {
    mat_tras = leerPtrMatriz(agregar(translate(vec3(0.0,0.0,0.0))));
    agregar(new MallaRevolPLY("peon.ply", nperfiles));
}

bool Peon::cuandoClick(const glm::vec3 & centro_oc) {
    *mat_tras = translate(vec3(0.0, 0.0, aux));
    aux++;
    return true;
}

Lata::Lata(const std::string & nombre_arch) {

    Textura* cocacola = new Textura(nombre_arch);
    Material* materialLata = new Material(cocacola, 0.5, 0.7, 0.5, 50.0);
    Material* materialExtremos = new Material(0.3, 0.4, 0.5, 50.0);

    NodoGrafoEscena* base = new NodoGrafoEscena();
    NodoGrafoEscena* torso = new NodoGrafoEscena();
    NodoGrafoEscena* tapa = new NodoGrafoEscena();

    base->agregar(materialExtremos);
    base->agregar(new MallaRevolPLY("lata-pinf.ply", 20));

    tapa->agregar(materialExtremos);
    tapa->agregar(new MallaRevolPLY("lata-psup.ply", 20));

    torso->agregar(materialLata);
    torso->agregar(new MallaRevolPLY("lata-pcue.ply", 20));

    agregar(base);
    agregar(torso);
    agregar(tapa);
}

LataPeones::LataPeones() {

    ponerNombre( std::string("Grafo LataPeones") );

    // Identificadores
    int ident_lata_cola = 1;
    int ident_peon_madera = 2;
    int ident_peon_blanco = 3;
    int ident_peon_negro = 4;

    Textura * textura_madera = new TexturaXY("text-madera.jpg");
    Material * material_madera = new Material(textura_madera, 0.2, 0.7, 1.0, 50.0);
    Material * material_blanco = new Material(0.1, 1.0, 0.0, 1.0);
    Material * material_negro = new Material(0.1, 0.3, 1.0, 5.0);

    NodoGrafoEscena *peones = new NodoGrafoEscena();
    peones->agregar(translate(vec3(0.0, 0.3, 0.6)));
    peones->agregar(scale(vec3(0.3, 0.3, 0.3)));

    NodoGrafoEscena * lata_cola = new Lata("lata-coke.jpg");
    lata_cola->ponerNombre("Lata de Coca-Cola");
    lata_cola->ponerIdentificador(ident_lata_cola);

    Peon *peon_madera = new Peon(20);
    peon_madera->ponerNombre("Peón de madera");
    peon_madera->ponerIdentificador(ident_peon_madera);
    peones->agregar(material_madera);
    peones->agregar(peon_madera);

    Peon *peon_negro = new Peon(20);
    peon_negro->ponerColor(glm::vec3(0.0,0.0,0.0));
    peon_negro->ponerNombre("Peón Negro");
    peon_negro->ponerIdentificador(ident_peon_negro);
    peones->agregar(translate(vec3(2.5, 0.0, 0.0)));
    peones->agregar(material_negro);
    peones->agregar(peon_negro);

    Peon *peon_blanco = new Peon(20);
    peon_blanco->ponerColor(glm::vec3(1.0,1.0,1.0));
    peon_blanco->ponerNombre("Peón blanco");
    peon_blanco->ponerIdentificador(ident_peon_blanco);
    peones->agregar(translate(vec3(2.5, 0.0, 0.0)));
    peones->agregar(material_blanco);
    peones->agregar(peon_blanco);



    agregar(peones);
    agregar(lata_cola);
}

VariasLatasPeones::VariasLatasPeones() {

    ponerNombre( std::string("Grafo VariasLatasPeones") );

    // Identificadores
    int ident_lata_cola = 1;
    int ident_peon_madera = 2;
    int ident_peon_blanco = 3;
    int ident_peon_negro = 4;
    int identPepsi = 5;
    int identUGR = 6;

    Material * material_blanco = new Material(0.1, 1.0, 0.0, 1.0);
    Material * material_negro = new Material(0.1, 0.3, 1.0, 5.0);
    Textura * textura_madera = new TexturaXY("text-madera.jpg");
    Material * material_madera = new Material(textura_madera, 0.2, 0.7, 1.0, 50.0);

    NodoGrafoEscena *peones = new NodoGrafoEscena();
    peones->agregar(translate(vec3(0.0, 0.3, 0.6)));
    peones->agregar(scale(vec3(0.3, 0.3, 0.3)));

    NodoGrafoEscena * lata_cola = new Lata("lata-coke.jpg");
    lata_cola->ponerNombre("Lata de Coca-Cola");
    lata_cola->ponerIdentificador(ident_lata_cola);

    Peon *peon_madera = new Peon(20);
    peon_madera->ponerNombre("Peón de madera");
    peon_madera->ponerIdentificador(ident_peon_madera);
    peones->agregar(material_madera);
    peones->agregar(peon_madera);

    Peon *peon_negro = new Peon(20);
    peon_negro->ponerColor(glm::vec3(0.0,0.0,0.0));
    peon_negro->ponerNombre("Peón Negro");
    peon_negro->ponerIdentificador(ident_peon_negro);
    peones->agregar(translate(vec3(2.5, 0.0, 0.0)));
    peones->agregar(material_negro);
    peones->agregar(peon_negro);

    Peon *peon_blanco = new Peon(20);
    peon_blanco->ponerColor(glm::vec3(1.0,1.0,1.0));
    peon_blanco->ponerNombre("Peón blanco");
    peon_blanco->ponerIdentificador(ident_peon_blanco);
    peones->agregar(translate(vec3(2.5, 0.0, 0.0)));
    peones->agregar(material_blanco);
    peones->agregar(peon_blanco);

    NodoGrafoEscena* lataPepsi = new NodoGrafoEscena();
    lataPepsi->ponerNombre("Lata de Pepsi");
    lataPepsi->ponerIdentificador(identPepsi);
    lataPepsi->agregar(translate(vec3(1.0, 0.0, 0.0)));
    lataPepsi->agregar(new Lata("lata-pepsi.jpg"));

    NodoGrafoEscena* lataUGR = new NodoGrafoEscena();

    lataUGR->ponerNombre("Lata de la UGR");
    lataUGR->ponerIdentificador(identUGR);
    lataUGR->agregar(translate(vec3(2.0, 0.0, 0.0)));
    lataUGR->agregar(new Lata("window-icon.jpg"));

    agregar(peones);
    agregar(lata_cola);
    agregar(lataPepsi);
    agregar(lataUGR);
}



