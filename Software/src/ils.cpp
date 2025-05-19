#include "ils.h"

tSolution Ils::mutar(const tSolution &sol,Problem *problem){
    tSolution copia(sol);
    int aux = copia.size() * MUT;
    int tam_mut= max(aux,2);

    int posibles_nodos= problem->getSolutionDomainRange().second+1;

    // Paso 1: Seleccionar índices aleatorios para mutar, sin repetir
    vector<int> indices_a_quitar;
    while (indices_a_quitar.size() < tam_mut) {
        int idx = rand() % copia.size();
        if (find(indices_a_quitar.begin(), indices_a_quitar.end(), idx) == indices_a_quitar.end()) {
            indices_a_quitar.push_back(idx);
        }
    }

    // Paso 2: Guardar nodos a quitar
    vector<int> nodos_a_quitar;
    for (int idx : indices_a_quitar) {
        nodos_a_quitar.push_back(copia[idx]);
    }

    // Paso 3: Construir nodos_en_sol (lo que ya está en la solución)
    vector<int> nodos_en_sol = copia;

    // Paso 4: Construir lista de nodos posibles que:
    // - No estén ya en la solución
    // - No estén entre los que se van a quitar
    vector<int> nodos_posibles;
    for (int i = 0; i < posibles_nodos; ++i) {
        if (find(nodos_en_sol.begin(), nodos_en_sol.end(), i) == nodos_en_sol.end() &&
            find(nodos_a_quitar.begin(), nodos_a_quitar.end(), i) == nodos_a_quitar.end()) {
            nodos_posibles.push_back(i);
        }
    }

    // Barajar nodos posibles
    Random::shuffle(nodos_posibles);

    // Paso 5: Reemplazar nodos seleccionados en copia
    for (int i = 0; i < indices_a_quitar.size(); ++i) {
        if (i < nodos_posibles.size()) {
            copia[indices_a_quitar[i]] = nodos_posibles[i];
        }
    }

    return copia;

}

//REVISAR PORQUE NO SE SI ESTO ES LO QUE QUIERE
ResultMH Ils::optimize(Problem *problem, const int maxevals)
{

    int evals = (maxevals/iteraciones);
    int evaluaciones_locales=0;
    
    tSolution sol = problem->createSolution();
    tFitness fit=problem->fitness(sol);
    pair<tSolution, double> best_sol(sol,fit);

    for(int i=0;i<iteraciones-1;i++){
        tSolution sol_mutada = mutar(best_sol.first,problem);
        ResultMH aux(sol_mutada,fit,evals);
        if(type){
            aux =es->optimize(problem,sol_mutada,fit,evals);
        }else{
            aux =rbl->optimize(problem,sol_mutada,fit,evals);
        }
        
        evaluaciones_locales+= aux.evaluations;
        if(aux.fitness<best_sol.second){
            best_sol.first=aux.solution;
            best_sol.second=aux.fitness;
        }
    }

    ResultMH resultado(best_sol.first,best_sol.second,evaluaciones_locales);

    return resultado;
}