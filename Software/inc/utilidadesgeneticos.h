#pragma once

#include "problem1.h"
#include "random.hpp"
#include <vector>
#include <algorithm>
#include <utility>
#include <numeric> // Para count

using std::pair;
using std::vector;

/**
 * Class with the necessary functions to compute all that genetic algorithms need
 *
 *
 */

class GUtils
{
public:
  /**
   * Creates a Family with size tam_pob
   *
   *
   * @param tam_pob The size of the populition that must be created
   * @param problem The problem we are working on
   * @return A family of Solutions (vector<tSolution>)
   */
  static vector<tSolution> CreateFamily(Problem &problem, int tam_pob);

  /**
   * Randomly select 3 members of the vector fitness and gives the best index between the 3
   *
   *
   * @param fitness The fitness of the members of the population
   * @return The index of the best element chosen
   */
  static int torneo(const vector<double> &fitness);

  /**
   * Heuristic fix of solutions used in uniform cross
   * Given a solution it adapts it to have k ones
   *
   * @param k The number of ones the solution must have (the number of elements of the solution)
   * @param solucion The Solution we are working on
   * @param problem The problem we are working on
   * @return The fitness of the partial solution
   */
  static void reparar(tSolution &solucion, int k, Problem *problem);

  /**
   * Uniform cross
   * Shifts the solutions to binary representation and cross them keeping the common parts and randomly assigning the
   * other positions using later reparar to assure it is a valid solution
   *
   * @param p1 The first solution we want to cross
   * @param p2 The second solution we want to cross
   * @param k The number of ones the solution must have
   * @param tam_max The max size of our dominion
   * @param problem The problem we are working on
   * @return A pair containing the 2 children result of the cross in int representation
   */
  static pair<tSolution, tSolution> cruceUniforme(const tSolution &p1, const tSolution &p2, int k, int tam_max, Problem *problem);

  /**
   * Position cross
   * Shifts the solutions to binary representation and cross them keeping the common parts and randomly assigning the
   * other positions assuring that it is a valid solution
   *
   * @param p1 The first solution we want to cross
   * @param p2 The second solution we want to cross
   * @param k The number of ones the solution must have
   * @param tam_max The max size of our dominion
   * @return A pair containing the 2 children result of the cross in int representation
   */
  static pair<tSolution, tSolution> cruceIntercambio(const tSolution &p1, const tSolution &p2, int k, int tam_max);

  /**
   * Mutate the with probabolity PM the solutions in poblacion
   * which is changing one node to another not already in the solution
   *
   * @param poblacion The Family we are working on in the AG
   * @param PM The Mutation Probability
   * @param tam_sol The solution size
   * @param tipoGenetico The type of AG we are using
   * @param tam_max The max size of our dominion
   */
  static void mutarPoblacion(vector<tSolution> &poblacion, double PM, int tam_sol, bool tipoGenetico, int tam_max);

  /**
   * Shifts a solution in binary representation to int representation
   *
   *
   * @param binary The Solution in binary representation
   */
  static tSolution binaryToIndices(const tSolution &binary);

  /**
   * Shifts a solution in int representation to binary representation
   *
   *
   * @param binary The Solution in binary representation
   */
  static tSolution indicesToBinary(const tSolution &ind, int tam_max);
};
