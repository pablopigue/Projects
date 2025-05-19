#include "memetico.h"
#include <cassert>
#include <iostream>
#include <util.h>
#include <random.hpp>
#include "problem1.h"

using namespace std;

// Función para ordenar la población por fitness (de mejor a peor)
void ordenarPoblacionPorFitness(vector<tSolution>& poblacion, vector<double>& fitness) {
    // Crear un vector de índices
    vector<size_t> indices(poblacion.size());
    for (size_t i = 0; i < indices.size(); ++i) {
        indices[i] = i;
    }

    // Ordenar los índices basados en el fitness
    sort(indices.begin(), indices.end(), [&fitness](size_t a, size_t b) {
        return fitness[a] < fitness[b];
    });

    // Aplicar el ordenamiento a ambas estructuras
    vector<tSolution> poblacion_ordenada;
    vector<double> fitness_ordenado;
    
    for (size_t i : indices) {
        poblacion_ordenada.push_back(poblacion[i]);
        fitness_ordenado.push_back(fitness[i]);
    }

    poblacion = move(poblacion_ordenada);
    fitness = move(fitness_ordenado);
}

// Función para aplicar búsqueda local a una solución
double Memetico::aplicarBusquedaLocal(Problem* problem,tSolution& Sol,tFitness fitness, int max_evals,int &eval_loc) {

    ResultMH result=rbl->optimize(problem,Sol,fitness,max_evals);

    Sol=result.solution;

    eval_loc+=result.evaluations;

    return result.fitness;
}

ResultMH Memetico::optimize(Problem *problem, const int maxevals)
{
    vector<tSolution> poblacion = GUtils::CreateFamily(*problem,TAM_POB);

    int tam_sol = problem->getSolutionSize();
    int tam_max = problem->getSolutionDomainRange().second+1;

    vector<double> fitness(TAM_POB);
    int evaluaciones = 0;
    int generacion = 0;

    // Evaluar población inicial
    for (int i = 0; i < TAM_POB; ++i)
    {
        fitness[i] = problem->fitness(poblacion[i]);
        evaluaciones++;
    }

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
                hijos_pair = GUtils::cruceUniforme(padres[i], padres[i + 1], tam_sol,tam_max,problem);

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
        GUtils::mutarPoblacion(hijos, PM, tam_sol,0,tam_max);

        // Evaluar hijos
        vector<double> fitHijos(TAM_POB);
        for (int i = 0; i < TAM_POB; ++i)
        {
            fitHijos[i] = problem->fitness(hijos[i]);
            evaluaciones++;
        }
        // Elitismo: mantener el mejor si no sobrevive
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
        
        generacion++;

        
        // Aplicar búsqueda local según la versión del AM
        if (generacion % 10 == 0) { // Cada 10 generaciones
            switch (versionAM) {
                case 1: // AM-(10,1.0)
                    for (int i = 0; i < TAM_POB && evaluaciones < maxevals; ++i) {
                        int evals_a_hacer=min(400,maxevals-evaluaciones);
                        double fitnesscalculado=aplicarBusquedaLocal(problem,poblacion[i],fitness[i],evals_a_hacer,evaluaciones);
                        fitness[i] = fitnesscalculado;
                    }
                    break;
                    
                case 2: // AM-(10,0.1)
                    for (int i = 0; i < TAM_POB && evaluaciones < maxevals; ++i) {
                        if (Random::get<double>(0, 1) < 0.1) { // Probabilidad 0.1
                            int evals_a_hacer=min(400,maxevals-evaluaciones);
                            double fitnesscalculado=aplicarBusquedaLocal(problem,poblacion[i],fitness[i],evals_a_hacer,evaluaciones);
                            fitness[i] = fitnesscalculado;
                        }
                    }
                    break;
                    
                case 3: // AM-(10,0.1mej)
                    ordenarPoblacionPorFitness(poblacion, fitness);
                    int numBL = static_cast<int>(0.1 * TAM_POB);
                    for (int i = 0; i < numBL; ++i) {
                        if(evaluaciones < maxevals){
                            int evals_a_hacer=min(400,maxevals-evaluaciones);
                            double fitnesscalculado=aplicarBusquedaLocal(problem,poblacion[i],fitness[i],evals_a_hacer,evaluaciones);
                            fitness[i] = fitnesscalculado;
                        }
                    }
                    break;
            }
        }
            

    }

    int mejorIdx = min_element(fitness.begin(), fitness.end()) - fitness.begin();

    return ResultMH(poblacion[mejorIdx], fitness[mejorIdx], evaluaciones);
}