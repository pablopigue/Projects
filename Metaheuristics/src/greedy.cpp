#include <cassert>
#include <greedy.h>
#include <iostream>
#include <problem1.h>
#include <random.hpp>

using namespace std;

template <class T> void print_vector(string name, const vector<T> &sol) {
  cout << name << ": ";

  for (auto elem : sol) {
    cout << elem << ", ";
  }
  cout << endl;
}

tFitness GreedySearch::Heuristica(const SolutionInfoP1 &info,unsigned elemento,tSolution const &solucion){
  tFitness fitness=0;
  vector<double> copia;
  tFitness maxSum = std::numeric_limits<tFitness>::min();
  tFitness minSum = std::numeric_limits<tFitness>::max();
  //Calculamos una copia de columna_fact con la adición de distancias de nuevo elemento
  for(unsigned i=0;i<info.columna_fact.size();i++){
    double modif = info.columna_fact[i]+info.matriz[solucion[i]][elemento];
    copia.push_back(modif);
  }
  //Calculamos la suma de distancias de nuevo elemento
  double disp =0;
  for(unsigned i=0;i<solucion.size();i++){
    disp+=info.matriz[elemento][solucion[i]];
  }
  copia.push_back(disp);

  for(unsigned i=0; i<copia.size();i++){
    if (copia[i] > maxSum) {
      maxSum = copia[i];
    }
    if (copia[i] < minSum) {
      minSum = copia[i];
    }
  }
  //Calculamos la dispersion
  fitness = maxSum - minSum;
  return fitness;
}

/**
 * Create random solutions until maxevals has been achieved, and returns the
 * best one.
 *
 * @param problem The problem to be optimized
 * @param maxevals Maximum number of evaluations allowed
 * @return A pair containing the best solution found and its fitness
 */
ResultMH GreedySearch::optimize(Problem *problem, int maxevals) {
  assert(maxevals > 0);
  vector<tOption> values;
  Problem1 *realproblem = dynamic_cast<Problem1 *>(problem);

  int tam = realproblem->GetTam();
  vector<vector<double>> mapa = realproblem->GetMatriz(); 
  int num_nodos = realproblem->GetCantNodos();

  int primero=Random::get(0, num_nodos-1);
  tSolution sol;
  //Metemos un nodo aleatoriamente a la solución
  sol.push_back(primero);
  SolutionInfoP1 informacion(sol,problem);
  while(sol.size()<tam){
    double minDispersion = numeric_limits<double>::max();
    int mejorElemento = -1;
    for(unsigned i=0;i<num_nodos;i++){
      if (find(sol.begin(), sol.end(), i) == sol.end()) {
        // Calculamos la dispersión de la posible eleccion
        double dispersion = Heuristica(informacion,i,sol);
        // Si la dispersión es menor, actualizar el mejor elemento
        if (dispersion < minDispersion) {
            minDispersion = dispersion;
            mejorElemento = i;
        }
      }
    }
    sol.push_back(mejorElemento);
    informacion.ActualizarGreedy(mejorElemento,sol);
  }
  tFitness fitness = realproblem->fitness(sol);
  return ResultMH(sol, fitness, 1);
}
