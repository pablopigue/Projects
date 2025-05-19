#include <cassert>
#include <randomsearch.h>

using namespace std;

/**
 * Create random solutions until maxevals has been achieved, and returns the
 * best one.
 *
 * @param problem The problem to be optimized
 * @param maxevals Maximum number of evaluations allowed
 * @return A pair containing the best solution found and its fitness
 */
ResultMH RandomSearch::optimize(Problem *problem, int maxevals) {
  assert(maxevals > 0);
  tSolution best;
  float best_fitness = -1;

  for (int i = 0; i < maxevals; i++) {
    //Creamos soluciones aleatorias
    tSolution solution = problem->createSolution();
    float fitness = problem->fitness(solution);
    //Evaluamos si es mejor a la que ya tenemos
    if (fitness < best_fitness || best_fitness < 0) {
      best = solution;
      best_fitness = fitness;
    }
  }

  return ResultMH(best, best_fitness, maxevals);
}
