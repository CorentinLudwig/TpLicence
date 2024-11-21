package jeu;

import java.util.ConcurrentModificationException;
import java.util.Iterator;
import java.util.NoSuchElementException;

import cartes.Carte;

public class Sabot implements Iterable<Carte> {
	private Carte[] tabCartes;
	private int nbCartes = 0;
	private int nombreOperations = 0;

	public Sabot(int tailleSabot) {
		tabCartes = new Carte[tailleSabot];
	}

	public boolean estVide() {
		return nbCartes == 0;

	}

	public void ajouterCarte(Carte carte) {
		if (nbCartes < tabCartes.length) {
			tabCartes[nbCartes] = carte;
			nbCartes++;
			nombreOperations++;
		} else {
			throw new IndexOutOfBoundsException();
		}
	}

	public void ajouterFamilleCarte(Carte carte) {
		for (int i = 0; i < carte.getNombre(); i++) {
			ajouterCarte(carte);
		}
	}

	public void ajouterFamilleCarte(Carte... cartes) {
		for (Carte carte : cartes) {
			for (int i = 0; i < carte.getNombre(); i++) {
				ajouterCarte(carte);
			}
		}
	}

	@Override
	public Iterator<Carte> iterator() {
		return new Iterateur();
	}

	public Carte piocher() {
		Iterator<Carte> ite = iterator();
		Carte c = ite.next();
		ite.remove();
		return c;
	}

	private class Iterateur implements Iterator<Carte> {
		private int indiceIterateur = 0;
		private int nombreOperationsReference = nombreOperations;
		private boolean nextEffectue = false;

		public boolean hasNext() {
			return indiceIterateur < nbCartes;
		}

		public Carte next() {
			verificationConcurrence();
			if (hasNext()) {
				Carte carte = tabCartes[indiceIterateur];
				indiceIterateur++;
				nextEffectue = true;
				return carte;
			} else {
				throw new NoSuchElementException();
			}
		}

		@Override
		public void remove() {
			verificationConcurrence();
			if (nextEffectue && nbCartes > 0) {
				for (int i = indiceIterateur - 1; i < nbCartes - 1; i++) {
					tabCartes[i] = tabCartes[i + 1];

				}
				nbCartes--;
				nombreOperations++;
				nombreOperationsReference++;
				indiceIterateur--;
			} else {
				throw new IllegalStateException();
			}
		}

		private void verificationConcurrence() {
			if (nombreOperations != nombreOperationsReference) {
				throw new ConcurrentModificationException();
			}
		}
	}
}
