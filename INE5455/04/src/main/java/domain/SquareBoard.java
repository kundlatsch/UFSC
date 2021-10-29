package domain;

public class SquareBoard extends Board {
	
	private int dimension;
	
	public SquareBoard (int dimension){
		super(dimension, dimension);
		this.dimension = dimension;
	}
	
	public int getDimension() {
		return this.dimension;
	}

}
