#include "utilidadesgeneticos.h"

vector<tSolution> GUtils::CreateFamily(Problem &problem, int tam_pob)
{
  //Creamos población de tamaño tam_pob
  vector<tSolution> family;
  for (int i = 0; i < tam_pob; i++)
  {
    family.push_back(problem.createSolution());
  }
  return family;
}

int GUtils::torneo(const vector<double> &fitness)
{
  //Elegimos 3 padres aleatorialmente y devolvemos el indice del que tenga menor fitness
  int bestIdx = Random::get<int>(0, fitness.size() - 1);
  double bestFit = fitness[bestIdx];

  for (int i = 0; i < 2; ++i)
  {
    int idx = Random::get<int>(0, fitness.size() - 1);
    double fit = fitness[idx];
    if (fit < bestFit)
    {
      bestFit = fit;
      bestIdx = idx;
    }
  }

  return bestIdx;
}

void GUtils::reparar(tSolution &solucion, int k, Problem *problem)
{
  // Intentar hacer dynamic_cast a Problem1
  Problem1 *problem1 = dynamic_cast<Problem1 *>(problem);
  if (problem1 == nullptr)
  {
    throw std::runtime_error("El objeto problem no es de tipo Problem1");
  }

  const std::vector<std::vector<double>> &distancias = problem1->GetMatriz();
  int n = solucion.size();
  int unos = count(solucion.begin(), solucion.end(), 1);

  while (unos != k)
  {
    if (unos > k)
    {
      //Aquí esta implementado si se quisiese de forma aleatoria y no heurística
      /*
      // Eliminar unos excedentes (aleatoriamente)
      std::vector<int> posicionesUnos;
      for (int i = 0; i < n; ++i)
      {
        if (solucion[i] == 1)
          posicionesUnos.push_back(i);
      }
      Random::shuffle(posicionesUnos.begin(), posicionesUnos.end());
      solucion[posicionesUnos[0]] = 0;
      unos--;
      */
     //Vemos la posiciones de los unos y vemos cual da más contribución para eliminarlo
      vector<int> posicionesUnos;
      for (int i = 0; i < n; ++i)
      {
        if (solucion[i] == 1)
          posicionesUnos.push_back(i);
      }

      double maxContribucion = -1.0;
      int posicionAEliminar = -1;

      for (int i = 0; i < posicionesUnos.size(); ++i)
      {
        int pos = posicionesUnos[i];
        double contribucion = 0.0;

        for (int j = 0; j < posicionesUnos.size(); ++j)
        {
          if (i != j)
          {
            contribucion += distancias[pos][posicionesUnos[j]];
          }
        }

        if (contribucion > maxContribucion)
        {
          maxContribucion = contribucion;
          posicionAEliminar = pos;
        }
      }

      if (posicionAEliminar != -1)
      {
        solucion[posicionAEliminar] = 0;
        unos--;
      }
    }
    else
    {
      // Añadir unos faltantes (aleatoriamente)
      std::vector<int> posicionesCeros;
      for (int i = 0; i < n; ++i)
      {
        if (solucion[i] == 0)
          posicionesCeros.push_back(i);
      }
      Random::shuffle(posicionesCeros.begin(), posicionesCeros.end());
      solucion[posicionesCeros[0]] = 1;
      unos++;
    }
  }
}

pair<tSolution, tSolution> GUtils::cruceUniforme(const tSolution &p1, const tSolution &p2, int k, int tam_max,Problem *problem)
{
  int n = tam_max;
  tSolution h1(n), h2(n);

  // Paso a binario
  tSolution p1_binary = indicesToBinary(p1, tam_max);
  tSolution p2_binary = indicesToBinary(p2, tam_max);

  // Paso 1: Mantener valores comunes en ambos hijos
  for (int i = 0; i < n; ++i)
  {
    if (p1_binary[i] == p2_binary[i])
    {
      h1[i] = p1_binary[i];
      h2[i] = p1_binary[i];
    }
  }

  // Paso 2: Para posiciones diferentes, asignar aleatoriamente
  vector<int> posicionesDiferentes;
  for (int i = 0; i < n; ++i)
  {
    if (p1_binary[i] != p2_binary[i])
    {
      posicionesDiferentes.push_back(i);
    }
  }

  Random::shuffle(posicionesDiferentes.begin(), posicionesDiferentes.end());

  // Asignar alternativamente a h1 y h2
  for (int i = 0; i < posicionesDiferentes.size(); ++i)
  {
    int pos = posicionesDiferentes[i];
    if (i % 2 == 0)
    {
      h1[pos] = p1_binary[pos];
      h2[pos] = p2_binary[pos];
    }
    else
    {
      h1[pos] = p2_binary[pos];
      h2[pos] = p1_binary[pos];
    }
  }

  // Paso 3: Reparación para exactamente k
  // Aplicar reparación 
  reparar(h1, k,problem);
  reparar(h2, k,problem);
  tSolution h1_entero = binaryToIndices(h1);
  tSolution h2_entero = binaryToIndices(h2);
  return {h1_entero, h2_entero};
}

