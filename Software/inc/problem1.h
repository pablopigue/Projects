#pragma once
#include <algorithm>
#include <problem.h>
#include <vector>
#include <solutionInfo.h>
using namespace std;

class Problem1 : public Problem {
  tSolution solucion;
  int tam;
  int num_nodos;
  vector<vector<double>> distancias;


public:
  Problem1(vector<std::vector<double>>& matriz,int num_nod, int k) : Problem() { distancias=matriz; tam=k; num_nodos=num_nod; }
  // Implement the MH interface methods
  /**
   * Do the calcs to get the fitness of a solution
   *
   * @param solution The solution
   * @return the fitness of the solution
   */
  tFitness fitness(const tSolution &solution) override;
  tFitness fitnessBinary(const tSolution &solution) ;
  // Implement the MH interface methods
  /**
   * Do the calcs to get the fitness of the change of a solution using factorization
   *
   * @param solution The solution
   * @param solution_info The solution information
   * @param pos_change The position of the element we want to change
   * @param new_value The value we want to take into account
   * @return the fitness of the solution
   */
  tFitness fitness(const tSolution &solution,SolutionFactoringInfo *solution_info,unsigned pos_change, tDomain new_value) override;
  /**
   * Creates a Random Solution
   *
   * @return a random Solution
   */
  tSolution createSolution() override;

  tSolution createSolutionBinary();
  //Getters
  size_t getSolutionSize() override { return tam; }
  SolutionFactoringInfo *generateFactoringInfo(const tSolution &solution) override;
  std::pair<tDomain, tDomain> getSolutionDomainRange() override;

  const vector<vector<double>> &GetMatriz() const{
    return distancias;
  }
  const int GetTam() const{
    return tam;
  }
  const int GetCantNodos() const{
    return num_nodos;
  }
};
