#include <grasp.h>

tFitness Grasp::Heuristica(const SolutionInfoP1 &info, unsigned elemento, tSolution const &solucion)
{
    tFitness fitness = 0;
    vector<double> copia;
    tFitness maxSum = std::numeric_limits<tFitness>::min();
    tFitness minSum = std::numeric_limits<tFitness>::max();
    // Calculamos una copia de columna_fact con la adición de distancias de nuevo elemento
    for (unsigned i = 0; i < info.columna_fact.size(); i++)
    {
        double modif = info.columna_fact[i] + info.matriz[solucion[i]][elemento];
        copia.push_back(modif);
    }
    // Calculamos la suma de distancias de nuevo elemento
    double disp = 0;
    for (unsigned i = 0; i < solucion.size(); i++)
    {
        disp += info.matriz[elemento][solucion[i]];
    }
    copia.push_back(disp);

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
    // Calculamos la dispersion
    fitness = maxSum - minSum;
    return fitness;
}

/**
 * Create random solutions until maxevals has been achieved, and returns the
 * best one.
 *
 * @param problem The problem to be optimized
 * @param maxevals Maximum number of evaluations allowed
 * @return A pair containing the best solution found and its fitness
 */
ResultMH Grasp::optimize(Problem *problem, int maxevals)
{
    assert(maxevals > 0);
    Problem1 *realproblem = dynamic_cast<Problem1 *>(problem);
    int tam = realproblem->GetTam();
    int num_nodos = realproblem->GetCantNodos();
    vector<vector<double>> mapa = realproblem->GetMatriz();
    int nveces = 0;
    double best_fit=MAXFLOAT;
    tSolution best_sol;
    int evals=10;
    while (nveces < NUM_EJECS)
    {
        // Escogemos los dos primeros nodos aleatoriamente
        int primero = Random::get(0, num_nodos - 1);
        int segundo = primero;
        while (segundo == primero)
        {
            segundo = Random::get(0, num_nodos - 1);
        }

        tSolution sol;
        sol.push_back(primero);
        sol.push_back(segundo);

        SolutionInfoP1 informacion(sol, problem);

        while (sol.size() < tam)
        {
            vector<pair<int, double>> candidatos;

            // Evaluamos todos los nodos no seleccionados
            for (int i = 0; i < num_nodos; ++i)
            {
                if (find(sol.begin(), sol.end(), i) == sol.end())
                {
                    double heur = Heuristica(informacion, i, sol);
                    candidatos.emplace_back(i, heur);
                }
            }

            // Encontramos heurmin y heurmax
            double heurmin = std::numeric_limits<double>::max();
            double heurmax = std::numeric_limits<double>::lowest();
            for (const auto &par : candidatos)
            {
                heurmin = std::min(heurmin, par.second);
                heurmax = std::max(heurmax, par.second);
            }

            // Creamos LRC con α = 0.2
            double umbral = heurmin + ALPHA * (heurmax - heurmin);

            vector<int> LRC;
            for (const auto &par : candidatos)
            {
                if (par.second <= umbral)
                {
                    LRC.push_back(par.first);
                }
            }

            // Selección aleatoria desde LRC
            int elegido = LRC[Random::get<int>(0, LRC.size() - 1)];
            sol.push_back(elegido);
            informacion.ActualizarGreedy(elegido, sol);
        }

        tFitness fitness = realproblem->fitness(sol);
        if(version){
            ResultMH result=rbl->optimize(problem,sol,fitness,10000);

            sol=result.solution;
            evals+=result.evaluations;
            fitness=result.fitness;
        }

        if(fitness<best_fit){
            best_fit=fitness;
            best_sol=sol;
        }
        nveces++;
    }

    return ResultMH(best_sol, best_fit, evals);
}