pair<tSolution, tSolution> GUtils::cruceIntercambio(const tSolution &p1, const tSolution &p2, int k, int tam_max)
{
  int n = tam_max;
  tSolution h1(n, 0), h2(n, 0);
  // Paso a binario
  tSolution p1_binary = indicesToBinary(p1, tam_max);
  tSolution p2_binary = indicesToBinary(p2, tam_max);

  // Paso 1: Mantener los valores comunes en ambos hijos
  for (int i = 0; i < n; ++i)
  {
    if (p1_binary[i] == p2_binary[i])
    {
      h1[i] = p1_binary[i];
      h2[i] = p1_binary[i];
    }
  }

  // Paso 2: Identificar posiciones no comunes disponibles
  vector<int> posicionesDisponibles;
  for (int i = 0; i < n; ++i)
  {
    if (p1_binary[i] != p2_binary[i])
    {
      posicionesDisponibles.push_back(i);
    }
  }

  // Paso 3: Contar unos ya asignados en cada hijo
  int unosH1 = count(h1.begin(), h1.end(), 1);
  int unosH2 = count(h2.begin(), h2.end(), 1);

  // Paso 4: Calcular unos restantes por asignar
  int unosRestantes = k - unosH1;

  // Paso 5: Mezclar posiciones disponibles para asignación aleatoria
  Random::shuffle(posicionesDisponibles.begin(), posicionesDisponibles.end());

  // Paso 6: Asignar unos restantes a los hijos
  for (int i = 0; i < unosRestantes; ++i)
  {
    if (i < posicionesDisponibles.size())
    {
      h1[posicionesDisponibles[i]] = 1;
    }
  }

  // Paso 5: Mezclar posiciones disponibles para asignación aleatoria
  Random::shuffle(posicionesDisponibles.begin(), posicionesDisponibles.end());

  // Paso 6: Asignar unos restantes a los hijos
  for (int i = 0; i < unosRestantes; ++i)
  {
    if (i < posicionesDisponibles.size())
    {
      h2[posicionesDisponibles[i]] = 1;
    }
  }

  tSolution h1_entero = binaryToIndices(h1);
  tSolution h2_entero = binaryToIndices(h2);

  return {h1_entero, h2_entero};
}

void GUtils::mutarPoblacion(vector<tSolution> &poblacion, double PM, int tam_sol, bool tipoGenetico, int tam_max)
{
  if (tipoGenetico)
  {
    //Mutar primer hijo con prob PM
    if (Random::get<double>() < PM)
    {
      tSolution &hijo_a_mutar = poblacion[0];
      int idx1 = Random::get<int>(0, hijo_a_mutar.size() - 1);

      int next;
      bool valido;
      do
      {
        next = Random::get<int>(0, tam_max - 1);
        valido = true;
        for (int val : hijo_a_mutar)
        {
          if (val == next)
          {
            valido = false;
            break; 
          }
        }
      } while (!valido);

      hijo_a_mutar[idx1] = next;
    }
    //Mutar segundo hijo con prob PM
    if (Random::get<double>() < PM)
    {
      tSolution &hijo_a_mutar = poblacion[1];
      // paso a binario
      int idx1 = Random::get<int>(0, hijo_a_mutar.size() - 1);

      int next;
      bool valido;
      do
      {
        next = Random::get<int>(0, tam_max - 1);
        valido = true;
        for (int val : hijo_a_mutar)
        {
          if (val == next)
          {
            valido = false;
            break; 
          }
        }
      } while (!valido);

      hijo_a_mutar[idx1] = next;
    }
  }
  else
  {

    int numMutaciones = round(PM * poblacion.size() * tam_sol);

    for (int i = 0; i < numMutaciones; ++i)
    {
      // 1. Seleccionar un hijo_a_mutar aleatorio
      int idHijo = Random::get<int>(0, poblacion.size() - 1);
      tSolution &hijo_a_mutar = poblacion[idHijo];
      int idx1 = Random::get<int>(0, hijo_a_mutar.size() - 1);

      int next;
      bool valido;
      do
      {
        next = Random::get<int>(0, tam_max - 1);
        valido = true;
        for (int val : hijo_a_mutar)
        {
          if (val == next)
          {
            valido = false;
            break;
          }
        }
      } while (!valido);

      hijo_a_mutar[idx1] = next;
    }
  }
}

tSolution GUtils::binaryToIndices(const tSolution &binary)
{
  tSolution indices;
  for (int i = 0; i < binary.size(); ++i)
  {
    if (binary[i] == 1)
    {
      indices.push_back(i);
    }
  }
  return indices;
}

tSolution GUtils::indicesToBinary(const tSolution &ind, int tam_max)
{
  tSolution binary(tam_max, 0);
  for (int i = 0; i < ind.size(); ++i)
  {
    binary[ind[i]] = 1;
  }
  return binary;
}