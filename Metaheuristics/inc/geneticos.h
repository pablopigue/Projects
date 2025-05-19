#include <mh.h>
#include <vector>
#include "problem1.h"
#include "solutionInfo.h"
#include "utilidadesgeneticos.h"

/**
 * Class Geneticos which implements AGG,AGE
 * Creates a Randomly generated population that changes during the algorithm
 * shifting into better solutions
 */
class Geneticos : public MH
{
  // Cross type
  // True-Uniform cross
  // False-Position cross
  bool tipoCruce;
  // Genetic type
  // True-Estacionary
  // False-Generational
  bool tipoGenetico;
  // Population size
  const int TAM_POB = 50;
  // Cross Probability
  const double PC = 0.7;
  // Mutation Probability
  const double PM = 0.1;

public:
  Geneticos(bool type, bool type2)
  {
    tipoCruce = type;
    tipoGenetico = type2;
  }
  virtual ~Geneticos() { tipoCruce = true; }
  // Implement the MH interface methods
  /**
   * Creates a Randomly generated population that changes during the algorithm
   * shifting into better solutions
   *
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */
  ResultMH optimize(Problem *problem, const int maxevals) override;
};