#include <mh.h>
#include <vector>
#include "problem1.h"
#include "solutionInfo.h"
#include "busquedalocal.h"
#include "utilidadesgeneticos.h"

// Implement the Memetico class
/**
 * Implementation of the Memetic Search metaheuristic
 *  - Randomly generates a population and uses AGG and LocalSearch to find a decent solution
 */

class Memetico : public MH
{

  //Population size
  const int TAM_POB = 50;
  //Cross Probability
  const double PC = 0.7;
  //Mutation Probability
  const double PM = 0.1;
  BusquedaLocal *rbl;
  //Version of the memetic
  //1-AM(10,1)
  //2-AM(10,0.1)
  //3-AM(10,0.1mej)
  int versionAM;

public:
  /**
   * Create a Memetico object
   *
   * @param version The AM version
   */
  Memetico(int version)
  {
    rbl = new BusquedaLocal(false);
    versionAM = version;
  }
  virtual ~Memetico()
  {
    delete rbl;
  }
  /**
   * Applies LocalSearch to a solution given and changes it to the found solution
   *
   * @param problem The problem to be optimized
   * @param sol The solution given
   * @param fitness The fitness of the solution given
   * @param maxevals Maximum number of evaluations allowed in the LocalSearch
   * @param eval_loc Evals in the AM
   * @return The fitness of the new Solution
   */
  double aplicarBusquedaLocal(Problem *problem, tSolution &sol, tFitness fitness, int max_evals, int &eval_loc);
  // Implement the MH interface methods
  /**
   * Randomly generates a population and uses AGG and LocalSearch to find a decent solution
   * returning the best one.
   *
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */
  ResultMH optimize(Problem *problem, const int maxevals) override;
};