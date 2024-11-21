package cartes;

public class Parade extends Bataille {

	public Parade(int nombre, Type type) {
		super(nombre, type);
	}

	
	@Override
	public String toString() {
		switch (super.getType()) {
		case FEU:
			return "FEU VERT";
			
		case ESSENCE:
			return "ESSENCE";
		
		case CREVAISON:
			return "ROUE DE SECOURS";
			
		case ACCIDENT:
			return "REPARATIONS";
			
		default :
			return "erreur";
		}	
		
	}
	
	@Override
	public boolean equals(Object obj) {
		if(obj instanceof Parade) {
			Parade at = (Parade) obj;
			return at.getType() == this.getType();
		}
		return false;
	}
}
