%Ejercicio 6

% Número de TFGs
int: N = 10;   

% Número de profesores      
int: P = 9;

% Número de tribunales
int: T = 2; 

% TFGs por tribunal         
int: Max_TFGs = 5; 
  
% Dias
int: Dias = 5;

% Creamos sets de indices
set of int: PROFESORES = 1..P;
set of int: TFGS = 1..N;
set of int: TRIBUNALES = 1..T;
set of int: DECSAI = 1..3;
set of int: LSI = 4..6;
set of int: ICAR = 7..9;
set of int: DIAS = 1..Dias;

% Disponibilidad por profesor
array[PROFESORES] of set of DIAS: disponibilidad = [
  {2}, {1,3,5}, {1,3,5}, {3,4}, {1,4}, {1,2,3,4,5}, {5}, {3,4}, {4}
];

% Supervisión (profesor x TFG)
array[PROFESORES, TFGS] of bool: dirigio = array2d(PROFESORES, TFGS, [
  false, false, false, false, false, false, false, false, false, false,
  true,  false, false, false, false, false, false, false, false, true,
  false, false, false, false, false, false, false, true,  false, false,
  false, false, false, false, false, true,  false, false, false, false,
  false, true,  false, false, false, false, false, false, false, false,
  false, false, true,  false, false, false, false, false, false, false,
  false, false, true,  false, false, false, false, false, false, false,
  false, false, false, true,  false, false, true,  false, false, false,
  false, false, false, false, false, false, false, false, false, false
]);

% Departamento al que pertenece cada profesor
array[PROFESORES] of string: depto = 
  ["DECSAI", "DECSAI", "DECSAI", 
   "LSI", "LSI", "LSI", 
   "ICAR", "ICAR", "ICAR"];

% Variables
array[TRIBUNALES] of var DIAS: tribunal_dia;
array[TRIBUNALES, 1..3] of var PROFESORES: tribunal_profesores;
array[TFGS] of var TRIBUNALES: tfg_tribunal;

%Restricciones:
  % Asignar un profesor a un único tribunal
constraint forall(p in PROFESORES)(
  sum([ tribunal_profesores[t,i] = p | t in TRIBUNALES, i in 1..3 ]) <= 1
);


  % Un tribunal debe tener un profesor de cada departamento
constraint forall(t in TRIBUNALES)(
  exists(p in 1..3)(tribunal_profesores[t,p] in DECSAI) /\
  exists(p in 1..3)(tribunal_profesores[t,p] in LSI) /\
  exists(p in 1..3)(tribunal_profesores[t,p] in ICAR)
);

  % Cada tribunal en un día diferente
constraint tribunal_dia[1] != tribunal_dia[2];

  % Disponibilidad de cada profesor
constraint forall(t in TRIBUNALES, i in 1..3)(
  tribunal_dia[t] in disponibilidad[tribunal_profesores[t,i]]
);

  % Un TFG no puede ser evaluado por quien lo dirigió
constraint forall(t in TFGS, tr in TRIBUNALES)(
  (tfg_tribunal[t] = tr) ->
  forall(p in 1..3)(
    not dirigio[tribunal_profesores[tr,p], t]
  )
);

  % Número máximo de TFGs por tribunal
constraint forall(tr in TRIBUNALES)(
  sum([ tfg_tribunal[t] = tr | t in TFGS ]) <= Max_TFGs
);

% Rompemos simetrías: tribunal 1 con día menor que tribunal 2
constraint tribunal_dia[1] < tribunal_dia[2];

% Ordenar los profesores por ID dentro del tribunal para romper simetría
constraint forall(t in TRIBUNALES)(
  tribunal_profesores[t,1] < tribunal_profesores[t,2] /\
  tribunal_profesores[t,2] < tribunal_profesores[t,3]
);

% Romper simetría en asignación de TFGs 
constraint min([i | i in TFGS where tfg_tribunal[i] = 1]) < min([i | i in TFGS where tfg_tribunal[i] = 2]);

solve satisfy;

% Días de la semana
array[DIAS] of string: dias_semana = ["Lunes", "Martes", "Miércoles", "Jueves", "Viernes"];

output [
  "Tribunal " ++ show(t) ++ ": " ++
  show(tribunal_profesores[t,1]) ++ "(" ++ depto[fix(tribunal_profesores[t,1])] ++ "), " ++
  show(tribunal_profesores[t,2]) ++ "(" ++ depto[fix(tribunal_profesores[t,2])] ++ "), " ++
  show(tribunal_profesores[t,3]) ++ "(" ++ depto[fix(tribunal_profesores[t,3])] ++ ") (Día: " ++ dias_semana[fix(tribunal_dia[t])] ++ ")\n" ++
  "TFGs a evaluar: " ++
  join(", ", [show(tfg) | tfg in TFGS where fix(tfg_tribunal[tfg]) = t]) ++ "\n\n"
  | t in TRIBUNALES
];

