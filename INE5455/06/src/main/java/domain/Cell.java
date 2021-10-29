package domain;

public class Cell {
	
	//int line, column;
	Tile tile;
	
	public Cell(){
		
	}
	
	public void putTile(Tile tile) {
		this.tile = tile;
	}
	
	public Tile removeTile() {
		Tile aux = this.tile;
		this.tile = null;
		return aux;
	}
	
	public Tile getTile() {
		return this.tile;
	}
	
	public boolean equals(Object cell) {
		if (this.tile == null && ((Cell)cell).tile == null)
			return true;
		else
			if (this.tile == null || ((Cell)cell).tile == null)
				return false;
			else
				return this.tile.equals(((Cell)cell).tile);
	}
	
	public String toString(){
		 return (this.tile == null ? "" : tile.toString());
	}

	
}
