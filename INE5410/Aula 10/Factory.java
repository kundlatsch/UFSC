import java.io.*;
import java.util.*;
public class Factory{
	
	public static void main(String[] args){
		Buffer buffer = new Buffer(10);
		ProducerRobot x = new ProducerRobot('x', buffer);
		ProducerRobot o = new ProducerRobot('o', buffer);
		ConsumerRobot consumer = new ConsumerRobot(buffer);
		consumer.start();
		x.start();
		o.start();
	}
}