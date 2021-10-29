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
	
	// All-c-uses/Some-p-uses

	// -> line 
	// É definido em 1 e possui c-use apenas em 3.
	// 1) 1-2-3-4-5

	// -> posicaoPeca
	// É definido em 3 e possui c-use apenas em 6.
	// 2) 1-2-3-4-6-7

	// All-p-uses

	// -> line
	// É definido em 1 e possui p-uses nas arestas (2,3) e (2,8)
	// 3) 1-2-8
	// 4) 1-2-3-4-5

	// -> posicaoPeca
	// É definido em 3 e possui p-uses nas arestas (4, 5) e (4,6)
	// 5) 1-2-3-4-5
	// 6) 1-2-3-4-6-7


	private PuzzleGame puzzleGame;

	@BeforeEach
	void setup() {
		ShufflePuzzleForTesting shuffler = new ShufflePuzzleForTesting();
		puzzleGame = new PuzzleGame(3, shuffler);
	}

	// Caminho 1, 4 e 5
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

	// Caminho 2 e 6
	@Test
	void testValidPiecePosition() {
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

	// Caminho 3
	@Test
	void testInvalidPiecePosition() {
		//Fixture Setup
		
		//Exercise SUT
		assertThrows(ExceptionInvalidPosition.class,() -> {
			puzzleGame.getTile(-1, 1);;
		});
		
		//Result Verification
		
		//Fixture Teardown
	}


}
