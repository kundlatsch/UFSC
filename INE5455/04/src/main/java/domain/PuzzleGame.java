package domain;

import java.util.*;

import exceptions.ExceptionInvalidPosition;

public class PuzzleGame implements Puzzle{
	
	private final SquareBoard board;
	private final Map<Tile,Position> positionsOfTiles;
	private Position emptyCell;
	private final SquareBoard boardWithFinalState;
	

	public PuzzleGame(int dimension, StrategyShufflePuzzle shuffler){
		this.boardWithFinalState = new SquareBoard (dimension);
		this.board = new SquareBoard(dimension);
		this.positionsOfTiles = new HashMap<Tile,Position>();
		
		List<Tile> listOfTiles = this.generateListOfTiles (dimension*dimension-1);
		
		this.putTilesInTheBoard (boardWithFinalState, listOfTiles);
		
		this.putTilesInTheBoard (board, listOfTiles);		
		this.putTilesInTheMapOfPositions (positionsOfTiles, listOfTiles, dimension);
		this.emptyCell = new Position(dimension,dimension);
		
		shuffler.shuffle(this); 
		
	}

	public String getTile(int line, int column) throws ExceptionInvalidPosition {
        if (line>0 && line<=this.board.getDimension() && column>0 && column<=board.getDimension()) {
            Position posicaoPeca = new Position(line, column);
            if (posicaoPeca.getLine()==emptyCell.getLine() &&
                posicaoPeca.getColumn()==emptyCell.getColumn()) {
                return (" ");
            }
            else {
                Tile peca = board.getTile(posicaoPeca);
                return peca.toString();
            }
        }
        else 
            throw new ExceptionInvalidPosition();
    }
		
	private void putTilesInTheBoard(SquareBoard t, List<Tile> list) {
		Iterator<Tile> tilesListIterator = list.iterator();		
		//from first line to the line before the last
		for (int line=1; line<t.getDimension(); line++){
			for (int column=1; column<=t.getDimension(); column++){
				Tile tile = tilesListIterator.next();
				Position position = new Position(line,column);
				t.putTile(tile, position);
				this.positionsOfTiles.put(tile, position);
			}	
		}
		//last line
		for (int columm=1; columm<t.getDimension(); columm++) {
			Tile tile = tilesListIterator.next();
			Position position = new Position(t.getDimension(),columm);
			t.putTile(tile, position);
			this.positionsOfTiles.put(tile, position);
		}
	}
	
	private void putTilesInTheMapOfPositions (Map<Tile,Position> positions, List<Tile> list, int dimension) {
		Iterator<Tile> tilesListIterator = list.iterator();		
		//from first line to the line before the last
		for (int line=1; line<dimension; line++){
			for (int column=1; column<=dimension; column++){
				Tile tile = tilesListIterator.next();
				Position position = new Position(line,column);
				positions.put(tile, position);
			}	
		}	
		//last line
		for (int column=1; column<dimension; column++) {
			Tile tile = tilesListIterator.next();
			Position position = new Position (dimension,column);
			positions.put(tile, position);
		}
	}
	
	
	//coloquei como package	
	List<Tile> generateListOfTiles(int quantityOfTiles) {
		List<Tile> listOfTiles = new ArrayList<Tile>();
		for (int i=1; i<=quantityOfTiles; i++)
			listOfTiles.add(new Tile(i));
		return listOfTiles;
	}
				
	public void moveTile(int tileNumber) throws ExceptionInvalidPosition {
		Position tilePosition = this.positionsOfTiles.get(new Tile(tileNumber));
		if (tilePosition!=null && (board.isAdjacent(tilePosition, this.emptyCell))){
			if (board.changeTilesInPositions(tilePosition, this.emptyCell)){
				this.positionsOfTiles.put(new Tile(tileNumber), this.emptyCell);
				this.emptyCell = tilePosition;
			}	
		}
	}
	
	public void moveTile(int line, int column) throws ExceptionInvalidPosition {
		if (line>0 && line<=this.board.getDimension() && column>0 && column<=board.getDimension()) {
			Position posicaoPeca = new Position(line, column);
			if (board.isAdjacent(posicaoPeca, this.emptyCell)){
				Tile peca = board.getTile(posicaoPeca);
				if (board.changeTilesInPositions(posicaoPeca, this.emptyCell)){
					this.positionsOfTiles.put(peca, this.emptyCell);
					this.emptyCell = posicaoPeca;
				}	
			}
		}
	}
	
	public boolean moveEmptyCell(Direction direction) {
		if (direction == Direction.DOWN) {
			return this.moveEmptyCellDown();
		}
		else if (direction == Direction.UP)
				return this.moveEmptyCellUp();
			else
				if (direction == Direction.RIGHT)
					return this.moveEmptyCellRight();
				else
					if (direction == Direction.LEFT)
						return this.moveEmptyCellLeft();
		return false;
	}

	private boolean moveEmptyCellDown() {
		if (this.board.isInTheBottomBorder(this.emptyCell))
			return false;
		else {
			Position tilePosition = new Position (this.emptyCell.line+1, this.emptyCell.column);
			Tile tile = board.getTile(tilePosition);
			try{
				board.changeTilesInPositions(tilePosition, this.emptyCell); }
			catch (ExceptionInvalidPosition exception){
			}
			this.positionsOfTiles.put(tile, this.emptyCell);
			this.emptyCell = tilePosition;
			return true;
		}		
	}
	
	private boolean moveEmptyCellUp() {
		if (this.board.isInTheSuperiorBorder(this.emptyCell))
			return false;
		else {
			Position tilePosition = new Position (this.emptyCell.line-1, this.emptyCell.column);
			Tile tile = board.getTile(tilePosition);
			try{ 
				board.changeTilesInPositions(tilePosition, this.emptyCell);}
			catch (ExceptionInvalidPosition exception){
			}
			this.positionsOfTiles.put(tile, this.emptyCell);
			this.emptyCell = tilePosition;
			return true;
		}		
	}
	
	private boolean moveEmptyCellRight() {
		if (this.board.isInTheRightBorder(this.emptyCell))
			return false;
		else {
			Position tilePosition = new Position (this.emptyCell.line, this.emptyCell.column+1);
			Tile tile = board.getTile(tilePosition);
			try{
				board.changeTilesInPositions(tilePosition, this.emptyCell);}
			catch (ExceptionInvalidPosition exception){
			}
			this.positionsOfTiles.put(tile, this.emptyCell);
			this.emptyCell = tilePosition;
			return true;
		}		
	}
	
	private boolean moveEmptyCellLeft() {
		if (this.board.isInTheLeftBorder(this.emptyCell))
			return false;
		else {
			Position tilePosition = new Position (this.emptyCell.line, this.emptyCell.column-1);
			Tile tile = board.getTile(tilePosition);
			try{
				board.changeTilesInPositions(tilePosition, this.emptyCell);}
			catch (ExceptionInvalidPosition exception){
				}
			this.positionsOfTiles.put(tile, this.emptyCell);
			this.emptyCell = tilePosition;
			return true;
		}		
	}
	
	public boolean endOfTheGame() {
		return this.board.equals(this.boardWithFinalState);
	}
	
	public String toString(){
		return this.board.toString();	
	}
	
}
