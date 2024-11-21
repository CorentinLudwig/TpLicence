package testsFonctionnels;

import java.util.ArrayList;
import java.util.List;

import cartes.Attack;
import cartes.Bataille;
import cartes.Borne;
import cartes.Botte;
import cartes.Carte;
import cartes.Parade;
import cartes.Probleme.Type;
import jeu.Joueur;

public class TestJoueur {
	
	public static void main(String[] args) {
		Joueur joueur = new Joueur("bob");
		Borne borne = new Borne(1, 50);
		Bataille feuVert = new Parade(1, Type.FEU);
		Bataille feuRouge = new Attack(1, Type.FEU);
		Botte prioritaire = new Botte(1, Type.FEU);
		
		List<Borne> listborne = joueur.getPileBorne();
		
		for (int i = 0; i < 5; i++) {
			listborne.add(borne);
		}
		
		System.out.println(joueur.getKM());
		
		System.out.println(joueur.estBloque());
		joueur.getPileBataille().add(feuVert);
		System.out.println(joueur.estBloque());
		joueur.getPileBataille().add(feuRouge);
		System.out.println(joueur.estBloque());
		joueur.getBottes().add(prioritaire);
		System.out.println(joueur.estBloque());
	}
	

}
