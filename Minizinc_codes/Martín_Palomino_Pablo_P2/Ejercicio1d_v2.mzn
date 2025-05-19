% Ejercicio 1D

% Conjunto de indices para mayor legibilidad,posible reutilización y código más óptimo y escalable
% La otra opción seria no crear este conjunto de indices y poner a mano 1..9 pero de esta manera es más elegante
set of int: monedas = 1..4;  

% Creamos array que contiene los valores en centimos de cada moneda quitando una gran cantidad de monedas
% para reducir el espacio de búsqueda
array[monedas] of int: valores = [1, 10, 50, 100];

% Cantidad total deseada en céntimos
int: total = 122233367;  

% Número de monedas de cada tipo
array[monedas] of var int: cantidades;

% La parte entera del importe
int: entero = total div 100;
% Parte decimal
int: centimos = total mod 100;

% Restricciones:
  % La parte entera se cubre solo con monedas de 1 y 5 unidades
constraint
    (cantidades[4] * valores[4]) = entero * 100;

  % La parte decimal se cubre con las monedas fraccionarias
constraint
    sum(i in 1..3)(cantidades[i] * valores[i]) = centimos;

  % No se permiten cantidades negativas
constraint
    forall(i in monedas)(cantidades[i] >= 0);

% Resolvemos el problema con las restricciones ya mencionadas
solve satisfy;

% Formateado de Salida de datos con formato pedido en práctica
output [
    "monedas: [", join(", ", [show(cantidades[i]) | i in monedas]), "]\n",
    "total monedas: ", show(sum(i in monedas)(cantidades[i]))
];