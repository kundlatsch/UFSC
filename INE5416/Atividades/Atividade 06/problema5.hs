-- Uma declaração newtype cria um novo tipo da mesma maneira que os data. A sintaxe e o uso de newtypes são virtualmente idênticos àqueles das declarações de data - na verdade, você pode substituir a palavra-chave newtype por data e ela ainda compilará, na verdade, é provável que o programa ainda funcione. O contrário não é verdadeiro, no entanto - os data só podem ser substituídos por newtype se o tipo tiver exatamente um construtor com exatamente um campo dentro dele.

newtype A = A B
