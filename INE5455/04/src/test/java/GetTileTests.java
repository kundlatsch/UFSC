package domain;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import domain.*;
import exceptions.ExceptionInvalidPosition;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.LinkedList;

public class GetTileTests {


	// Caminhos (consulte o diagrama em anexo)
	
	// Cobertura de Comandos:
	// Caminho 1: 1-2(T)-3(T)-4(T)-5(T)-6-7(T)-8(T)-9
	// Caminho 2: 1-2(T)-3(T)-4(T)-5(T)-6-7(F)-11-12
	// Caminho 3: 1-2(F)-10

	// Cobertura de Ramos:
	// Caminho 4: 1-2(F)-10
	// Caminho 5: 1-2(T)-3(F)-10
	// Caminho 6: 1-2(T)-3(T)-4(F)-10
	// Caminho 7: 1-2(T)-3(T)-4(T)-5(F)-10
	// Caminho 8: 1-2(T)-3(T)-4(T)-5(T)-6-7(F)-11-12
	// Caminho 9: 1-2(T)-3(T)-4(T)-5(T)-6-7(T)-8(F)-11-12
	// Caminho 10: 1-2(T)-3(T)-4(T)-5(T)-6-7(T)-8(T)-9


	private PuzzleGame puzzleGame;

	@BeforeEach
	void setup() {
		ShufflePuzzleForTesting shuffler = new ShufflePuzzleForTesting();
		puzzleGame = new PuzzleGame(3, shuffler);
	}

	// Caminho 1 e 10
	@Test
	void testEmptyPosition() {
		//Fixture Setup
		String result;

		//Exercise SUT
		try {
			result = puzzleGame.getTile(1, 2);
		} catch (ExceptionInvalidPosition e) {
			result = e.toString();
		}
		
		//Result Verification
		assertEquals(" ", result);
		
		//Fixture Teardown
	}

	// Caminho 9
	@Test
	void testValidPiecePositionOnEmptyLine() {
		//Fixture Setup
		String result;

		//Exercise SUT
		try {
			result = puzzleGame.getTile(1, 3);
		} catch (ExceptionInvalidPosition e) {
			result = e.toString();
		}
		
		//Result Verification
		assertEquals("3", result);
		
		//Fixture Teardown
	}

	// Caminho 3 e 4
	@Test
	void testNegativeLine() {
		//Fixture Setup
		
		//Exercise SUT
		assertThrows(ExceptionInvalidPosition.class,() -> {
			puzzleGame.getTile(-1, 1);;
		});
		
		//Result Verification
		
		//Fixture Teardown
	}

	// Caminho 5
	@Test
	void testOverDimensionLine() {
		//Fixture Setup
		
		//Exercise SUT
		assertThrows(ExceptionInvalidPosition.class,() -> {
			puzzleGame.getTile(4, 1);;
		});
		
		//Result Verification
		
		//Fixture Teardown
	}

	// Caminho 6
	@Test
	void testNegativeCloumn() {
		//Fixture Setup
		
		//Exercise SUT
		assertThrows(ExceptionInvalidPosition.class,() -> {
			puzzleGame.getTile(1, -1);;
		});
		
		//Result Verification
		
		//Fixture Teardown
	}

	// Caminho 7
	@Test
	void testOverDimensionColumn() {
		//Fixture Setup
		
		//Exercise SUT
		assertThrows(ExceptionInvalidPosition.class,() -> {
			puzzleGame.getTile(1, 4);;
		});
		
		//Result Verification
		
		//Fixture Teardown
	}

	// Caminho 2 e 8
	@Test
	void testValidPiecePositionOnEmptyColumn() {
		//Fixture Setup
		String result;

		//Exercise SUT
		try {
			result = puzzleGame.getTile(2, 2);
		} catch (ExceptionInvalidPosition e) {
			result = e.toString();
		}
		
		//Result Verification
		assertEquals("2", result);
		
		//Fixture Teardown
	}


}
