package cartes;

import jeu.Joueur;

public abstract class Carte {
	private int nombre;

	protected Carte(int nombre) {
		super();
		this.nombre = nombre;
	}

	public int getNombre() {
		return nombre;
	}

	public void setNombre(int nombre) {
		this.nombre = nombre;
	}

	public abstract boolean appliquer(Joueur j);
}
