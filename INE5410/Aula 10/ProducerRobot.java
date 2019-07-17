public class ProducerRobot extends Thread {
	private char part;
	private Buffer buffer;

	public ProducerRobot(char part, Buffer buffer) {
		this.part = part;
		this.buffer = buffer;
	}

	public void run() {
        	while(true) {
        		char item = this.part;
        		if(this.buffer.insert(item)) {
                    produce();
                }
        	}
    }

    private void produce() {
    	System.out.println("Produzindo " + this.part);
    	try { Thread.sleep(100); } catch (InterruptedException ex) {
    		System.out.println ("Production time error");
		}
    }

}