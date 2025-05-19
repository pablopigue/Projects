%Ejercicio 5

% Declaramos el numero de tareas a resolver
int: num_tareas = 10;

% Declaramos un set de indices para legibilidad de codigo
set of int: TAREAS = 1..num_tareas;

% Declaramos un array con las tareas a asignar
array[TAREAS] of string: nombres = [
  "Montar chasis", "Instalar ruedas", "Cableado eléctrico", 
  "Motor de fusión nuclear", "Tablero de control", 
  "Instalación y configuración del condensador de fluzo", 
  "Ajuste aerodinámico", "Instalación de puertas de ala de gaviota", 
  "Panel de tiempo", "Implementación y validación de algoritmos de búsqueda heurística para optimización de caminos"
];

% Declaramos un array con las duraciones de las tareas cuando las realiza marty
array[TAREAS] of int: dur_marty = [10,1,4,15,14,13,7,3,2,4];

% Declaramos un array con las duraciones de las tareas cuando las realiza doc
array[TAREAS] of int: dur_doc   = [16,2,3,5,3,5,6,4,2,2];

% Declaramos un array formado por los indices de las tareas que se deben de hacer antes de poder realizar una tarea
% concreta
array[TAREAS] of set of int: precedencias = [
  {},        % 1
  {1},       % 2
  {2},       % 3
  {1,3},     % 4
  {3},       % 5
  {3,4,5},   % 6
  {3,4},     % 7
  {7},       % 8
  {6,8},     % 9
  {7,9}      % 10
];

% Array que indica el inicio de cada tarea
array[TAREAS] of var 1..100: inicio;

% Array que indica el final de cada tarea
array[TAREAS] of var 1..100: fin;

% Array que indica si una tarea esta asignada a Marty (1) o a Doc(2)
array[TAREAS] of var 1..2: asignado;

% Array que indica si una tarea se realiza con apoyo de biff
array[TAREAS] of var bool: biff_apoya;
array[TAREAS] of var 1..20: duracion_real;

% Array que indica la duración sin reducción
array[TAREAS] of var 1..20: duracion_sin_reduccion;

% Array que contendrá si la reduccion dada por Biff es de 0,1 o 2
array[TAREAS] of var 0..2: reduccion;

% Restricciones:
  %  Las duraciones sin reducciones seran si la hace marty la duracion de marty y si la hace doc la duracion de doc
constraint forall(i in TAREAS) (
  duracion_sin_reduccion[i] = 
    if asignado[i] == 1 then dur_marty[i] else dur_doc[i] endif
);



  % Calcula apropiadamente la reducción proporcionada por Biff
constraint forall(i in TAREAS) (
  reduccion[i] = 
    if duracion_sin_reduccion[i] < 3 then
      0
    else 
      if (i == 1 \/ i == 3 \/ i == 8) then
        if biff_apoya[i] then 2 else 0 endif
      else
        if biff_apoya[i] then 1 else 0 endif
      endif
    endif
);

  % Calculamos apropadiamente la duración real y cálculo de fin
constraint forall(i in TAREAS)(
  duracion_real[i] = duracion_sin_reduccion[i] - reduccion[i] /\
  fin[i] = inicio[i] + duracion_real[i] -1
);

  % Las tareas no pueden empezar hasta que se hayan hecho las predecedoras
constraint forall(i in TAREAS, j in precedencias[i]) (
  inicio[i] > fin[j]
);

  % No solapamiento de tareas para cada trabajador
constraint forall(i,j in TAREAS where i < j) (
  (asignado[i] != asignado[j]) \/
  (fin[i] < inicio[j]) \/ (fin[j] < inicio[i])
);

  % No solapamiento de tareas con apoyo de Biff
constraint forall(i,j in TAREAS where i < j) (
  (not biff_apoya[i]) \/ (not biff_apoya[j]) \/
  (fin[i] < inicio[j]) \/ (fin[j] < inicio[i])
);

  % Máximo 4 tareas con apoyo de Biff
constraint sum([biff_apoya[i] | i in TAREAS]) <= 4;

  % Minimizar tiempo total
var int: tiempo_total = max([fin[i] | i in TAREAS]);

% Resolvemos el problema
solve minimize tiempo_total;

% Formato de salida
output [
  " \"" ++ nombres[i] ++ "\"\n" ++
  "  Inicio día " ++ show(inicio[i]) ++ 
  ", Fin día " ++ show(fin[i]) ++ 
  ", Asignada a: " ++ (if fix(asignado[i]) == 1 then "Marty" else "Doc" endif) ++
  (if fix(biff_apoya[i]) then " (con Biff)" else "" endif) ++ "\n"
  | i in TAREAS
] ++ ["\nTiempo total: " ++ show(tiempo_total)];