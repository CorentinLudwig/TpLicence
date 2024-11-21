package cartes;

import jeu.Joueur;

public class Botte extends Probleme {

	public Botte(int nombre, Type type) {
		super(nombre, type);
		// TODO Auto-generated constructor stub
	}

	@Override
	public String toString() {
		switch (super.getType()) {
		case FEU:
			return "VÉHICULE PRIORITAIRE";
			
		case ESSENCE:
			return "CITERNE D’ESSENCE";
		
		case CREVAISON:
			return "INCREVABLE";
			
		case ACCIDENT:
			return "AS DU VOLANT";
			
		default :
			return "erreur";
		}	
		
	}
	
	@Override
	public int hashCode() {
		return 31*getType().hashCode();
	}
	
	@Override
	public boolean equals(Object obj) {
		if(obj instanceof Botte at) {
			return at.getType() == this.getType();
		}
		return false;
	}

	@Override
	public boolean appliquer(Joueur j) {
		j.getBottes().add(this);
		
		return true;
	}
}
