package testsFonctionnels;

import java.util.Iterator;

import cartes.Attack;
import cartes.Botte;
import cartes.Carte;
import cartes.Parade;
import cartes.Probleme.Type;
import jeu.Sabot;

public class Testsabot {
	
	public static void main(String[] args) {
		
		Sabot sab = new Sabot(10);
		sab.ajouterFamilleCarte(
				new Attack(3, Type.ACCIDENT),
				new Parade(3, Type.ACCIDENT),
				new Botte(1, Type.ACCIDENT)
				);
		
		
//		while(!sab.estVide()) {
//			System.out.println(sab.piocher());
//		}
		
		
		
		
		for(Iterator<Carte> ite = sab.iterator();ite.hasNext();) {
			System.out.println(ite.next());
			ite.remove();
		}
		
		sab.ajouterCarte(new Botte(1, Type.ACCIDENT));
		System.out.println(sab.piocher());
	}

}
