package ilu2;

public class Welcome {

	public static String welcome(String input) {
		if (input.isBlank())
			return "Hello, my friend";
		String[] tabinput = input.split(",");

		String[] tabNom = new String[tabinput.length];
		int[] tabCompte = new int[tabinput.length];
		int n = 0;

		for (int i = 0; i < tabinput.length; i++) {
			tabinput[i] = tabinput[i].trim();
			int e = 0;
			while (e < n && !tabNom[e].equals(tabinput[i])) {
				e++;
			}

			if (e == n) {
				tabNom[n] = tabinput[i];
				tabCompte[n] = 1;
				n++;
			} else {
				tabCompte[e]++;
			}
		}

		return builderWelcome(tabNom, tabCompte, n);
	}

	private static void insertAnd(StringBuilder b) {
		int i = b.lastIndexOf(",");
		if (i > -1) {
			b.replace(i, i + 1, " and");
		}
	}

	private static String builderWelcome(String[] tabNom, int[] tabCompte, int n) {

		StringBuilder lower = new StringBuilder();
		StringBuilder upper = new StringBuilder();
		boolean upperYoda = false;
		boolean lowerYoda = false;

		for (int i = 0; i < n; i++) {

			if (tabNom[i].equals(tabNom[i].toUpperCase())) {
				addName(upper, tabNom[i], tabCompte[i]);
				if(tabNom[i].equalsIgnoreCase("yoda"))
					upperYoda = true;
			} else {
				addName(lower, tabNom[i], tabCompte[i]);
				if(tabNom[i].equalsIgnoreCase("yoda"))
					lowerYoda = true;
			}
		}
		
		
		insertAnd(upper);
		insertAnd(lower);
		
		addHello(upper, upperYoda);
		addHello(lower, lowerYoda);
		
		upper.append(" !");
		
		String upperfinal = upper.toString().toUpperCase();
		if (lower.length() == 7)
			return upperfinal;

		if (upper.length() > 9) {
			lower.append(". AND ");
			lower.append(upperfinal);
		}
		return lower.toString();

	}
	
	private static void addHello(StringBuilder b, boolean yoda) {
		if(yoda) {
			b.append(", Hello");
		} else
			b.insert(0, "Hello, ");
	}


	private static void addName(StringBuilder b, String name, int occurence) {
		if(b.length()>0)
			b.append(", ");
		b.append(name.substring(0, 1).toUpperCase());
		b.append(name.substring(1));
		if (occurence > 1)
			b.append(" (x" + occurence + ")");
	}
}
