package tracks.singlePlayer.evaluacion.src_MARTIN_PALOMINO_PABLO;

import core.game.Observation;
import core.game.StateObservation;
import core.player.AbstractPlayer;
import ontology.Types;
import tools.ElapsedCpuTimer;
import tools.Vector2d;

import java.util.*;

public class AgenteRTAStar extends AbstractPlayer {
	private Vector2d fescala; //Factor de escala
	private Vector2d posicion_jugador; //La posicion del jugador al principio
	private ArrayList<Observation>[][] matriz; //Guarda el mapa
	private double tiempo; //tiempo del algoritmo
	private double tam_sol; //tamaño de la solucion
	private ArrayList<Ubicacion> capas; //Lista de la ubicacion de las distintas capas del mapa
	private Map<Estado,Double> heuristicas; //Map que para cada estado guarda su valor heuristico

	Vector2d portal; //La ubicacion de la meta
	private Nodo current_node; //Guarda el nodo actual

	// Constructor
	public AgenteRTAStar(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {
		//Obtenemos el factor de escala como en el ejemplo de camel de clase dividiendo el ancho
		//del mata por el tamaño de una casilla y lo mismo por el alto
		fescala = new Vector2d(stateObs.getWorldDimension().width / stateObs.getObservationGrid().length,
				stateObs.getWorldDimension().height / stateObs.getObservationGrid()[0].length);

		//Inicializamos el map (hashmap por eficiencia) que alberga para cada estado como key su valor heuristico
		heuristicas= new HashMap<>();
		tiempo=0;
		tam_sol=0;
		//Obtenemos el mapa
		matriz=stateObs.getObservationGrid();
		
		//Obtenemos la posicion del jugador escalada
		posicion_jugador = stateObs.getAvatarPosition();
		posicion_jugador.x = Math.floor(posicion_jugador.x / fescala.x);
		posicion_jugador.y = Math.floor(posicion_jugador.y / fescala.y);
	
		//Inicializamos el estado
		Estado estado_interno = new Estado();
		estado_interno.posicion.y = (int) posicion_jugador.x;
		estado_interno.posicion.x = (int) posicion_jugador.y;
		
		//Creamos la lista de capas
		capas = new ArrayList<Ubicacion>();

		//Recorremos el mapa y guardamos en capas la ubicacion de las capas rojas y azules
		for (int i = 0; i < matriz.length; i++) {
			for (int j = 0; j < matriz[0].length; j++) {

				for (Observation obs : matriz[i][j]) {

					if (obs.itype == 8) {
						int[] aux = new int[3];
						aux[0] = (int) Math.floor(obs.position.x / fescala.x);
						aux[1] = (int) Math.floor(obs.position.y / fescala.y);
						Ubicacion aux2 = new Ubicacion(aux[0],aux[1]);
						capas.add(aux2);
					} else if (obs.itype == 9) {
						int[] aux = new int[3];
						aux[0] = (int) Math.floor(obs.position.x / fescala.x);
						aux[1] = (int) Math.floor(obs.position.y / fescala.y);
						aux[2] = 1;
						Ubicacion aux2 = new Ubicacion(aux[0],aux[1]);
						capas.add(aux2);
					}
				}
			}
		}
		//ponemos capas de estado interno al valor capas
		estado_interno.capas=capas;
		
		current_node = new Nodo();
		current_node.estado=estado_interno;
		
		//Obtenemos posicion de la meta escalada
		portal = stateObs.getPortalsPositions(posicion_jugador)[0].get(0).position;
        portal.x = Math.floor(portal.x / fescala.x);
        portal.y = Math.floor(portal.y / fescala.y);

	}
	@Override
	public Types.ACTIONS act(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {
		//Inicializamos el tiempo de la ejecucion de un act
		double Inicio = System.nanoTime();
		//Inicializamos un vector de costos por cada posible accion (4)
	    double[] costos = new double[4];
	    
	    //Comprobamos si el nodo actual tenia una capa en el mapa para actualizar acordemente a la realidad
	    //si la tenia ponemos a true o false los booleanos necesarios y eleminamos esa capa de capas
		if (CasillaTransitable(current_node.estado, current_node.estado.posicion.x, current_node.estado.posicion.y)) {
			if (!matriz[current_node.estado.posicion.y][current_node.estado.posicion.x].isEmpty()) {
				for (Observation obs : matriz[current_node.estado.posicion.y][current_node.estado.posicion.x]) {
					if (obs.itype == 8 || obs.itype == 9) {
						int posx = (int) Math.floor(obs.position.x / fescala.x);
						int posy = (int) Math.floor(obs.position.y / fescala.y);

						for (int i = 0; i < current_node.estado.capas.size(); i++) {
							if (posx == current_node.estado.capas.get(i).x && posy == current_node.estado.capas.get(i).y) {
								if (obs.itype == 9) {
									current_node.estado.capa_roja = false;
									current_node.estado.capa_azul = true;
								} else {
									current_node.estado.capa_roja = true;
									current_node.estado.capa_azul = false;
								}
								
								current_node.estado.capas.remove(i);

								break;
							}
						}
					}
				}
			}
		}
		
		//Rellenamos el array del maximo valor posible para luego encontrar el minimo y segundoMinimo mas facilmente
	    Arrays.fill(costos, Double.MAX_VALUE);
	    int activo=0;
	    //Evaluamos cada acción posible
	    Estado[] nuevosEstados = {
	        applyAction(Types.ACTIONS.ACTION_RIGHT, current_node.estado),
	        applyAction(Types.ACTIONS.ACTION_LEFT, current_node.estado),
	        applyAction(Types.ACTIONS.ACTION_UP, current_node.estado),
	        applyAction(Types.ACTIONS.ACTION_DOWN, current_node.estado)
	    };
	    
	    //Calculamos los costos solo para movimientos validos
	    for (int i = 0; i < 4; i++) {
	        if (!nuevosEstados[i].equals(current_node.estado)) {
	            costos[i] = 1 + getHeuristica(nuevosEstados[i]);
	            activo++;
	        }
	    }
	    
	    //Si no hubo ningún movimiento valido devolver acción nula (Nunca deberia ejecutarse pero como cortafuegos esta bien)
	    if(activo==0) {
	    	return Types.ACTIONS.ACTION_NIL;
	    }
	    //Encontramos mejor y segundo mejor movimiento
	    int mejorIdx = 0;
	    double min = Double.POSITIVE_INFINITY;
	    double segundoMin = Double.POSITIVE_INFINITY;
	    
	    for (int i = 0; i < 4; i++) {
	        if (costos[i] < min) {
	            segundoMin = min;
	            min = costos[i];
	            mejorIdx = i;
	        } else if (costos[i] < segundoMin) {
	            segundoMin = costos[i];
	        }
	    }
	    //Si solo hubo un movimiento valido en realidad no hay segundominimo y te quedas como ese de minimo
	    if(activo<=1) {
	    	segundoMin=costos[mejorIdx];
	    }
	    
	    //Actualizamos valor del map heuristicas de acuerdo a RTA
	    double actual = getHeuristica(current_node.estado);
	    double nueva = Math.max(actual, segundoMin);
	    heuristicas.put(current_node.estado,nueva);    
	    
	    //Actualizamos el estado y devolvemos la accion correspondiente
	    Types.ACTIONS[] acciones = {
	        Types.ACTIONS.ACTION_RIGHT,
	        Types.ACTIONS.ACTION_LEFT,
	        Types.ACTIONS.ACTION_UP,
	        Types.ACTIONS.ACTION_DOWN
	    };
	    
	    current_node.estado = nuevosEstados[mejorIdx];
	    
	    //Calculamos el tiempo del act y lo sumamos al acumulado, tambien aumentamos el tamaño de la sol en 1
        double Fin = System.nanoTime();
		double Resta= (Fin - Inicio)/1000000;
		tiempo+=Resta;
		tam_sol++;
		
		//Si el nodo al que se llega es la solucion imprimir toda la informacion
        if(current_node.estado.posicion.y == portal.x && current_node.estado.posicion.x == portal.y) {
        	System.out.println("Tiempo:" + tiempo);
        	System.out.println("Tamaño ruta:" + tam_sol);
        	System.out.println("Nodos expandidos:" + tam_sol);
        }
        
	    return acciones[mejorIdx];
	}

	// TRAMPA ITYPE 3
	// MURO ITYPE 5
	// CAMINO ROJO ITYPE 6
	// CAMINO AZUL ITYPE 7
	// CAPA ROJA ITYPE 8
	// CAPA AZUL ITYPE 9
	// JUGADOR ITYPE 10
	//Esta funcion comprueba que la casilla en la matriz[j][i] es transitable es decir es una casilla valida
	public boolean CasillaTransitable(Estado estate, int i, int j) {
		//Compruebo que este dentro de los limites del mapa
		if (j < 0 || j >= matriz.length || i < 0 || i >= matriz[j].length) {
			return false;
		}
		//Si es 'pasto' es decir camino viable devuelvo true
		if (matriz[j][i].isEmpty()) {
			return true;
		}
		//Según el tipo de casilla que sea devuelvo si es transitable o no, trampa y muro no lo son
		//camino rojo no lo es si no tienes capa roja y lo mismo con capa azul y camino azul
		for (Observation obs : matriz[j][i]) {
			switch (obs.itype) {
			case 3: // Trampa
			case 5: // Muro
				return false;
			case 6: // Camino rojo
				if (!estate.capa_roja)
					return false;
				break;
			case 7: // Camino azul
				if (!estate.capa_azul)
					return false;
				break;
			}
		}

		return true;
	}
	
    //Devuelve el valor de la heurística para el estado e, en caso de no estar ademas lo agrega
    private double getHeuristica(Estado e) {
        if (!heuristicas.containsKey(e)) {
        	heuristicas.put(e, heuristicaManhattan(e));
        }
        return heuristicas.get(e);
    }
	
    //Calcula la heuristica manhattan del estado
	private double heuristicaManhattan(Estado estado) {
	    
	    int distancia = Math.abs(estado.posicion.y - (int) portal.x) + Math.abs(estado.posicion.x - (int) portal.y);

	    return distancia;
	}
	//Aplica la accion indicada al estado
	private Estado applyAction(Types.ACTIONS action, Estado estado) {
		Estado newEstado = estado.deepCopy();
		//Cambiamos la posicion en funcion de la accion pasada
		switch (action) {
		case ACTION_UP:
			newEstado.posicion.x--;
			break;
		case ACTION_DOWN:
			newEstado.posicion.x++;
			break;
		case ACTION_LEFT:
			newEstado.posicion.y--;
			break;
		case ACTION_RIGHT:
			newEstado.posicion.y++;
			break;
		default:
			break;
		}
		//Si la nueva posicion se detecta como una casilla no transitable entonces se devuelve el mismo estado al pasado
		//a la funcion
		if(!CasillaTransitable(newEstado, newEstado.posicion.x, newEstado.posicion.y)) {
			newEstado = estado;
		}

		return newEstado;
	}
}
