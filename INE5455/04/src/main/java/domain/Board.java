package domain;

import exceptions.ExceptionInvalidPosition;

public class Board { 
	
	private int line, column;
	
	private Grid<Cell> grid;
	
	public Board(int line, int column) {
		this.line = line;
		this.column = column;
		this.grid = new Grid<Cell>(line, column);
		this.initializeAllCells();
	}

	private void initializeAllCells(){	
		for (int i=1; i<=this.line; i++) 
			for (int j=1; j<=this.column; j++)
				this.grid.put(i, j, new Cell());
	}
	
	public void putTile(Tile tile, Position position) {
		this.grid.get(position.getLine(), position.getColumn()).putTile(tile);
	}
	
	public Tile getTile(Position position) {
		if (this.isInsideTheBoard(position)){
			return grid.get(position.getLine(), position.getColumn()).tile;
		}	
		return null;
	}
	
	public boolean isAdjacent(Position position1, Position position2) {
		return ((this.isInsideTheBoard(position1) && this.isInsideTheBoard(position2)) &&
		       ((position1.getLine() == position2.getLine() &&
			     Math.abs(position1.getColumn() - position2.getColumn()) == 1) ||
			    (position1.getColumn() == position2.getColumn() &&
			     Math.abs(position1.getLine() - position2.getLine()) == 1)));
	}
	
	public boolean changeTilesInPositions(Position position1, Position position2) throws ExceptionInvalidPosition{
		if (this.isInsideTheBoard(position1) && this.isInsideTheBoard(position2)) {
			Cell cell1 = this.grid.get(position1.getLine(), position1.getColumn());
			Cell cell2 = this.grid.get(position2.getLine(), position2.getColumn());
			Tile aux = cell1.getTile();
			cell1.putTile(cell2.getTile());
			cell2.putTile(aux);
			return true;
		}
		else 
			throw new ExceptionInvalidPosition();
	}
	
	public boolean isInTheSuperiorBorder(Position position) {
		return (this.isInsideTheBoard(position) && position.getLine() == 1);
	}
	
	public boolean isInTheBottomBorder(Position position) {
		return (this.isInsideTheBoard(position) && position.getLine() == this.line);
	}
	
	public boolean isInTheLeftBorder(Position position) {
		return (this.isInsideTheBoard(position) && position.getColumn() == 1);
	}
	
	public boolean isInTheRightBorder(Position position) {
		return (this.isInsideTheBoard(position) && position.getColumn() == this.column);
	}
	
	private boolean isInsideTheBoard(Position position) {
		return position.getLine()>0 && position.getLine()<=this.line &&
			   position.getColumn()>0 && position.getColumn()<=this.column;
	}
	
	public boolean equals(Object board) {
		if (this.line != ((Board)board).line || this.column != ((Board)board).column)
			return false;
		for (int i=1; i<=this.line; i++) 
			for (int j=1; j<=this.column; j++)
				if (!this.grid.get(i,j).equals(((Board)board).grid.get(i,j)))
					return false;
		return true;
	}

	public String toString() {
		String result = "";
		for (int i=1; i<=this.line; i++) {
			for (int j=1; j<=this.column; j++) {
				Cell cell = grid.get(i, j);
				result = result + "("+i+","+j+")"+":" + cell.toString() + "   "; 
		    }
			result = result + "\n";
		}	
		return result;	
	}

}
