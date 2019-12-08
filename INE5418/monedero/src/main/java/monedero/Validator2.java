package monedero;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.apache.kafka.clients.producer.KafkaProducer;
import java.io.IOException;

public class Validator2 implements Producer {
	private final KafkaProducer<String, String> producer;
	private final String validMessages;
	private final String invalidMessages;

	private static final ObjectMapper MAPPER = new ObjectMapper();
	public Validator2(String servers, String validMessages, String invalidMessages) { //1
		this.producer = new KafkaProducer<>(Producer.createConfig(servers));
		this.validMessages = validMessages;
		this.invalidMessages = invalidMessages;
	}

	@Override
	public void process(String message) {
		try {
			JsonNode root = MAPPER.readTree(message);
			String error = "";
			error = error.concat(validate(root, "name")); //2
			error = error.concat(validate(root, "class"));
			error = error.concat(validate(root, "race"));
			error = error.concat(validate(root, "level"));
			if (error.length() > 0) {
				Producer.write(this.producer, this.invalidMessages, "{\"error\": \" " + error + "\"}");
			} else {
				Producer.write(this.producer, this.validMessages, MAPPER.writeValueAsString(root));
			}
		} catch (IOException e) {
			Producer.write(this.producer, this.invalidMessages, "{\"error\": \"" + e.getClass().getSimpleName() + ": " + e.getMessage() + "\"}");
		}
	}

	private String validate(JsonNode root, String path) {
		if (!root.has(path)) {
			return path.concat(" is missing. ");
		}
		JsonNode node = root.path(path);
		if (node.isMissingNode()) {
			return path.concat(" is missing. ");
		}
		return "";
	}
}

