Run application:

Genereate topics:
~/kafka/bin/kafka-topics.sh --create --zookeeper localhost:2181 --replication-factor 1 --partitions 1 --topic valid-messages
~/kafka/bin/kafka-topics.sh --create --zookeeper localhost:2181 --replication-factor 1 --partitions 1 --topic invalid-messages

Recall:
~/kafka/bin/kafka-topics.sh --list --zookeeper localhost:2181

To delete a topic:
~/kafka/bin/kafka-topics.sh --delete -zookeeper localhost:2181 --topic unwantedTopic

In the first terminal (input window):
~/kafka/bin/kafka-console-producer.sh --broker-list localhost:9092 --topic input-topic

In the second terminal (output: valid messages):
~/kafka/bin/kafka-console-consumer.sh --bootstrap-server localhost:9092 --from-beginning --topic valid-messages

In the third terminal (output: invalid messages):
~/kafka/bin/kafka-console-consumer.sh --bootstrap-server localhost:9092 --from-beginning --topic invalid-messages

In the fourth terminal, in the project root (processing engine):
java -jar  ./build/libs/monedero-0.1.0.jar localhost:9092 foo input-topic valid-messages invalid-messages

Tests:

{"event": "CUSTOMER_CONSULTS_ETHPICE", "customer": {"id": "148627768", "name": "Snowden, Edward", "ipAddress": "95.31.18.111"}, "currency": {"name": "ethereum", "price": "RUB"}, "timestamp": "2018-09-28T09:09:09Z"}

{"event": "CUSTOMER_CONSULTS_ETHPICE", "customer": {"id": "13548310", "name": "Assange, Julian", "ipAddress": "185.86.151.11"}, "currency": {"name": "ethereum", "price": "EUR"}, "timestamp": "2018-09-28T08:08:14Z"}

{"event": "CUSTOMER_CONSULTS_ETHPICE", "customer": {"id": "15887564", "name": "Mills, Lindsay", "ipAddress": "186.46.129.15"}, "currency": {"name": "ethereum", "price": "USD"}, "timestamp": "2018-09-28T19:51:35Z"}
