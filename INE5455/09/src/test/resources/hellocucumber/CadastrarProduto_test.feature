Feature: Cadastrar Produto 
  Cadastrar produtos

Background:
Given O cadastro do usuario "Ernani Cesar" foi realizado
 
@CadastrarProdutoPermitido 
Scenario: Cadastrar Produto Permitido
Given o produto "guitarra"
  And a descricao "guitarra semi nova"
  And e o valor minimo "400.0"
When O produto eh cadastrado 
Then O produto eh cadastrado com sucesso

@CadastrarProdutoNomeRepetido
Scenario: Cadastrar Produto Nome Repetido
Given o produto "guitarra"
  And a descricao "guitarra semi nova"
  And e o valor minimo "400.0"
When O produto eh cadastrado
And O mesmo produto eh cadastrado novamente
Then O sistema retorna erro 