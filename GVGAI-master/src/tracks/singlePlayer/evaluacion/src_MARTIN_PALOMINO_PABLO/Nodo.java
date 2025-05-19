package tracks.singlePlayer.evaluacion.src_MARTIN_PALOMINO_PABLO;
import java.util.List;
import java.util.Objects;
import java.util.ArrayList;

import ontology.Types;
public class Nodo implements Comparable<Nodo> {
	public Estado estado; //El estado del nodo
	public List<Types.ACTIONS> lista; //Lista de acciones a ejecutar para llegar al estado del nodo
	double costo; //Coste para llegar al nodo
	int orden; //Orden usado para ordenar en caso de empates
	double esperado; //Suma del costo mas la heuristica
	double heur; //Valor de la heuristica para el nodo
	
	//Constructor
    public Nodo() {
        this.estado = new Estado();
        this.lista = new ArrayList<>();
        this.costo = 0;
        this.orden = 0;
        this.heur =0;
        this.esperado=0;
    }
    
    //Constructor pasandole un estado que copia
    public Nodo(Estado e) {
        this.estado = new Estado(e);
        this.lista = new ArrayList<>();
        this.costo = 0;
        this.orden = 0;
        this.heur =0;
        this.esperado=0;
    }
    
    //Constructor de copia
    public Nodo(Nodo other) {
        this.estado = new Estado(other.estado);
        this.lista = new ArrayList<Types.ACTIONS>(other.lista);
        this.costo = other.costo;
        this.orden = other.orden;
        this.heur =other.heur;
        this.esperado=other.esperado;
    }
    
    //Funcion que chequea igualdad entre objetos nodo
    @Override
    public boolean equals(Object obj) {
    	//Si son el mismo objeto en memoria es true
        if (this == obj) return true;
        //Si el objeto es null o es de otra clase es false;
        if (obj == null || getClass() != obj.getClass()) return false;
        
        Nodo that = (Nodo) obj;
        //Si tienen mismo estado consideramos que los nodos son iguales
        return Objects.equals(estado, that.estado);
    }
    
    //Funcion que compara nodos
    @Override
    public int compareTo(Nodo other) {
    	//Primero comparamos por esperado que es la suma de costo y heuristica
    	int result = Double.compare(this.esperado, other.esperado);
    	//Si tienen mismo esperado entonces comparamos por costo
    	if(result==0) {
    		result = Double.compare(this.costo, other.costo);
    	}
    	//Si tienen mismo costo comparamos finalmente por orden
		if(result==0) {
			result = Double.compare(orden, other.orden);
		}
		return result;
    }
    
    //Funcion que genera codigo hash en funcion de los atributos del objeto
    @Override
    public int hashCode() {
    	return Objects.hash(estado,lista,costo,orden,heur,esperado);
    }
    
}
