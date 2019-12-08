package monedero;

public class ProcessingEngine {

	public static void main(String[] args) {
		String servers = args[0];
		String groupId = args[1];
		String inputTopic = args[2];
		String validTopic = args[3];
		String invalidTopic = args[4];
		String validationType = args[5];
		Reader reader = new Reader(servers, groupId, inputTopic);
		if (validationType.equals("monedero")) {
			Validator1 validator = new Validator1(servers, validTopic, invalidTopic);
			reader.run(validator);
		}
		else if (validationType.equals("rpg")) {
			Validator2 validator = new Validator2(servers, validTopic, invalidTopic);
			reader.run(validator);
		}
	}
}

