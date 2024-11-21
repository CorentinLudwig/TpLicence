package ilu2;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class WelcomeTest {


	@Test
	void exo1() {
		assertEquals("Hello, Bob", Welcome.welcome("bob"));
	}
	
	@Test
	void exo2() {
		assertEquals("Hello, my friend", Welcome.welcome(""));
		assertEquals("Hello, my friend", Welcome.welcome("\n"));

	}

	@Test
	void exo3() {
		assertEquals("HELLO, JERRY !", Welcome.welcome("JERRY"));
	}
	
	@Test
	void exo4() {
		assertEquals("Hello, Jerry and Bob", Welcome.welcome("jerry,bob"));
	}
	
	@Test
	void exo5() {
		assertEquals("Hello, Jerry, Bob and Amy", Welcome.welcome("jerry,bob,amy"));
	}
	
	@Test
	void exo6() {
		assertEquals("Hello, Amy and Jerry. AND HELLO, BOB !", Welcome.welcome("amy,BOB,jerry"));
		assertEquals("Hello, Amy. AND HELLO, BOB AND JERRY !", Welcome.welcome("amy,BOB,JERRY"));
	}
	
	@Test
	void exo7() {
		assertEquals("Hello, Bob, Amy and Jerry", Welcome.welcome("bob,amy,jerry"));
		assertEquals("Hello, Bob and Jerry. AND HELLO, AMY AND JACK !", Welcome.welcome("bob,AMY,jerry,JACK"));
	}
	
	
	@Test
	void exo8() {
		assertEquals("Hello, Bob, Amy and Jerry", Welcome.welcome("    bob    ,amy,   jerry"));
	}
	
	@Test
	void exo9() {
		// j'ai choisi de mettre le X en majuscule puisque qe cela me semble plus logique  et 
		// que dans l'exemple de l'exigence suivante il est en majuscule
		assertEquals("Hello, Bob (x3) and Amy. AND HELLO, JERRY (X2) !", Welcome.welcome("bob,  JERRY  ,amy,bob,JERRY,bob"));
	}
	
	@Test
	void exo10() {
		assertEquals("Hello, Bob and Amy. AND YODA (X2) AND JERRY, HELLO !", Welcome.welcome("bob, YODA, amy, JERRY, YODA"));
		assertEquals("Bob, Yoda and Amy, Hello. AND HELLO, JERRY !", Welcome.welcome("bob, yoda, amy, JERRY"));

	}
}
