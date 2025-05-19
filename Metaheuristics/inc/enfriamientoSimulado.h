#pragma once
#include <mh.h>
#include <vector>
#include "problem1.h"
#include "solutionInfo.h"
#include "busquedalocal.h"

class EnfriamientoSimulado : public MHTrayectory
{

  int iteraciones;
  const double MU=0.2;
  const double PHI=0.3;
  const double TF= pow(10,-3);
  BusquedaLocal *rbl;

public:
  /**
   * Create a Memetico object
   *
   * @param version The AM version
   */
  EnfriamientoSimulado()
  {
    rbl = new BusquedaLocal(false);
  }
  virtual ~EnfriamientoSimulado()
  {
    delete rbl;
  }

  // Implement the MH interface methods
  /**
   * Dies a number of LocalSearchs keeping the best solutionfound
   *
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */
  ResultMH optimize(Problem *problem, const tSolution &current,tFitness fitness, int maxevals) override;

  vector<pair<int,int>> crearVecindario(Problem *problem,tSolution ini);
};