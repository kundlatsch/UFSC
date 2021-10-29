package hellocucumber; 
 
import static org.junit.jupiter.api.Assertions.*;
 
import io.cucumber.java.en.Given;
import io.cucumber.java.en.When;
import io.cucumber.java.en.Then;

import interfaces.IMercadoLeilao; 
import interfaces.IUsuario; 
import modelo.MercadoLeilao; 
import modelo.Usuario;
 
public class StepsProduto { 
  
    IUsuario usuario;
    public String nome_produto = "";
    public String descricao = "";
    public Double valor_minimo = 0.0;
    public Boolean cadastro = false;
    public String error = "";
    
    
    IMercadoLeilao mercado = new MercadoLeilao();

    @Given("O cadastro do usuario {string} foi realizado")
    public void o_cadastro_do_usuario_foi_realizado(String string) {
        String nome_usuario = string;
        String endereco = "Campus Universitario";
        String email = "ernani.santos@posgrad.ufsc.br";
        String cpf = "055.761.919-00";
        Boolean usuario_cadastrado = false;
        try { 
            mercado.cadastrarUsuario(nome_usuario, endereco, email, cpf); 
            usuario_cadastrado = true; 
           } catch (Exception e) { 
            e.printStackTrace(); 
        }
        assertEquals(true, usuario_cadastrado);
    }

    @Given("o produto {string}")
    public void o_produto(String string) {
        this.nome_produto = string;
    }

    @Given("a descricao {string}")
    public void a_descricao(String string) {
        this.descricao = string;
    }

    @Given("e o valor minimo {string}")
    public void e_o_valor_minimo(String string) {
        this.valor_minimo = Double.parseDouble(string); 
    }

    @When("O produto eh cadastrado")
    @When("O mesmo produto eh cadastrado novamente")
    public void o_produto_eh_cadastrado() {
        try { 
            mercado.cadastrarProduto(this.nome_produto, this.descricao, this.valor_minimo, "055.761.919-00", null);
            this.cadastro = true;
           } catch (Exception e) { 
            this.error = e.getMessage(); 
        }
    }

    @Then("O produto eh cadastrado com sucesso")
    public void o_produto_eh_cadastrado_com_sucesso() {
        assertEquals(true, cadastro);
    }

    @Then("O sistema retorna erro")
    public void o_sistema_retorna_erro() {
        String mensagem_erro = "O produto ja existe ou o leiloador nao esta cadastrado.";
        assertEquals(mensagem_erro, this.error);
    }
}