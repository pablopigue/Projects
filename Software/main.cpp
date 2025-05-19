#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>
#include <problem.h>
#include <random.hpp>
#include <string>
#include <util.h>
#include <ctime>
// Real problem class
#include "problem1.h"

// All all algorithms
#include "busquedalocal.h"
#include "greedy.h"
#include "randomsearch.h"
#include "geneticos.h"
#include "memetico.h"
#include "bmb.h"
#include "ils.h"
#include "enfriamientoSimulado.h"
#include "grasp.h"

using namespace std;
int main(int argc, char *argv[]) {
  long int seed;

  // Verificar que se hayan pasado los argumentos correctos
  if (argc != 3) {
    cerr << "Uso: " << argv[0] << " <semilla> <fichero_distancias>" << endl;
    return 1;
  }

  
  // Obtener la semilla y el nombre del fichero de los argumentos
  seed = atoi(argv[1]);
  string fichero_distancias = argv[2];
  Random::seed(seed);

  // Leer la matriz de distancias desde el fichero
  ifstream file(fichero_distancias);
  if (!file.is_open()) {
      cerr << "Error: No se pudo abrir el fichero " << fichero_distancias << endl;
      return 1;
  }

  // Leer el número de nodos y el tamaño de la solución
  int num_nodos, tam;
  file >> num_nodos >> tam;

  // Inicializar la matriz de distancias con ceros
  vector<vector<double>> distancias(num_nodos, vector<double>(num_nodos, 0.0));

  // Leer las distancias de la parte diagonal superior
  int i, j;
  double distancia;
  while (file >> i >> j >> distancia) {
      distancias[i][j] = distancia;
      distancias[j][i] = distancia; // Completar la parte inferior para hacerla simétrica
  }
  file.close();

  // Verificar que la matriz de distancias sea cuadrada
  for (const auto &fila : distancias) {
      if (fila.size() != num_nodos) {
          cerr << "Error: La matriz de distancias no es cuadrada." << std::endl;
          return 1;
      }
  }

  RandomSearch ralg = RandomSearch();
  GreedySearch rgreedy = GreedySearch();
  BusquedaLocal rbl = BusquedaLocal(false);
  BusquedaLocal rbl2 = BusquedaLocal(true);

  Geneticos gen = Geneticos(true,false);
  Geneticos gen1 = Geneticos(false,false);
  Geneticos genEst = Geneticos(true,true);
  Geneticos genEst1 = Geneticos(false,true);

  Memetico mem1 = Memetico(1);
  Memetico mem2 = Memetico(2);
  Memetico mem3 = Memetico(3);

  Bmb bmb = Bmb(10);
  Ils ils = Ils(10,false);
  Ils ilsEs= Ils(10,true);
  EnfriamientoSimulado es = EnfriamientoSimulado();
  Grasp grasp = Grasp(false);
  Grasp graspbl = Grasp(true);
  
  vector<pair<string, MH *>> algoritmos = {/*make_pair("RandomSearch", &ralg),make_pair("Greedy", &rgreedy)
  ,make_pair("BusquedaLocal desordenada", &rbl),make_pair("BusquedaLocal ordenada", &rbl2)
  ,make_pair("Genetico_Generacional_Uniforme", &gen),make_pair("Genetico_Generacional_Intercambio", &gen1)
  ,make_pair("Genetico_Estacionario_Uniforme", &genEst),make_pair("Genetico_Estacionario_Intercambio", &genEst1)
  ,make_pair("Memetico1", &mem1)/*,make_pair("Memetico2", &mem2),make_pair("Memetico3", &mem3), make_pair("Bmb", &bmb)
  ,*/make_pair("Ils", &ils)/*,make_pair("IlsEs", &ilsEs),make_pair("EnfriamientoSimulado", &es)/*,make_pair("Grasp", &grasp),make_pair("GraspBL", &graspbl)*/};
  Problem1 rproblem = Problem1(distancias,num_nodos,tam);
  Problem *problem = dynamic_cast<Problem *>(&rproblem);
  for (int i = 0; i < algoritmos.size(); i++) {
    Random::seed(seed);
    cout << algoritmos[i].first << endl;
    MH *mh = algoritmos[i].second;

    auto momentoInicio = chrono::high_resolution_clock::now();
    ResultMH result = mh->optimize(problem,100000);
    auto momentoFin = chrono::high_resolution_clock::now();
    
    chrono::milliseconds tiempo = chrono::duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
    
    /*
    cout <<"Tiempo Pasado: " <<tiempo.count() << " milisegundos" << endl;  
    cout << "Best solution: " << result.solution << endl;
    cout << "Best fitness: " << result.fitness << endl;
    cout << "Evaluations: " << result.evaluations << endl;
    */
    
    cout << tiempo.count() << " " << result.fitness << " " << result.evaluations <<endl;
    
    
  }

  return 0;
}
