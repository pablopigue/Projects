%Ejercicio 3
include "globals.mzn";

% Definimos el tamaño de la matriz
int: n = 12;
set of int: rango = 1..n;

% Creamos la matriz de 0s y 1s
array[rango, rango] of var 0..1: matrix;

% Restricciones:
  % Establecemos los valores preasignados
constraint matrix[1,2] = 1;
constraint matrix[1,3] = 0;
constraint matrix[1,8] = 0;
constraint matrix[1,11] = 0;
constraint matrix[2,2] = 1;
constraint matrix[2,3] = 1;
constraint matrix[2,6] = 1;
constraint matrix[3,9] = 1;
constraint matrix[3,12] = 0;
constraint matrix[4,3] = 0;
constraint matrix[4,4] = 0;
constraint matrix[4,12] = 0;
constraint matrix[5,7] = 1;
constraint matrix[5,8] = 1;
constraint matrix[5,10] = 0;
constraint matrix[6,2] = 1;
constraint matrix[6,4] = 0;
constraint matrix[6,6] = 1;
constraint matrix[6,7] = 1;
constraint matrix[6,11] = 1;
constraint matrix[7,9] = 1;
constraint matrix[8,1] = 1;
constraint matrix[8,4] = 1;
constraint matrix[8,11] = 0;
constraint matrix[9,2] = 1;
constraint matrix[9,9] = 0;
constraint matrix[10,8] = 0;
constraint matrix[11,1] = 1;
constraint matrix[11,12] = 1;
constraint matrix[12,2] = 1;
constraint matrix[12,4] = 1;
constraint matrix[12,10] = 0;
constraint matrix[12,11] = 0;

  % Aseguramos el mismo numero de 1s en cada columna y fila (asegurando asi tambien el mismo número de 0s)
constraint forall(i in rango)(
    sum([matrix[i,j] | j in rango]) = n div 2
)
/\
forall(j in rango)(
    sum([matrix[i,j] | i in rango]) = n div 2
);

  % Aseguramos filas distintas entre si y columnas distintas entre si
constraint forall(i, k in rango where i < k) (
    exists(j in rango)(matrix[i,j] != matrix[k,j])
)
/\
forall(j, l in rango where j < l) (
    exists(i in rango)(matrix[i,j] != matrix[i,l])
);

  % Aseguramos que no haya tres valores consecutivos iguales ni en filas ni en columnas
  % para ello en lugar de hacerlo "a mano" usamos una funcion logica booleana con predicate
  % todo esto para hacer el codigo más legible pero que se podria hacer manual metiendo la restriccion
  % que indica este predicado en el constraint que viene justo despues que es lo que verdaderamente implementa
  % la restriccion
predicate no_tres_consecutivos(array[int] of var int: arr) =
    forall(i in 1..n-2)(
        not (arr[i] == arr[i+1] /\ arr[i+1] == arr[i+2])
    );

constraint forall(i in rango)(
    no_tres_consecutivos([matrix[i,j] | j in rango])
)
/\
forall(j in rango)(
    no_tres_consecutivos([matrix[i,j] | i in rango])
);

  % Resolvemos el problema
solve satisfy;

  % Salida de datos
output [
    if j == 1 then "\n" else "" endif ++
    show(matrix[i,j]) ++ " "
    | i in rango, j in rango
];
