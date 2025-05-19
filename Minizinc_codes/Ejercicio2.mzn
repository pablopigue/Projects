% Ejercicio 2
include "globals.mzn";

% Hay 5 parejas
int: N = 5;

% Definimos un set de indices por legibilidad con el numero de participantes en este problema
set of int: personas = 1..10;

% Definimos un array con el nombre de las personas del problema
array[personas] of string: nombres = ["Lucía", "Sofía", "Juan", "Cristina", "Claudia", "Pablo", "Marcos", "José", "Marta", "Pedro"];

% Definimos un set de indices por legibilidad con el numero de ciudades en este problema
set of int: ciudades = 1..5;

% Definimos un array con el nombre de las ciudades del problema
array[ciudades] of string: nombre_ciudad = ["Madrid", "Barcelona", "Sevilla", "Lyon", "Parma"];

% Variables
% Array con primer miembro de una pareja
array[1..N] of var personas: persona1;

% Array con segundo miembro de una pareja
array[1..N] of var personas: persona2;

% Array con ciudad a la que pertenece una pareja
array[1..N] of var ciudades: ciudad;

% Array con años de casados de las parejas (Máximo 30 por enunciado)
array[1..N] of var 5..30: anos;

% Restricciones:

  % Aseguramos que todas las personas se emparejan exactamente una vez
constraint all_different(persona1 ++ persona2);

  % Aseguramos que ninguna persona se puede emparejar consigo mismo
constraint forall(i in 1..N)(persona1[i] != persona2[i]);

  % Aseguramos que cada pareja tiene un miembro de {Lucía, Sofía, Juan, Cristina, Claudia} 
  % y otro de {Pablo, Marcos, José, Marta, Pedro}
constraint forall(i in 1..N)(
    % persona1 es del grupo 1 (1-5) y persona2 del grupo 2 (6-10), o viceversa
    ((persona1[i] <= 5 /\ persona2[i] >= 6) \/ (persona1[i] >= 6 /\ persona2[i] <= 5))
);

  % Aseguramos que las ciudades y años deben ser únicos
constraint all_different(ciudad);
constraint all_different(anos);

  % Sofía < José y Barcelona, pero > Marcos y Madrid
constraint exists(i in 1..N)(
    (persona1[i] == 2 \/ persona2[i] == 2) /\
    exists(j in 1..N)((persona1[j] == 8 \/ persona2[j] == 8) /\ anos[i] < anos[j]) /\
    exists(k in 1..N)(ciudad[k] == 2 /\ anos[i] < anos[k]) /\
    exists(l in 1..N)((persona1[l] == 7 \/ persona2[l] == 7) /\ anos[i] > anos[l]) /\
    exists(m in 1..N)(ciudad[m] == 1 /\ anos[i] > anos[m])
);

  % Cristina = 2*Sevilla
constraint exists(i in 1..N)(
    (persona1[i] == 4 \/ persona2[i] == 4) /\
    exists(j in 1..N)(ciudad[j] == 3 /\ anos[i] == 2 * anos[j])
);

  % Cristina = Pablo/2
constraint exists(i in 1..N)(
    (persona1[i] == 4 \/ persona2[i] == 4) /\
    exists(j in 1..N)((persona1[j] == 6 \/ persona2[j] == 6) /\ anos[i] * 2 == anos[j])
);

  % Parma = Marta + 10
constraint exists(i in 1..N)(
    ciudad[i] == 5 /\
    exists(j in 1..N)((persona1[j] == 9 \/ persona2[j] == 9) /\ anos[i] == anos[j] + 10)
);

  % Parma = Juan - 10
constraint exists(i in 1..N)(
    ciudad[i] == 5 /\
    exists(j in 1..N)((persona1[j] == 3 \/ persona2[j] == 3) /\ anos[i] == anos[j] - 10)
);

  % Marcos y Claudia llevan 25 años menos que Lyon
constraint exists(i in 1..N)(
    ((persona1[i] == 5 \/ persona2[i] == 5) /\ (persona1[i] == 7 \/ persona2[i] == 7)) /\
    exists(j in 1..N)(ciudad[j] == 4 /\ anos[i] + 25 == anos[j])
);

  % Lyon tiene pareja de 30 años casados 
constraint exists(i in 1..N)(ciudad[i] == 4 /\ anos[i] == 30);

  % Lucía y Madrid no son mínimos
constraint forall(i in 1..N)(
    (persona1[i] == 1 \/ persona2[i] == 1) -> anos[i] > min(anos)
) /\
forall(j in 1..N)(
    (ciudad[j] == 1) -> anos[j] > min(anos)
);

  % Pedro lleva 25 años casado
constraint exists(i in 1..N)((persona1[i] == 10 \/ persona2[i] == 10) /\ anos[i] == 25);

  % Rompemos posibles simétrias en las soluciones ordenando parejas
constraint forall(i in 1..N)(
    persona1[i] <= 5 /\ persona2[i] >= 6
);

  % Preparamos el formateo para la salida de datos
constraint increasing(anos);

% Resolvemos el problema
solve satisfy;

% Función para mostrar nombres en la solución
function string: get_name(var int: p) = 
    if fix(p) in personas then nombres[fix(p)] else "?" endif;

% Salida de datos
output [
  nombre_ciudad[fix(ciudad[i])] ++ ": " ++
  get_name(persona1[i]) ++ " y " ++ get_name(persona2[i]) ++ " - " ++ show(anos[i]) ++ " años\n"
  | i in 1..N
];