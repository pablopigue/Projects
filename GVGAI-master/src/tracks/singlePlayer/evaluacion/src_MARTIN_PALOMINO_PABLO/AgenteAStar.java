package tracks.singlePlayer.evaluacion.src_MARTIN_PALOMINO_PABLO;

import core.game.Observation;
import core.game.StateObservation;
import core.player.AbstractPlayer;
import ontology.Types;
import tools.ElapsedCpuTimer;
import tools.Vector2d;

import java.util.*;

public class AgenteAStar extends AbstractPlayer {
	private Vector2d fescala; //Factor de escala
	private Vector2d posicion_jugador; //La posicion del jugador al principio
	private ArrayList<Observation>[] posiciones_meta; //Las posiciones de las metas
	private ArrayList<Ubicacion> capas; //La ubicacion de las distintas capas del mapa
	Vector2d portal; //La ubicacion de la meta

	private ArrayList<Observation>[][] matriz; //El mapa
	boolean SolutionFound; //Booleano que indica si hemos encontrado solucion
	public List<Types.ACTIONS> plan; //plan a realizar (lista de acciones hacia meta devuelto por astar)

	// Constructor
	public AgenteAStar(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {
		//Obtenemos el factor de escala como en el ejemplo de camel de clase dividiendo el ancho
		//del mata por el tamaño de una casilla y lo mismo por el alto
		fescala = new Vector2d(stateObs.getWorldDimension().width / stateObs.getObservationGrid().length,
				stateObs.getWorldDimension().height / stateObs.getObservationGrid()[0].length);

		//Obtenemos la posicion donde se encuentra el jugador y la guardamos escalada
		posicion_jugador = stateObs.getAvatarPosition();
		posicion_jugador.x = Math.floor(posicion_jugador.x / fescala.x);
		posicion_jugador.y = Math.floor(posicion_jugador.y / fescala.y);

		//Inicializamos a false el booleano de solucion encontrada
		SolutionFound = false;

		//Obtenemos las posiciones de las metas
		posiciones_meta = stateObs.getPortalsPositions(posicion_jugador);
		
		//Como sabemos que solo va a haber una meta en mapas propuesto establecemos la meta como la primera
		//y escalamos las posiciones
		portal = posiciones_meta[0].get(0).position;
		portal.x = Math.floor(portal.x / fescala.x);
		portal.y = Math.floor(portal.y / fescala.y);
		
		//Obtenemos el mapa
		matriz = stateObs.getObservationGrid();

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

		//Inicializamos la lista plan
		plan = new ArrayList<>();
	}

	@Override
	public Types.ACTIONS act(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {
		Types.ACTIONS action = Types.ACTIONS.ACTION_NIL;
		//Si el plan es nulo o no se ha creado plan entonces creamos plan y cogemos la primera accion del plan
		if (plan == null || plan.isEmpty()) {
			Estado estado_interno = new Estado();
			estado_interno.posicion.y = (int) posicion_jugador.x;
			estado_interno.posicion.x = (int) posicion_jugador.y;
			estado_interno.capas = capas;
			plan = Astar(estado_interno, matriz);
			if(!plan.isEmpty()) {
				action = plan.getFirst();
				plan.remove(0);
			}
			
		} 
		//Si hay plan creado entonces solo hace falta coger la siguiente accion
		else {
			action = plan.getFirst();
			plan.remove(0);
		}
		return action;
	}
	
	//Algoritmo Aestrella que devuelve el plan 
	public List<Types.ACTIONS> Astar(Estado inicio, ArrayList<Observation>[][] mapa) {
		//Empezamos medicion de tiempo
		long tInicio = System.nanoTime();
		
		//Creamos abiertos y cerrados siendo cerrados un hashset y abiertos una priorityqueue por eficiencia
		PriorityQueue<Nodo> frontier = new PriorityQueue<>();
		Set<Estado> explored = new HashSet<>();
		List<Types.ACTIONS> plan = new ArrayList<>();
		
		//Creamos el primer nodo y lo añadimos a abiertos
		Nodo current_node = new Nodo();
		current_node.estado = inicio;
		current_node.costo = 0;
		current_node.heur = heuristicaManhattan(current_node.estado);
		current_node.esperado = current_node.costo + current_node.heur;
		int contador_nodos = 0;
		int orden_prog = 0;
		
		frontier.add(current_node);

		//Mientras abiertos no este vacio o no hayamos encontrado solucion seguimos haciendo busqueda
		while (!frontier.isEmpty() && !SolutionFound) {
			//Obtenemos el siguiente nodo en abiertos
			current_node = frontier.poll();
			
			//Si ese nodo ha sido ya explorado es decir si estado esta en cerrados simplemente no lo seguimos explorando y 
			//volvemos al principio del while
		    if (explored.contains(current_node.estado)) {
		        continue;
		    }
		    
		  //añadimos a cerrados el estado del nodo
			explored.add(current_node.estado);
			contador_nodos++;
			
			// Chequeamos si hemos llegado a solucion
			if (current_node.estado.posicion.y == portal.x && current_node.estado.posicion.x == portal.y) {
				SolutionFound = true;
			}

			//Si no hemos llegado a solucion expandimos nodos
			if (!SolutionFound) {
				//Hijo derecha
				Nodo child_turnsr = new Nodo(current_node);
				//El apply se explica abajo pero devuelve el estado al hacer la accion y si no es viable se devuelve el mismo estado
				//que se ha pasado
				child_turnsr.estado = applyAction(Types.ACTIONS.ACTION_RIGHT, current_node.estado);
				//Añadimos la accion pertinente a la lista de acciones del nodo
				child_turnsr.lista.add(Types.ACTIONS.ACTION_RIGHT);

				//Si no se ha explorado previamente se actualizan costo,heur,esperado y orden y lo añadimos a abiertos
				if (!explored.contains(child_turnsr.estado)) {
					child_turnsr.costo = current_node.costo
							+ CostoCasilla(current_node.estado, Types.ACTIONS.ACTION_RIGHT);
					child_turnsr.heur = heuristicaManhattan(child_turnsr.estado);
					child_turnsr.esperado = child_turnsr.costo + child_turnsr.heur;
					orden_prog++;
					child_turnsr.orden = orden_prog;
					frontier.add(child_turnsr);
				}

				//Hijo izquierda (mismo codigo que derecha pero adaptado a accion izquierda)
				Nodo child_turnl = new Nodo(current_node);
				child_turnl.estado = applyAction(Types.ACTIONS.ACTION_LEFT, current_node.estado);
				child_turnl.lista.add(Types.ACTIONS.ACTION_LEFT);

				if (!explored.contains(child_turnl.estado)) {
					child_turnl.costo = current_node.costo
							+ CostoCasilla(current_node.estado, Types.ACTIONS.ACTION_LEFT);
					child_turnl.heur = heuristicaManhattan(child_turnl.estado);
					child_turnl.esperado = child_turnl.costo + child_turnl.heur;
					orden_prog++;
					child_turnl.orden = orden_prog;

					frontier.add(child_turnl);
				}

				//Hijo arriba (mismo codigo que derecha pero adaptado a accion arriba)
				Nodo child_actup = new Nodo(current_node);
				child_actup.estado = applyAction(Types.ACTIONS.ACTION_UP, current_node.estado);
				child_actup.lista.add(Types.ACTIONS.ACTION_UP);

				if (!explored.contains(child_actup.estado)) {
					child_actup.costo = current_node.costo + CostoCasilla(current_node.estado, Types.ACTIONS.ACTION_UP);
					child_actup.heur = heuristicaManhattan(child_actup.estado);
					child_actup.esperado = child_actup.costo + child_actup.heur;
					orden_prog++;
					child_actup.orden = orden_prog;

					frontier.add(child_actup);
				}

				//Hijo abajo (mismo codigo que derecha pero adaptado a accion abajo)
				Nodo child_actdown = new Nodo(current_node);
				child_actdown.estado = applyAction(Types.ACTIONS.ACTION_DOWN, current_node.estado);
				child_actdown.lista.add(Types.ACTIONS.ACTION_DOWN);

				if (!explored.contains(child_actdown.estado)) {
					child_actdown.costo = current_node.costo
							+ CostoCasilla(current_node.estado, Types.ACTIONS.ACTION_DOWN);
					child_actdown.heur = heuristicaManhattan(child_actdown.estado);
					child_actdown.esperado = child_actdown.costo + child_actdown.heur;
					orden_prog++;
					child_actdown.orden = orden_prog;

					frontier.add(child_actdown);

				}
			}
		}
		//Calculamos he imprimimos el tiempo
		long tFin = System.nanoTime();
		long tiempoTotalms = (tFin - tInicio) / 1000000;
		//Si hemos llegado a solucion imprimimos lo pedido en la practica
		if (SolutionFound) {
			plan = current_node.lista;
			System.out.println("Tamaño de la ruta: " + current_node.lista.size() + " | Cantidad de nodos: "
					+ contador_nodos + "\n" + "Tamaño del frontier(abiertos): " + frontier.size() + " | Tamaño del explored(cerrados): "
					+ explored.size() + " | Tiempo en act: " + tiempoTotalms);
		}
		return plan;
	}

	// TRAMPA ITYPE 3
	// MURO ITYPE 5
	// CAMINO ROJO ITYPE 6
	// CAMINO AZUL ITYPE 7
	// CAPA ROJA ITYPE 8
	// CAPA AZUL ITYPE 9
	// JUGADOR ITYPE 10
	//Misma funcion que djikstra,Funcion que me dice si una casilla es transitable
	public boolean CasillaTransitable(Estado estate, int i, int j) {
		//Primero chequeamos que este dentro de los limites del mapa
		if (j < 0 || j >= matriz.length || i < 0 || i >= matriz[j].length) {
			return false;
		}
		//Si esta vacio en esa posicion entonces es camino viable
		if (matriz[j][i].isEmpty()) {
			return true;
		}
		//Por ultimo disitnguimos los casos como trampa,muro o camino rojo sin capa roja e idem para azul
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

	//Esta funcion si hubiese distintos costes cambiaria el coste segun la casilla pero en nuestro caso siempre vale 1
	private int CostoCasilla(Estado estado, Types.ACTIONS action) {
		return 1;
	}

	//Funcion que calcula la heuristicaManhattan de un estado
	private int heuristicaManhattan(Estado estado) {

		int distancia = Math.abs(estado.posicion.y - (int) portal.x) + Math.abs(estado.posicion.x - (int) portal.y);

		return distancia;
	}

	//Funcion que aplica el cambio de realizar una accion a un estado (misma que djisktra)
	private Estado applyAction(Types.ACTIONS action, Estado estado) {
		//Realizamos una copia del estado
		Estado newEstado = new Estado();
		newEstado.posicion = new Ubicacion(estado.posicion.x, estado.posicion.y);
		newEstado.capa_azul = estado.capa_azul;
		newEstado.capa_roja = estado.capa_roja;
		newEstado.capas = estado.capas;
		//Realizamos el cambio que realizaria la accion
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

		//Si la casilla es transitable vemos si tiene una capa en suso dicho cambio tambien 
		//cambiaremos el estado en funcion de la capa
		if (CasillaTransitable(newEstado, newEstado.posicion.x, newEstado.posicion.y)) {
			if (!matriz[newEstado.posicion.y][newEstado.posicion.x].isEmpty()) {
				for (Observation obs : matriz[newEstado.posicion.y][newEstado.posicion.x]) {
					if (obs.itype == 8 || obs.itype == 9) {
						int posx = (int) Math.floor(obs.position.x / fescala.x);
						int posy = (int) Math.floor(obs.position.y / fescala.y);

						for (int i = 0; i < newEstado.capas.size(); i++) {
							if (posx == newEstado.capas.get(i).x && posy == newEstado.capas.get(i).y) {
								if (obs.itype == 9) {
									//Si obs.itype es 9 es que es una capa azul
									newEstado.capa_roja = false;
									newEstado.capa_azul = true;
								} else {
									//Si obs.itype es 8 es que es una capa roja
									newEstado.capa_roja = true;
									newEstado.capa_azul = false;
								}
								//Copiamos la lista de capas y quitamos la que hemos cogido
								newEstado.capas = new ArrayList<>(estado.capas);
								newEstado.capas.remove(i);
								
								break;
							}
						}
					}
				}
			}
		} //En caso de que la casilla no sea transitable devolvemos el mismo estado que recibio de entrada
		else {
			newEstado = estado;
		}

		return newEstado;
	}
}
