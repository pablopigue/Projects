#include <mh.h>
#include <vector>
#include "problem1.h"
#include "solutionInfo.h"
#include "busquedalocal.h"

class Bmb : public MH
{

  int iteraciones;
  BusquedaLocal *rbl;

public:
  /**
   * Create a Memetico object
   *
   * @param version The AM version
   */
  Bmb(int iters)
  {
    iteraciones = iters;
    rbl = new BusquedaLocal(false);
  }
  virtual ~Bmb()
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
  ResultMH optimize(Problem *problem, const int maxevals) override;
};