package utils;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;
import java.util.Random;

public class Utils {

	private static Random rng = new Random();

	 public static <T> T extraire(List<T> list) { // @to do extraire with iterator
		int rand = rng.nextInt(list.size());
		return list.remove(rand);
	}

	 public static <T> List<T> melanger(List<T> list) {
		List<T> listRetour = new ArrayList<>();

		while (!list.isEmpty()) {
			listRetour.add(extraire(list));
		}
		return listRetour;
	}

	 public static <T> boolean verifierMelange(List<T> list1, List<T> list2) {
		if (list1.size() != list2.size())
			return false;
		for (T t : list1) {
			if (Collections.frequency(list1, t) != Collections.frequency(list2, t)) {
				return false;
			}

		}
		return true;
	}

	 public static <T> List<T> rassembler(List<T> list) {
		List<T> listRetour = new ArrayList<>();

		for (T t : list) {
			if (!listRetour.contains(t)) {
				for (int i = 0; i < Collections.frequency(list, t); i++) {
					listRetour.add(t);
				}

			}

		}
		return listRetour;
	}

	 private static <T> boolean hasElements(List<T> list, T cur, int indice) {
		for (ListIterator<T> iterator2 = list.listIterator(indice); iterator2.hasNext();) {
			T t = iterator2.next();
			if (t.equals(cur)) {
				return true;
			}
		}
		return false;
	}

	 public static <T> boolean verifierRassemblement(List<T> list) {
		T precendent = list.get(0);
		T current;
		for (Iterator<T> iterator = list.iterator(); iterator.hasNext();) {
			current = iterator.next();
			if (!current.equals(precendent)) {
				if (hasElements(list, precendent, list.indexOf(current))) {
					return false;

				}
			}
			precendent = current;
		}

		return true;
	}

}
