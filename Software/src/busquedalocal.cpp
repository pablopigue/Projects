#include "busquedalocal.h"
#include <cassert>
#include <iostream>
#include <util.h>
#include "problem1.h"

using namespace std;
tFitness BusquedaLocal::HeuristicaBLocal(const SolutionInfoP1 &info, int pos_change, int new_value, tSolution const &solucion)
{
  tFitness fitness = 0;
  vector<double> copia;
  tFitness maxSum = std::numeric_limits<tFitness>::min();
  tFitness minSum = std::numeric_limits<tFitness>::max();
  // Calculamos el nuevo vector de suma de distancias
  for (unsigned i = 0; i < info.columna_fact.size(); i++)
  {
    double modif = info.columna_fact[i] - info.matriz[solucion[i]][solucion[pos_change]];
    modif += info.matriz[new_value][solucion[i]];
    copia.push_back(modif);
  }

  double disp = 0;
  for (unsigned i = 0; i < solucion.size(); i++)
  {
    if (i != pos_change)
    {
      disp += info.matriz[new_value][solucion[i]];
    }
  }
  copia[pos_change] = disp;
  // Calculamos su fitness
  for (unsigned i = 0; i < copia.size(); i++)
  {
    if (copia[i] > maxSum)
    {
      maxSum = copia[i];
    }
    if (copia[i] < minSum)
    {
      minSum = copia[i];
    }
  }

  fitness = maxSum - minSum;
  return fitness;
}

ResultMH BusquedaLocal::optimize(Problem *problem,  const tSolution &current,tFitness fitness, int maxevals)
{
  assert(maxevals > 0);

  tSolution sol = current;
  int num_nodos = problem->getSolutionDomainRange().second+1;
  SolutionInfoP1 informacion(sol, problem);

  tSolution no_seleccionados;
  vector<pair<pair<int, int>, tFitness>> movimientos_con_fitness;

  tFitness dispersion_actual = fitness;
  bool mejora;
  int evaluaciones = 0;

  do
  {
    mejora = false;

    for (int i = 0; i < num_nodos; ++i)
    {
      if (find(sol.begin(), sol.end(), i) == sol.end())
      {
        no_seleccionados.push_back(i);
      }
    }

    // Generación de todos posibles cambios
    for (unsigned i = 0; i < sol.size(); i++)
    {
      for (int elem : no_seleccionados)
      {
        pair<int, int> pareja = make_pair(i, elem);
        // Si es búsqueda aleatoria solo generamos los vecinos sin calcular el fitness
        pair<pair<int, int>, tFitness> pareja_fitness = make_pair(pareja, 0);
        if (tipo)
        {
          // si es búsqueda heuristica generamos los vecinos y su fitness para ordenar por el
          pareja_fitness = make_pair(pareja, HeuristicaBLocal(informacion, i, elem, sol));
        }
        movimientos_con_fitness.push_back(pareja_fitness);
      }
    }

    if (tipo)
    {
      // Ordenar movimientos según la función de Heuristica
      sort(movimientos_con_fitness.begin(), movimientos_con_fitness.end(),
           [](const pair<pair<int, int>, tFitness> &a, const pair<pair<int, int>, tFitness> &b)
           {
             return a.second < b.second;
           });
    }
    else
    {
      // Orden aleatorio
      Random::shuffle(movimientos_con_fitness);
    }
    // Explorar el entorno
    for (unsigned i = 0; i < movimientos_con_fitness.size(); ++i)
    {
      tFitness nueva_dispersion = 0;
      if (tipo)
      {
        // Si es búsqueda heurística ya hemos calculado los fitness asi que solo habría que tomarlo
        nueva_dispersion = movimientos_con_fitness[i].second;
      }
      else
      {
        // Si es búsqueda aleatoria calculamos el fitness del intercambio
        nueva_dispersion = problem->fitness(sol, static_cast<SolutionFactoringInfo *>(&informacion), movimientos_con_fitness[i].first.first, movimientos_con_fitness[i].first.second);
      }
      evaluaciones++;
      if (nueva_dispersion < dispersion_actual)
      {
        // Actualizamos la informacion de la solucion y la solucion
        informacion.ActualizarBusquedaLocal(movimientos_con_fitness[i].first.first, movimientos_con_fitness[i].first.second, sol);
        sol[movimientos_con_fitness[i].first.first] = movimientos_con_fitness[i].first.second;
        dispersion_actual = nueva_dispersion;
        mejora = true;
        break; // Primero Mejor: salir tras encontrar una mejora
      }
      if (evaluaciones >= maxevals)
        break;
    }
    no_seleccionados.clear();
    movimientos_con_fitness.clear();
  } while (mejora && evaluaciones < maxevals);

  return ResultMH(sol, dispersion_actual, evaluaciones);
}
