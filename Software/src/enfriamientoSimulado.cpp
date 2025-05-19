#include "enfriamientoSimulado.h"

vector<pair<int,int>> EnfriamientoSimulado::crearVecindario(Problem *problem,tSolution ini){
    tSolution no_seleccionados;
    vector<pair<int, int>> vecindario;

    for (int i = 0; i < problem->getSolutionDomainRange().second+1; ++i)
    {
        if (find(ini.begin(), ini.end(), i) == ini.end())
        {
            no_seleccionados.push_back(i);
        }
    }

    // Generación de todos posibles cambios
    for (unsigned i = 0; i < ini.size(); i++)
    {
        for (int elem : no_seleccionados)
        {
            pair<int, int> pareja = make_pair(i, elem);
            vecindario.push_back(pareja);
        }
    }

    Random::shuffle(vecindario);  

    return vecindario;
}
//MAL IMPLEMENTADO
ResultMH EnfriamientoSimulado::optimize(Problem *problem, const tSolution &current,tFitness fit, int maxevals)
{

    int evaluaciones = 0;
    tSolution sol(current);
    tFitness solFitness = fit;

    tSolution bestSol(sol);
    tFitness fitness_best_sol=solFitness;
    evaluaciones++;
    
    SolutionInfoP1 informacion(sol, problem);

    double t0 = (MU * fitness_best_sol) / (-log(PHI));
    double t = t0;

    if (TF >= t0)
    {
        return ResultMH(bestSol,fitness_best_sol,evaluaciones);
        //throw invalid_argument("TF debe ser menor que t0");
    }

    int m = problem->getSolutionSize();
    int max_vecinos = 100 * m;
    int max_exitos = 0.1 * max_vecinos;
    int M = maxevals / max_vecinos;

    double beta = (t0 - TF) / (M * t0 * TF);

    int exitos = 1;
    int vecinos_generados=0;

    while (evaluaciones < maxevals && exitos > 0)
    {
        exitos = 0;
        vecinos_generados = 0;

        vector<pair<int, int>> vecindario = crearVecindario(problem,sol);

        int indice_aux = 0;

        while (vecinos_generados < max_vecinos && exitos < max_exitos && evaluaciones < maxevals && indice_aux < vecindario.size())
        {
            double nueva_dispersion = problem->fitness(sol, static_cast<SolutionFactoringInfo *>(&informacion), vecindario[indice_aux].first, vecindario[indice_aux].second);

            vecinos_generados++;          
            evaluaciones++;
            
            double deltaf=nueva_dispersion-solFitness;
            //cout << "delta f " << deltaf << " indiceaux "<<indice_aux  << endl;
            if ((deltaf < 0) || Random::get<double>(0,1)<=exp(-deltaf/t))
            {
                // Actualizamos la informacion de la solucion y la solucion
                
                informacion.ActualizarBusquedaLocal(vecindario[indice_aux].first, vecindario[indice_aux].second, sol);
                sol[vecindario[indice_aux].first] = vecindario[indice_aux].second;
                solFitness = nueva_dispersion;
                exitos++;
                
                //cout << "hay exito "<<  exitos << " maxexitos " << max_exitos<<endl;
                if(nueva_dispersion<fitness_best_sol){
                    bestSol=sol;
                    fitness_best_sol=nueva_dispersion;
                }
            }

            indice_aux++;
        }

        t=t/(1+beta*t);
    }

    return ResultMH(bestSol,fitness_best_sol,evaluaciones);
}