package jeu;

import cartes.Carte;

public interface Main extends Iterable<Carte>{
	
	void prendre(Carte carte);
	void jouer(Carte carte);

}
