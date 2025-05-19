% Denominaciones de monedas en céntimos
set of int: monedas = 1..8;  % Quitamos la moneda de 5 Unidades
array[monedas] of int: valores = [1, 3, 5, 10, 25, 50, 100, 200];

% Cantidad total deseada en céntimos
int: total = 147;  

% Número de monedas de cada tipo
array[monedas] of var int: cantidades;

% La parte entera del importe
int: entero = total div 100;
% Parte decimal
int: centimos = total mod 100;

% Restricción: cubrir la parte entera solo con monedas de 1 y 2 Unidades
constraint
    sum(i in 7..8)(cantidades[i] * valores[i]) = entero * 100;

% Restricción: cubrir la parte decimal solo con monedas de céntimos
constraint
    sum(i in 1..6)(cantidades[i] * valores[i]) = centimos;

  % No se permiten cantidades negativas
constraint
    forall(i in monedas)(cantidades[i] >= 0);
% Resolvemos
solve satisfy;
% Salida de datos
output [
    "monedas: [", join(", ", [show(cantidades[i]) | i in monedas]), "]\n",
    "total monedas: ", show(sum(i in monedas)(cantidades[i]))
];