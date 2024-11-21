package testsFonctionnels;

import java.util.ArrayList;
import java.util.List;

import cartes.Carte;
import cartes.JeuDeCartes;
import utils.Utils;

public class TestJeuDeCartes {
	
	public static void main(String[] args) {
		JeuDeCartes jeu1 = new JeuDeCartes();
		
		for (int i = 0; i < jeu1.getListCartes().size(); i++) {
			System.out.println(jeu1.getListCartes().get(i));
		}
		
		System.out.println(jeu1.checkCount());
		
	
	

	JeuDeCartes jeu = new JeuDeCartes();
	List<Carte> listeCarteNonMelangee = jeu.getListCartes();
	List<Carte> listeCartes = new ArrayList<>(listeCarteNonMelangee);
	System.out.println(listeCartes);
	listeCartes = Utils.melanger(listeCartes);
	System.out.println(listeCartes);
	System.out.println("liste mélangée sans erreur ? "
	+ Utils.verifierMelange(listeCarteNonMelangee, listeCartes));
	listeCartes = Utils.rassembler(listeCartes);
	System.out.println(listeCartes);
	System.out.println("liste rassemblée sans erreur ? " + Utils.verifierRassemblement(listeCartes));

	}
}
