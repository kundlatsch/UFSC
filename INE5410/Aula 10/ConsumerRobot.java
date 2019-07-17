public class ConsumerRobot extends Thread {
	private Buffer buffer;
    private int partCounter;

	public ConsumerRobot(Buffer buffer) {
		this.buffer = buffer;
        this.partCounter = 0;
	}

	public void run() {
       
        while(true) {
            char item;
            if(this.partCounter == 0 || this.partCounter == 1) {
        	    item = this.buffer.remove('o');
                this.partCounter++;
            }
            else {
                item = this.buffer.remove('x');
                this.partCounter = 0;
            }
            if(item != 'e')
                this.consume(item);
            
        }
    }

    private void consume(char item) {
        System.out.println("Consumindo " + item);
    	try { Thread.sleep(100); } catch (InterruptedException ex) {
            System.out.println ("Consume time error");
        }
    }

}