#pragma once
#include <algorithm>
#include <problem.h>
#include "problem1.h"
#include <vector>
#include <iostream>
using namespace std;

class SolutionInfoP1: public SolutionFactoringInfo{
    public:
    //Vector donde se guarda la suma de distancias de un nodo con los demas
    vector<double> columna_fact;
    //Matriz de distancias
    vector<vector<double>> matriz;
    public:
    SolutionInfoP1(const tSolution &sol,Problem *problem);

    void ActualizarGreedy(unsigned mejor,tSolution const &solucion);
    void ActualizarBusquedaLocal(unsigned indice,int cambio,tSolution const &solucion);
};