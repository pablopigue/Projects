#include "bmb.h"

//REVISAR PORQUE NO SE SI ESTO ES LO QUE QUIERE
ResultMH Bmb::optimize(Problem *problem, const int maxevals)
{
    int evals = (maxevals/iteraciones);

    int evaluaciones_locales=0;

    tFitness min = MAXFLOAT;
    tSolution sol_final;

    for(int i=0;i<iteraciones;i++){
        tSolution sol = problem->createSolution();
        tFitness fit=problem->fitness(sol);
        ResultMH aux =rbl->optimize(problem,sol,fit,evals);

        evaluaciones_locales+= aux.evaluations;
        
        if(aux.fitness<min){
            sol_final=aux.solution;
            min=aux.fitness;
        }
    }

    ResultMH resultado(sol_final,min,evaluaciones_locales);

    return resultado;
}