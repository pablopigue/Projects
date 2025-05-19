#include <algorithm>
#include <problem1.h>
#include <random.hpp>
#include <iostream>

SolutionFactoringInfo * Problem1::generateFactoringInfo(const tSolution &solution){
  SolutionInfoP1* nueva_info = new SolutionInfoP1(solution, this); // Dynamically allocate
  
  return nueva_info; // Return as a pointer
}

tFitness Problem1::fitness(const tSolution &solution) {
  tFitness maxSum = std::numeric_limits<tFitness>::min();
  tFitness minSum = std::numeric_limits<tFitness>::max();

  for (int i = 0; i < solution.size(); ++i) {
    tFitness sum = 0;
    
    // Calcular la suma de distancias del nodo i con respecto a los demás nodos seleccionados
    for (int j = 0; j < solution.size(); ++j) {
        sum += distancias[solution[i]][solution[j]]; // Sumar la distancia entre el nodo i y el nodo j
    }
    // Actualizar la máxima y mínima suma
    if (sum > maxSum) {
        maxSum = sum;
    }
    if (sum < minSum) {
        minSum = sum;
    }
  }

  tFitness resultado = maxSum - minSum;
  return resultado;
}

tFitness Problem1::fitnessBinary(const tSolution &solution){
  tSolution sol;

  for(int i=0; i < solution.size();i++){
    if(solution[i]==1){
      sol.push_back(i);
    }
  }

  return fitness(sol);
}

tFitness Problem1::fitness(const tSolution &solution,SolutionFactoringInfo *informat,unsigned pos_change, tDomain new_value){
  tFitness fitness=0;
  vector<double> copia;
  tFitness maxSum = std::numeric_limits<tFitness>::min();
  tFitness minSum = std::numeric_limits<tFitness>::max();

  SolutionInfoP1* info = dynamic_cast<SolutionInfoP1*>(informat);
  if (!info) {
      throw std::runtime_error("Invalid type for SolutionFactoringInfo");
  }




  double modif=0;
  for(unsigned i=0;i<info->columna_fact.size();i++){
    modif=0;
    modif = info->columna_fact[i]-info->matriz[solution[i]][solution[pos_change]];
    modif += info->matriz[new_value][solution[i]];
    copia.push_back(modif);
  }


  double disp =0;
  for(unsigned i=0;i<solution.size();i++){
    if(i!=pos_change){
      disp+=info->matriz[new_value][solution[i]];
    }
  }


  copia[pos_change]=disp;    


  for(unsigned i=0; i<copia.size();i++){
    if (copia[i] > maxSum) {
      maxSum = copia[i];
    }
    if (copia[i] < minSum) {
      minSum = copia[i];
    }
  }
  
  fitness = maxSum - minSum;
  return fitness;
}

tSolution Problem1::createSolution() {
  tSolution solution(tam);
  vector<int> aux;
  //Genero un vector con todos los nodos sin repeticiones
  for(int i=0;i<num_nodos;i++){
    aux.push_back(i);
  }
  //mezclo los nodos aleatoriamente
  Random::shuffle(aux);

  //Escojo los tam primeros
  solution.assign(aux.begin(), aux.begin() + tam);
  return solution;
}

tSolution Problem1::createSolutionBinary() {
  tSolution solution(num_nodos); 
  vector<int> indices(num_nodos); 


  // Llenar con índices de 0 a tam-1
  for (int i = 0; i < num_nodos; ++i) {
      indices[i] = i;
  }

  // Mezclar aleatoriamente los índices
  Random::shuffle(indices);

  
  for (int i = 0; i < tam; ++i) {
      solution[indices[i]] = 1; // Asignar 1 en posición aleatoria
  }

  return solution;
}

pair<tDomain, tDomain> Problem1::getSolutionDomainRange() {
    return std::make_pair(0, num_nodos - 1); // Ajusta esto según tu problema
}
