#include "geneticos.h"
#include <cassert>
#include <iostream>
#include <util.h>
#include <random.hpp>
#include "problem1.h"

using namespace std;


ResultMH Geneticos::optimize(Problem *problem, const int maxevals)
{
  vector<tSolution> poblacion = GUtils::CreateFamily(*problem,TAM_POB);

  int tam_sol = problem->getSolutionSize();
  int tam_max = problem->getSolutionDomainRange().second+1;

  vector<double> fitness(TAM_POB);
  int evaluaciones = 0;

  // Evaluar población inicial
  for (int i = 0; i < TAM_POB; ++i)
  {
    fitness[i] = problem->fitness(poblacion[i]);
    evaluaciones++;
  }

  if (tipoGenetico)
  {
    while (evaluaciones < maxevals)
    {

      // Selección de padres por torneo SOLO COGEMOS 2
      vector<tSolution> padres;
      for (int i = 0; i < 2; ++i)
        padres.push_back(poblacion[GUtils::torneo(fitness)]);

      vector<tSolution> hijos;

      // Procesar las parejas
      tSolution h1, h2;

      std::pair<tSolution, tSolution> hijos_pair;
      if (tipoCruce)
      {
        hijos_pair = GUtils::cruceUniforme(padres[0], padres[1], tam_sol,tam_max,problem);
      }
      else
      {
        hijos_pair = GUtils::cruceIntercambio(padres[0], padres[1], tam_sol,tam_max);
      }
      h1 = hijos_pair.first;
      h2 = hijos_pair.second;

      hijos.push_back(h1);
      hijos.push_back(h2);

      // Aplicar mutación a la población de hijos
      GUtils::mutarPoblacion(hijos, PM, tam_sol,tipoGenetico,tam_max);

      // Evaluar hijos
      vector<double> fitHijos(2);
      for (int i = 0; i < 2; ++i)
      {
        fitHijos[i] = problem->fitness(hijos[i]);
        evaluaciones++;
      }

      // Remplazo
      int peorIdx = max_element(fitness.begin(), fitness.end()) - fitness.begin();
      double fitnes_peor_Padre = fitness[peorIdx];

      int mejorHijo = min_element(fitHijos.begin(), fitHijos.end()) - fitHijos.begin();
      double fitness_mejor_hijo = fitHijos[mejorHijo];
      
      if (fitness_mejor_hijo < fitnes_peor_Padre)
      {

        poblacion[peorIdx] = hijos[mejorHijo];
        fitness[peorIdx] = fitHijos[mejorHijo];
      }
    }
  }
  else
  {
    while (evaluaciones < maxevals)
    {
      // Selección de padres por torneo
      vector<tSolution> padres;
      for (int i = 0; i < TAM_POB; ++i)
        padres.push_back(poblacion[GUtils::torneo(fitness)]);
      
      vector<tSolution> hijos;
      // Calcular número esperado de cruces
      int num_cruces = ceil(PC * (TAM_POB / 2.0));

      // Procesar las parejas
      for (int i = 0; i < TAM_POB; i += 2)
      {
        tSolution h1, h2;

        // Las primeras num_cruces parejas cruzan, el resto no
        if (i / 2 < num_cruces)
        {
          std::pair<tSolution, tSolution> hijos_pair;
          if (tipoCruce)
          {
            hijos_pair = GUtils::cruceUniforme(padres[i], padres[i + 1], tam_sol,tam_max,problem);
          }
          else
          {
            hijos_pair = GUtils::cruceIntercambio(padres[i], padres[i + 1], tam_sol,tam_max);
          }
          h1 = hijos_pair.first;
          h2 = hijos_pair.second;
        }
        else
        {
          h1 = padres[i];
          h2 = padres[i + 1];
        }

        hijos.push_back(h1);
        hijos.push_back(h2);
      }

      // Aplicar mutación a la población de hijos
      GUtils::mutarPoblacion(hijos, PM, tam_sol,tipoGenetico,tam_max);

      // Evaluar hijos
      vector<double> fitHijos(TAM_POB);
      for (int i = 0; i < TAM_POB; ++i)
      {
        fitHijos[i] = problem->fitness(hijos[i]);
        evaluaciones++;
      }

      // Elitismo
      int mejorIdx = min_element(fitness.begin(), fitness.end()) - fitness.begin();
      double mejorFit = fitness[mejorIdx];
      tSolution mejorSol = poblacion[mejorIdx];

      poblacion = hijos;
      fitness = fitHijos;

      int mejorIdxNuevaPob = min_element(fitness.begin(), fitness.end()) - fitness.begin();
      double mejorFitNuevo = fitness[mejorIdx];

      if(mejorFitNuevo>mejorFit){
          int peorIdx = max_element(fitness.begin(), fitness.end()) - fitness.begin();
          poblacion[peorIdx] = mejorSol;
          fitness[peorIdx] = mejorFit;
      }
      
    }
  }

  int mejorIdx = min_element(fitness.begin(), fitness.end()) - fitness.begin();

  return ResultMH(poblacion[mejorIdx], fitness[mejorIdx], evaluaciones);
}