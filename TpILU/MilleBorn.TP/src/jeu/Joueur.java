package jeu;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import cartes.Attack;
import cartes.Bataille;
import cartes.Borne;
import cartes.Botte;
import cartes.Carte;
import cartes.FinLimite;
import cartes.Limite;
import cartes.Parade;
import cartes.Probleme.Type;

public class Joueur {
	private String nom;
	private List<Limite> pileLimite = new ArrayList<>();
	private List<Bataille> pileBataille = new ArrayList<>();
	private List<Borne> pileBorne = new ArrayList<>();
	private Set<Botte> bottes = new HashSet<>();
	private MainAsListe main = new MainAsListe();
	
	
	
	
	public Joueur(String nom) {
		super();
		this.nom = nom;
	}
	
	public String getNom() {
		return nom;
	}
	public MainAsListe getMain() {
		return main;
	}
	public List<Limite> getPileLimite() {
		return pileLimite;
	}
	public List<Bataille> getPileBataille() {
		return pileBataille;
	}
	public List<Borne> getPileBorne() {
		return pileBorne;
	}
	public Set<Botte> getBottes() {
		return bottes;
	}
	
	
	
	@Override
	public boolean equals(Object obj) {
		if(obj == null) {
			return false;
		}
		if (obj instanceof Joueur joueur2) {
			return joueur2.getNom() == this.getNom();
		}
		return super.equals(obj);
		
	}
	
		
	@Override
	public int hashCode() {
		
		return 31*getNom().hashCode();
	}
		
	@Override
	public String toString() {
		return this.getNom();
	}

		
	public void donner(Carte carte) {
		main.prendre(carte);
	}
		
		
	public Carte prendreCarte(List<Carte> sabot) {
		if(sabot.isEmpty()) {
			return null;
		}
		Carte carte = sabot.get(0);
		sabot.remove(carte);
		donner(carte);
		return carte;
	}
		
	
	public int getKM() {
		int km = 0;
		for (Borne borne : pileBorne) {
			km += borne.getKm();
		}
		
		return km;
	}
		
	public int getLimite() {
		
		if(pileBataille.isEmpty()) {
			return 200;
		}
		
		if(bottes.contains(new Botte(1,Type.FEU))) {
			return 200;
		}
		
		Limite limite = pileLimite.get(pileLimite.size()-1);
		
		if (bottes instanceof FinLimite) {
			return 200;
		}
		
		return 50;
		
	}
		

	public boolean estBloque() {
		boolean estPrioritaire = bottes.contains(new Botte(1, Type.FEU));
		if(pileBataille.isEmpty()) {
			return estPrioritaire;
		}
		Bataille sommet = pileBataille.get(pileBataille.size()-1);
		if(sommet instanceof Parade parade && (estPrioritaire || parade.getType()==Type.FEU)) {
			return false;
		}
		if(sommet instanceof Attack attack && bottes.contains(new Botte(1, attack.getType()))) {
			return false;
		}
		return true;
	}
	
	public Set<Coup> coupsPossibles(List<Joueur> participants){
		Set<Coup> listCoup = new HashSet<>();
		
		for (Joueur joueur : participants) {
			for (Carte carte : main) {
				Coup coup = new Coup(carte, joueur);
				if(coup.estValide(this))
					listCoup.add(coup);
			}
		}
		
		return listCoup;
	}
	
	
	public Set<Coup> coupsParDefault(){
		Set<Coup> listCoup = new HashSet<>();
		
			for (Carte carte : main) {
				Coup coup = new Coup(carte, null);
				if(coup.estValide(this))
					listCoup.add(coup);
			}
		
		return listCoup;
	}
}




























