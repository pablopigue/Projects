#include <mh.h>
#include <vector>
#include "problem1.h"
#include "solutionInfo.h"
#include "busquedalocal.h"
#include "enfriamientoSimulado.h"

class Ils : public MH
{

  int iteraciones;
  bool type;
  const double MUT=0.2;
  BusquedaLocal *rbl;
  EnfriamientoSimulado *es;

public:
  /**
   * Create a Memetico object
   *
   * @param version The AM version
   */
  Ils(int iters,bool tipo)
  {
    type=tipo;
    iteraciones = iters;
    rbl = new BusquedaLocal(false);
    es = new EnfriamientoSimulado();
  }
  virtual ~Ils()
  {
    delete rbl;
  }

  tSolution mutar(const tSolution &sol,Problem *problem);
  // Implement the MH interface methods
  /**
   * Dies a number of LocalSearchs keeping the best solutionfound
   *
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */
  ResultMH optimize(Problem *problem, const int maxevals) override;
};