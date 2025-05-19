%Ejercicio 4

% Establecemos el número a resolver
int: x =120;

% Establecemos set de indices de los posibles elementos del conjunto S
set of int: Y = 1..x-1;

% Establecemos un array que contendrá las soluciones que será array binario 
% que indicará 1 si el elemento pertenece al conjunto e indicará 0 si no lo hace
array[Y] of var 0..1: in_set;

% Calculamos la suma de cuadrados
var int: suma_cuadrados = sum(i in Y)(in_set[i] * i * i);

% Calculamos la cardinalidad del conjunto
var int: cardinalidad = sum(i in Y)(in_set[i]);

% Restricciones: 
  % Aseguramos que la suma de cuadrados debe ser igual a x*x
constraint suma_cuadrados = x*x;

% Resolvemos maximizando la cardinalidad
solve maximize cardinalidad;

% Formateamos la salida
output [
  "x = \"", show(x), "\"\n",
  "S = {[", 
  join(", ", [ show(i) | i in Y where fix(in_set[i]) = 1 ]),
  "]}\n",
  "Cardinalidad = \"", show(cardinalidad), "\"\n",
  "Suma de cuadrados = \"", show(suma_cuadrados), "\"\n"
];
