#include <mhtrayectory.h>
#include <vector>
#include "solutionInfo.h"
#ifndef BUSQUEDALOCAL_H
#define BUSQUEDALOCAL_H

class BusquedaLocal : public MHTrayectory {
bool tipo;
public:
  BusquedaLocal(bool type){
    tipo=type;
  }
  virtual ~BusquedaLocal() {tipo=true;}
  // Implement the MH interface methods
  /**
   * Create a solution by using LocalSearch (Random and heuristic)
   * which consist in taking a rand sol and then making changes to get betters sols
   *
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */

  ResultMH optimize(Problem *problem, const tSolution &current,tFitness fitness, int maxevals) override;
    /**
   * Compute the necessary calcs to get the fitness of a change in a solution
   * 
   *
   * @param info The information of the solution
   * @param i The position of the element we want to change
   * @param j The element that is going to take the place in i
   * @param solucion The solution we are working with
   * @return The fitness of the new solution
   */
  tFitness HeuristicaBLocal(const SolutionInfoP1 &info,int i,int j,tSolution const &solucion);
};
#endif // BUSQUEDALOCAL_H