package domain;

import exceptions.ExceptionInvalidPosition;

public class ShufflePuzzleForTesting implements StrategyShufflePuzzle{
		
	public ShufflePuzzleForTesting() {
		
	}
	
	public void shuffle (Puzzle game){
		try{
			game.moveTile(2, 3);
			game.moveTile(2, 2);
			game.moveTile(1, 2);
			//1 - 3
			//4 2 5
			//7 8 6
		}
		catch (ExceptionInvalidPosition e){
		}
	}

}
