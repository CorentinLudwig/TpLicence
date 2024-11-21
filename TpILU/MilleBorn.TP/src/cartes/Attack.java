package cartes;

import jeu.Joueur;

public class Attack extends Bataille {

	public Attack(int nombre, Type type) {
		super(nombre, type);
	}
	
	@Override
	public String toString() {
		switch (super.getType()) {
		case FEU:
			return "FEU ROUGE";
			
		case ESSENCE:
			return "PANNE D'ESSENCE";
		
		case CREVAISON:
			return "CREVAISON";
			
		case ACCIDENT:
			return "ACCIDENT";
			
		default :
			return "erreur";
		}	
		
	}
	
	@Override
	public boolean equals(Object obj) {
		if(obj instanceof Attack at) {
			return at.getType() == this.getType();
		}
		return false;
	}

	
	
}
