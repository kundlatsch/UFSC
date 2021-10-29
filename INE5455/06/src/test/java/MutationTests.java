package domain;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import domain.*;
import domain.Puzzle.Direction;
import exceptions.ExceptionInvalidPosition;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.LinkedList;

public class MutationTests {

	// Testes anteriores

	private PuzzleGame puzzleGame;

	@BeforeEach
	void setup() {
		ShufflePuzzleForTesting shuffler = new ShufflePuzzleForTesting();
		puzzleGame = new PuzzleGame(3, shuffler);
	}

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

	// --------------------------------------------------------------------- //

	// 5 Mutantes que foram mortos apenas com os testes que já haviam sido criados:

	// 1
	// Board.java
	// Mutação: linha 29 -> negar condicional
	// Morto pelo teste testValidPiecePosition

	// 2
	// Board.java
	// Duas Mutações: linha 38 -> negar condicional e trocar soma por subtração
	// Mortos pelo teste testEmptyPosition

	// 3
	// PuzzleGame.java
	// Mutação: linha 28 -> remover chamada ao método shuffle
	// Morto pelo teste testEmptyPosition

	// 4
	// PuzzleGame.java
	// Mutação: linha 55 -> remover chamada ao método putTile
	// Morto pelo teste testValidPiecePosition

	// 5
	// SquareBoard.java
	// Mutação: linha 13 -> trocar o retorno por 0
	// Morto pelo teste testValidPiecePosition

	// Testes novos (após teste de mutação)

	// Board.java
	// Mutação: linha 36 -> trocar o retorno do método isAdjacent por true
	@Test
	void testNotAdjacentPiece() {
		//Fixture Setup
		String result;

		//Exercise SUT
		try {
			puzzleGame.moveTile(3, 3);
		} catch (ExceptionInvalidPosition e) {}

		//Result Verification
		try {
			result = puzzleGame.getTile(3, 3);
		} catch (ExceptionInvalidPosition e) {
			result = e.toString();
		}
		assertEquals("6", result);
		
		//Fixture Teardown
	}

	// PuzzleGame.java
	// Mutação: linha 33 -> trocar o condicional (3 mutantes)

	@Test
	void testPieceColumnGreaterThanDimension() {
		//Fixture Setup
		
		//Exercise SUT
		assertThrows(ExceptionInvalidPosition.class,() -> {
			puzzleGame.getTile(1, 4);;
		});
		
		//Result Verification
		
		//Fixture Teardown
	}

	@Test
	void testPieceColumnLowerThanDimension() {
		//Fixture Setup
		
		//Exercise SUT
		assertThrows(ExceptionInvalidPosition.class,() -> {
			puzzleGame.getTile(2, 0);;
		});
		
		//Result Verification
		
		//Fixture Teardown
	}

	@Test
	void testPieceLineLowerThanDimension() {
		//Fixture Setup
		
		//Exercise SUT
		assertThrows(ExceptionInvalidPosition.class,() -> {
			puzzleGame.getTile(0, 1);;
		});
		
		//Result Verification
		
		//Fixture Teardown
	}



	// Board.java
	// Mutação: linha 96 -> trocar o retorno do método toString por ""
	// @Test
	// void testPuzzleGameToString() {
	// 	//Fixture Setup
	// 	String result;

	// 	//Exercise SUT
	// 	result = puzzleGame.toString();

	// 	//Result Verification
	// 	String expected = 
	// 		"(1,1):1   (1,2):   (1,3):3   \n" + 
	// 		"(2,1):4   (2,2):2   (2,3):5   \n" +
	// 		"(3,1):7   (3,2):8   (3,3):6   \n";
	// 	assertEquals(expected, result);
		
	// 	//Fixture Teardown
	// }


	// PuzzleGame.java
	// Mutação: linha 150 -> negar condicional
	// @Test
	// void testTopBorder() {
	// 	//Fixture Setup
	// 	String result;
		
	// 	//Exercise SUT
	// 	puzzleGame.moveEmptyCell(Direction.UP);
		
	// 	//Result Verification
	// 	try {
	// 		result = puzzleGame.getTile(1, 2);
	// 	} catch (ExceptionInvalidPosition e) {
	// 		result = e.toString();
	// 	}
	// 	assertEquals(" ", result);
		
	// 	//Fixture Teardown
	// }

	// PuzzleGame.java
	// Mutação: linha 129 -> trocar retorno por false
	// @Test
	// void testLeftMovement() {
	// 	//Fixture Setup
	// 	Boolean result;
		
	// 	//Exercise SUT
	// 	result = puzzleGame.moveEmptyCell(Direction.LEFT);
		
	// 	//Result Verification
	// 	assertEquals(true, result);
		
	// 	//Fixture Teardown
	// }

	// PuzzleGame.java
	// Mutação: linha 198 -> trocar retorno por true
	// @Test
	// void testNotEndOfTheGame() {
	// 	//Fixture Setup
	// 	Boolean result;
		
	// 	//Exercise SUT
	// 	result = puzzleGame.endOfTheGame();
		
	// 	//Result Verification
	// 	assertEquals(false, result);
		
	// 	//Fixture Teardown
	// }

}
