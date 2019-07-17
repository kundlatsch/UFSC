import java.util.ArrayList;
public class Buffer {
	private int size;
	private ArrayList<Character> buffer;
	private int size_counter;
	private int o_counter;
	private int x_counter;


	public Buffer(int size) {
		this.size = size;
		this.buffer = new ArrayList<Character>();
		this.size_counter = 0;
		this.o_counter = 0;
		this.x_counter = 0;
	}

	public synchronized boolean insert(char part){
		// Espera ocupada :(
		if(size_counter < size) {
			if((part == 'o' && o_counter < 9) || (part == 'x' && x_counter < 8)){
				buffer.add(part);
				size_counter++;

				if(part == 'o')
					o_counter++;
				else
					x_counter++;

				printBuffer();
				return true;
			}
		}
		return false;
	}

	public synchronized char remove(char part){
		if(buffer.indexOf('o') < buffer.lastIndexOf('o') && buffer.contains('x')) {
			if(part == 'o')
				o_counter--;
			else if(part == 'x')
				x_counter--;

			size_counter--;
			printBuffer();
			return buffer.remove(buffer.indexOf(part));
		}
		return 'e';
	}

	public void printBuffer() {
		System.out.println(buffer.toString());
	}
}