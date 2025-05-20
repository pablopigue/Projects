// Nombre: Pablo, Apellidos: Martín Palomino, Titulación: GIM
// email: pablomarpa@correo.ugr.es, DNI o pasaporte: 75930976H
#include "examen-ec-p45.h"

Piramide::Piramide(){
    //NO DA EXACTAMENTE LO PEDIDO PERO HABRIA QUE AJUSTARLO LIGERAMENTE PERO NO ME DA TIEMPO A HACERLO PUES DEBO HACER EL EJERCICIO DE LA P5
   vertices = {
    //Vertices sin repetir
        {1.0,0.0,1.0}, //0
        {1.0,0.0,-1.0}, //1
        {-1.0,0.0,-1.0},//2
        {-1.0,0.0,1.0},//3
        {0.0,2.0,0.0},//4

    //Repeticion de vertices para texturas
        {1.0,0.0,1.0}, //5
        {1.0,0.0,-1.0}, //6
        {-1.0,0.0,-1.0},//7
        {-1.0,0.0,1.0},//8
        {0.0,2.0,0.0},//9

        {1.0,0.0,1.0}, //10
        {1.0,0.0,-1.0}, //11
        {-1.0,0.0,-1.0},//12
        {-1.0,0.0,1.0},//13
        {0.0,2.0,0.0},//14

        //el vertice de arriba necesitamos que se repita 4 veces
        {0.0,2.0,0.0},//15
        {1.0,0.0,-1.0},//16
        {-1.0,0.0,1.0}//17

    
   };

   triangulos = {
    {0,1,4},
    {6,2,9},
    {7,3,14},
    {8,10,15},
    {11,12,13},
    {5,16,17}
   };
    
   cc_tt_ver = {
    {0,1},//0
    {1,1},//1
    {1,1},//2
    {1,1},//3
    {0.5,0.25},//4
    {1,1},//5
    {0,1},//6
    {0,1},//7
    {0,1},//8
    {0.5,0.25},//9
    {1,1},//10
    {1,0},//11
    {0,0},//12
    {0,1},//13
    {0.5,0.25},//14
    {0.5,0.25},//15
    {1,0},//16
    {0,1},//17
   };

   //Tambien habria que definir nor ver porque calcular normales no las da correctas
   //me he dado cuenta tarde y no me da tiempo a cambiarlo pero lo que habria que hacer es asignar a cada vertice en nor ver
   //de tal manera que apunte perpendicularmente que vendrá dado por el vector (1,0,0) (0,1,0) o (0,0,1) segun donde este el vertice
    calcularNormales();
    
}

NodoPiramide::NodoPiramide()
{
   Textura *text = new Textura("window-icon.jpg");
   Material *mate = new Material(text, 0.7, 0.5, 0, 30.0);
   agregar(mate);
   agregar(new Piramide());
}

Triangulo::Triangulo(glm::vec3 v1,glm::vec3 v2, glm::vec3 v3)
{
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);

    triangulos.push_back({0,1,2});


    glm::vec3 centro = (v1+v2+v3);
    for(int i=0;i<3;i++){
        centro[i]=centro[i]/3;
    }
    ponerCentroOC(centro);
}

MiTriangulo::MiTriangulo(glm::vec3 v1,glm::vec3 v2, glm::vec3 v3,int n)
{

   pos=n;
   ponerColor(glm::vec3(1.0,1.0,1.0));
   agregar(new Triangulo(v1,v2,v3));
   
}



bool MiTriangulo::cuandoClick(const glm::vec3 &centro_oc)
{
   if(leerColor()==glm::vec3(1.0,1.0,1.0)){
      ponerColor(glm::vec3(1.0,0.0,0.0));
   }else{
      ponerColor(glm::vec3(1.0,1.0,1.0));
   }
   std::cout << "Soy el triangulo " << pos << std::endl;
   /*
    //Habria que hacer tambien el cuandoClick del padre pero ahora mismo no se como hacer eso con la herencia
    cout << "Ejecutando método 'cuandoClick' por defecto." << endl ;
    cout << "Click realizado sobre el objeto: '" << leerNombre() << "'" << endl ;
    //escena->camaraActual()->mirarHacia(centro_oc);
    */
   return true;
}

grafoexp5::grafoexp5(){
    std::vector<glm::vec3>  vertices;
    vertices.push_back({1.0,0.0,1.0});
    vertices.push_back({1.0,0.0,-1.0});
    vertices.push_back({-1.0,0.0,-1.0});
    vertices.push_back({-1.0,0.0,1.0});
    vertices.push_back({0.0,2.0,0.0});
    int aux =1;
    MiTriangulo *tri0= new MiTriangulo(vertices[0],vertices[1],vertices[4],aux);
    tri0->ponerIdentificador(aux);
    aux++;
    agregar(tri0);
    MiTriangulo *tri1= new MiTriangulo(vertices[1],vertices[2],vertices[4],aux);
    tri1->ponerIdentificador(aux);
    aux++;
    agregar(tri1);
    MiTriangulo *tri2= new MiTriangulo(vertices[2],vertices[3],vertices[4],aux);
    tri2->ponerIdentificador(aux);
    aux++;
    agregar(tri2);
    MiTriangulo *tri3= new MiTriangulo(vertices[3],vertices[0],vertices[4],aux);
    tri3->ponerIdentificador(aux);
    aux++;
    agregar(tri3);
    MiTriangulo *tri4= new MiTriangulo(vertices[0],vertices[1],vertices[2],aux);
    tri4->ponerIdentificador(aux);
    aux++;
    agregar(tri4);
    MiTriangulo *tri5= new MiTriangulo(vertices[2],vertices[3],vertices[0],aux);
    tri5->ponerIdentificador(aux);
    aux++;
    agregar(tri5);


}