package domain;

import exceptions.ExceptionInvalidPosition;

public interface Puzzle {

	public enum Direction {
		UP, LEFT, DOWN, RIGHT
	};

	public boolean moveEmptyCell(Direction dir) ;

	public void moveTile(int line, int column) throws ExceptionInvalidPosition;
	
	public void moveTile(int tileNumber) throws ExceptionInvalidPosition;

	public boolean endOfTheGame();
	
}
