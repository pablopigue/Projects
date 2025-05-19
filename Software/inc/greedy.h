#pragma once
#include <mh.h>
#include "solutionInfo.h"

using namespace std;

/**
 * Implementation of the Random Search metaheuristic
 *  - Randomly generates solutions and selects the best one
 *
 * @see MH
 * @see Problem
 */
class GreedySearch : public MH {
  
public:
  GreedySearch() : MH() {}
  virtual ~GreedySearch() {}
  /**
   * Compute the necessary calcs to get the fitness of a partial solution
   * 
   *
   * @param info The information of the solution
   * @param elemento The element we want to check in the solution
   * @param solucion The solution we are working with
   * @return The fitness of the partial solution
   */

  tFitness Heuristica(const SolutionInfoP1 &info,unsigned elemento,tSolution const &solucion);
  
  // Implement the MH interface methods
  /**
   * Create a solution taking in each step the most aproppiate candidate
   * 
   *
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */
  ResultMH optimize(Problem *problem, int maxevals) override;
};
