package jeu;

import cartes.Attack;
import cartes.Carte;
import cartes.DebutLimite;

public class Coup {
	private Carte carte;
	private Joueur joueur;
	
	
	
	public Coup(Carte carte, Joueur joueur) {

		this.carte = carte;
		this.joueur = joueur;
	}
	
	
	
	
	public Carte getCarte() {
		return carte;
	}


	public Joueur getJoueur() {
		return joueur;
	}


	public boolean estValide(Joueur j) {
		return ((carte instanceof Attack) || (carte instanceof DebutLimite)) && j.equals(joueur);
	}
	
	@Override
	public boolean equals(Object obj) {
		if(obj instanceof Coup coup) {
			return coup.getCarte()==this.getCarte() && coup.getJoueur().equals(this.getJoueur());
		}
		return false;
	}
	
	@Override
	public int hashCode() {
		// TODO Auto-generated method stub
		return getCarte().hashCode()*(getJoueur().hashCode())*31;
	}
}












