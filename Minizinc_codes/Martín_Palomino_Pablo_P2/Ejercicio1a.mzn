% Ejercicio 1A

% Conjunto de indices para mayor legibilidad,posible reutilización y código más óptimo y escalable
% La otra opción seria no crear este conjunto de indices y poner a mano 1..9 pero de esta manera es más elegante
set of int: monedas = 1..9;

% Creamos array que contiene los valores en centimos de cada moneda
array[monedas] of int: valores = [1, 3, 5, 10, 25, 50, 100, 200, 500];

% Cantidad total deseada en céntimos
int: total = 299;

% Número de monedas de cada tipo
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

