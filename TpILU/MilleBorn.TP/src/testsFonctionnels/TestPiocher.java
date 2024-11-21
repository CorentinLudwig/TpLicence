package testsFonctionnels;

import cartes.Attack;
import cartes.Borne;
import cartes.Botte;
import cartes.Parade;
import cartes.Probleme.Type;

public class TestPiocher {
	
	public static void main(String[] args) {
		Attack acc = new Attack(1,Type.ACCIDENT);
		Parade rep = new Parade(1,Type.ACCIDENT);
		Botte ace = new Botte(1,Type.ACCIDENT);
		Botte ace2 = new Botte(1,Type.ACCIDENT);

		Attack feu = new Attack(1, Type.FEU);
		Borne b100 = new Borne(1, 100);
		Borne b50 = new Borne(1, 50);
		
		
		System.out.println(acc.equals(rep));
		System.out.println(acc.equals(feu));
		System.out.println(ace.equals(rep));
		System.out.println(b100.equals(b50));
		System.out.println(ace.equals(ace2));



	}

}
