package cartes;

import java.util.ArrayList;
import java.util.List;

import cartes.Probleme.Type;
import utils.Utils;

public class JeuDeCartes {
	private Carte[] typesDeCartes = new Carte[] {
			new Botte(1,Type.ACCIDENT),
			new Botte(1,Type.FEU),
			new Botte(1,Type.CREVAISON),
			new Botte(1,Type.ESSENCE),
			
			new Attack(3,Type.ACCIDENT),
			new Attack(5,Type.FEU),
			new Attack(3,Type.CREVAISON),
			new Attack(3,Type.ESSENCE),
			
			new Parade(6,Type.ACCIDENT),
			new Parade(14,Type.FEU),
			new Parade(6,Type.CREVAISON),
			new Parade(6,Type.ESSENCE),
			
			new Borne(10,25),
			new Borne(10,50),
			new Borne(10,75),
			new Borne(12,100),
			new Borne(4,200),
			
			new FinLimite(6),
			new DebutLimite(4)
	};
	
	
	
	public List<Carte> getListCartes() {
		return listCartes;
	}






	private List<Carte> listCartes = new ArrayList<>();
	
	public JeuDeCartes() {
		
		for(Carte cartes : typesDeCartes) {
			for (int i = 0; i < cartes.getNombre(); i++) {
				listCartes.add(cartes);
			}
		}
		
		listCartes = Utils.melanger(listCartes);


	}
	
	
	private int countCartes(Carte cartes){
		int count = 0;
		for (Carte carteList : listCartes) {
			if(cartes.equals(carteList)) {
				count++;
			}
		}
		
		return count;
	}

	
	public boolean checkCount() {
		
		for (Carte cartes : typesDeCartes) {
			if(cartes.getNombre() != countCartes(cartes)) {
				return false;
			}
		}
		return true;	
	}
	
	
	
	
	
	

}
