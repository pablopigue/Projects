#include <solutionInfo.h>

SolutionInfoP1::SolutionInfoP1(const tSolution &sol,Problem *problem){
    Problem1 *realproblem = dynamic_cast<Problem1 *>(problem);
    matriz=realproblem->GetMatriz();
    double disp_ind=0;
    for(unsigned i=0; i < sol.size();i++){
        disp_ind=0;
        for(unsigned j=0; j < sol.size();j++){
            disp_ind+=matriz[sol[i]][sol[j]];
        }
        columna_fact.push_back(disp_ind);
    }
}

void SolutionInfoP1::ActualizarGreedy(unsigned mejor,tSolution const &solucion){
    
    double disp=0;
    for(unsigned i=0;i<columna_fact.size();i++){
        columna_fact[i]+=matriz[solucion[i]][mejor];
    }

    for(unsigned i=0;i<solucion.size();i++){
        disp+=matriz[mejor][solucion[i]];
    }
    columna_fact.push_back(disp);
}

void SolutionInfoP1::ActualizarBusquedaLocal(unsigned indice,int cambio,tSolution const &solucion){
    
    //Cambio la columna de distancia pues he cambiado el nodo
    double disp=0;
    for(unsigned i=0;i<columna_fact.size();i++){
        columna_fact[i]-=matriz[solucion[indice]][solucion[i]];
        columna_fact[i]+=matriz[cambio][solucion[i]];
    }

    
    for(unsigned i=0;i<solucion.size();i++)
    {
        if(i!=indice){
            disp+=matriz[cambio][solucion[i]];
        }
        
    }
    columna_fact[indice]=disp;
}