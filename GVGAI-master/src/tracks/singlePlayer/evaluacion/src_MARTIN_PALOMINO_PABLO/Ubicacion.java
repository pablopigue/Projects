package tracks.singlePlayer.evaluacion.src_MARTIN_PALOMINO_PABLO;

import java.util.Objects;

public class Ubicacion implements Comparable<Ubicacion>{
	int x; //Coordenada x
	int y; //Coordenada y
	
	//Constructor
	public Ubicacion() {
		this.x = 0;
		this.y = 0;
	}
	//Constructor con parametros
	public Ubicacion(int f, int c) {
		this.x = f;
		this.y = c;
	}
	//Constructor de copia
	public Ubicacion(Ubicacion other) {
		this.x = other.x;
		this.y = other.y;
	}

	//equals que comprueba la igualdad entre dos objetos Ubicacion
	@Override
	public boolean equals(Object obj) {
		//Si es el mismo objeto en memoria son iguales
		if (this == obj)
			return true;
		//Si el objeto es nulo o no es de la misma clase no pueden ser iguales
		if (obj == null || getClass() != obj.getClass())
			return false;
		Ubicacion that = (Ubicacion) obj;
		//Comprobamos igualdad de atributos
		return x == that.x && y == that.y;
	}
	
	//Comparacion de objetos Ubicacion
	@Override
    public int compareTo(Ubicacion other) {
		//Primero comparamos coordenada x
    	if (this.x != other.x)                
    		return Integer.compare(this.x, other.x);
    	//Segundo comparamos coordenada y
    	return Integer.compare(this.y, other.y);  
	}
    
	//Funcion que genera un codigo hash en funcion de los valores de x e y
	@Override
	public int hashCode() {
		return Objects.hash(x,y);
	}
}
