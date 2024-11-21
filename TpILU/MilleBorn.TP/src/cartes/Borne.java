package cartes;

import jeu.Joueur;

public class Borne extends Carte {
	private int km;

	public Borne(int nombre, int km) {
		super(nombre);
		this.km = km;
	}
	
	public void setKm(int km) {
		this.km = km;
	}
	
	
	public int getKm() {
		return km;
	}
	
	@Override
	public String toString() {
		StringBuilder str= new StringBuilder();;
		str.append(km);
		str.append("km");
		return str.toString();
	}
	
	
	@Override
	public boolean equals(Object obj) {
		if(obj instanceof Borne) {
			Borne at = (Borne) obj;
			return at.getKm() == this.getKm();
		}
		return false;
	}

	@Override
	public boolean appliquer(Joueur j) {
		if(!j.estBloque() && j.getLimite()>this.getKm() && j.getKM()+getKm()<=1000) {
			j.getPileBorne().add(this);
			return true;
		}
		return false;
	}
}
