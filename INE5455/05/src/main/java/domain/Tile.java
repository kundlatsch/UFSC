package domain;

public class Tile {

	int number;
	int hash;
	
	public Tile (int number){
		this.number = number;
		this.hash = new Integer(this.number).hashCode();
	}
	
	public int getNumber() {
		return this.number;
	}
	
	public boolean biggerThan(Tile otherTile) {
		return this.number > otherTile.number;
	}
	
	@Override
	public boolean equals (Object tile){
		return (this.number == ((Tile) tile).number);
	}
	
	@Override
	public int hashCode() {
		return hash;
	}
	
	@Override
	public String toString(){
		return String.valueOf(this.number);	
	}

}
