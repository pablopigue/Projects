% Ejercicio 1E

% Conjunto de indices para mayor legibilidad,posible reutilización y código más óptimo y escalable
% La otra opción seria no crear este conjunto de indices y poner a mano 1..9 pero de esta manera es más elegante
set of int: monedas = 1..9;

% Creamos array que contiene los valores en unidades de cada moneda
array[monedas] of float: valores = [0.01, 0.03, 0.05, 0.10, 0.25, 0.50, 1, 2, 5];

% Cantidad total deseada en unidades
float: total = 2.30;

% Número de monedas de cada tipo pero en enteros por probar
array[monedas] of var int: cantidades;

% Restricciones:
  % Aseguramos que la cantidad de monedas cogidas de cada tipo suman el valor de la cantidad deseada
constraint
    sum(i in monedas)(cantidades[i] * valores[i]) = total;

  % Aseguramos que no haya cantidades negativas de monedas
constraint
    forall(i in monedas)(cantidades[i] >= 0);

% Resolvemos el problema con las restricciones ya mencionadas
solve satisfy;

% Formateado de Salida de datos con formato pedido en práctica
output [
    "monedas: [", join(", ", [show(cantidades[i]) | i in monedas]), "]\n",
    "total monedas: ", show(sum(i in monedas)(cantidades[i]))
];