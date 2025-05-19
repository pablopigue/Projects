package tracks.singlePlayer.evaluacion.src_MARTIN_PALOMINO_PABLO;

import java.util.ArrayList;
import java.util.Objects;

public class Estado implements Comparable<Estado> {


	public Ubicacion posicion; //La posicion de nuestro personaje
	public boolean capa_azul;  //Booleano que indica si tenemos una capa azul
	public boolean capa_roja;  //Booleano que indica si tenemos una capa roja
	public ArrayList<Ubicacion> capas; //Lista de capas presentes en el mapa

	//Constructor
	public Estado() {
		posicion= new Ubicacion();
		capa_azul = false;
		capa_roja = false;
		capas= new ArrayList<Ubicacion>();
	}
	
	//Constructor de copia
	public Estado(Estado other) {
		posicion= new Ubicacion(other.posicion);
		capa_azul = other.capa_azul;
		capa_roja = other.capa_roja;
		//Aqui usamos copia por referencia osea no copia profunda para eficiencia dentro de djikstra y a*
		capas= other.capas;

	}
	
	//Funcion que devuelve una copia profunda del estado (sin copia por referencia)
	public Estado deepCopy() {
		Estado e = new Estado(this);
		e.capas= new ArrayList<Ubicacion>(this.capas);
		return e;
	}

	//Funcion que chequea que dos estados sean iguales
	@Override
	public boolean equals(Object obj) {
		//Si son el mismo objeto en memoria devuelve true
		if (this == obj)
			return true;
		//Si es null o no es de la misma clase no pueden ser iguales
		if (obj == null || getClass() != obj.getClass())
			return false;
		Estado that = (Estado) obj;
		//Compruebo igualdad por atributos que componen el objeto
		return capa_azul == that.capa_azul && 
		           capa_roja == that.capa_roja && 
		           posicion.equals(that.posicion) && 
		           capas.equals(that.capas);
	}

	//Funcion para comparar dos estados
	@Override
	public int compareTo(Estado b) {
		//Comparamos por posicion x
	    if (this.posicion.x != b.posicion.x)
	        return Integer.compare(this.posicion.x, b.posicion.x);
	    
	    //Comparamos por posicion y
	    if (this.posicion.y != b.posicion.y)
	        return Integer.compare(this.posicion.y, b.posicion.y);
	    
	    //Comparamos por capa_azul 
	    if (this.capa_azul != b.capa_azul)
	        return Boolean.compare(this.capa_azul, b.capa_azul);
	    
	    //Comparamos por posicion capa_roja  
	    if (this.capa_roja != b.capa_roja)
	        return Boolean.compare(this.capa_roja, b.capa_roja); 
	    
	    int compare_capas=0;
	    //Comparamos por la lista capas teniendo en cuenta que es comparacion por cada elemento que lo compone
	    for (int i = 0; i < this.capas.size(); i++) {
	        compare_capas = this.capas.get(i).compareTo(b.capas.get(i));
	        if (compare_capas != 0) return compare_capas;
	    }
	    
	    return compare_capas;
	}
	
	//Funcion para hashing, es decir para usar hashmaps,sets etc...
	//Devuelve codigo hash en funcion de los atributos del objeto
	@Override
    public int hashCode() {
    	return Objects.hash(posicion,capa_azul,capa_roja,capas);
    }
	
	

}
