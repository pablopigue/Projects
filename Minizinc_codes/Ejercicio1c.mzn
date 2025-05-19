% Ejercicio 1C

% Conjunto de indices para mayor legibilidad,posible reutilización y código más óptimo y escalable
% La otra opción seria no crear este conjunto de indices y poner a mano 1..9 pero de esta manera es más elegante
set of int: monedas = 1..9;

% Creamos array que contiene los valores en centimos de cada moneda
array[monedas] of int: valores = [1, 3, 5, 10, 25, 50, 100, 200, 500];

% Cantidad total deseada en céntimos
int: total = 299;  

% Número de monedas de cada tipo
array[monedas] of var int: cantidades;

% Para poder cumplir la restricción de parte entera a unidades y parte decimal a moneda fracccionaria debemos hacer
% la distinción entre parte entera y parte decimal del importe
% La parte entera del importe
int: entero = total div 100;
% Parte decimal
int: centimos = total mod 100;

% Creamos la variable objetivo a minimizar que es el total de monedas usadas
var int: total_cant = sum(i in monedas)(cantidades[i]);

%Restricciones:
  % Aseguramos que la parte entera sea completada por monedas de 1 o 2 unidades impidiendo que se puedan usar monedas de 5 unidades
constraint cantidades[9]=0;

  % Aseguramos que la parte entera se completa únicamente con monedas unitarias, podriamos en realidad poner indice de 7..8 pero por
  % dejar el codigo un poco mas general lo puse 7..9 , pero que vamos, el caso 9 siempre sera cero en la suma
constraint
    sum(i in 7..9)(cantidades[i] * valores[i]) = entero * 100;

  % Aseguramos que se complete la parte decimal
constraint
    sum(i in 1..6)(cantidades[i] * valores[i]) = centimos;

  % Aseguramos que no se permiten cantidades negativas
constraint
    forall(i in monedas)(cantidades[i] >= 0);
    
% Resolver minimizando el número total de monedas
solve minimize total_cant;

% Formateado de Salida de datos con formato pedido en práctica
output [
    "monedas: [", join(", ", [show(cantidades[i]) | i in monedas]), "]\n",
    "total monedas: ", show(total_cant)
];
