package br.ufsc.solidity.testing.acceptanceTests.cucumberSteps;

import cucumber.api.java.en.Given;
import cucumber.api.java.en.Then;
import cucumber.api.java.en.When;

import org.assertj.core.api.Assertions;
import org.web3j.crypto.Credentials;
import org.web3j.protocol.Web3j;
import org.web3j.protocol.http.HttpService;
import org.web3j.tx.gas.DefaultGasProvider;

import br.ufsc.solidity.contracts.ProductSaleContract;

import static org.assertj.core.api.Assertions.assertThat;

import java.math.BigInteger;

public class CreateProductSaleContractTest {

	private static final String ETHEREUM_URL = "http://localhost:7545";
	private static final String WALLET_PRIVATE_KEY = "0ac85ac8f30c2cc801f062a20e76cc65d68bcdabe516fe5edef270b5c13ac0e4";
			
	private ProductSaleContract productContract;
	
	public String contractSeller;
	public String contractBuyer;
	public String contractEffectiveDate;
	
	public String contractTitle = "";
	public String currencyName = "";
	
	@Given("^the seller \"([^\"]*)\"$")
	public void the_seller(String arg1) throws Throwable {
		this.contractSeller = arg1;
	}

	@Given("^the buyer \"([^\"]*)\"$")
	public void the_buyer(String arg1) throws Throwable {
		this.contractBuyer = arg1;
	}

	@Given("^the effective date of the contract \"([^\"]*)\"$")
	public void the_effective_date_of_the_contract(String arg1) throws Throwable {
		this.contractEffectiveDate= arg1;
	}
	
	@Given("^the contract title \"([^\"]*)\"$")
	public void the_contract_title(String arg1) throws Throwable {
		this.contractTitle = arg1;
	}

	@Given("^the currency \"([^\"]*)\"$")
	public void the_currency(String arg1) throws Throwable {
		this.currencyName = arg1;
	}
	
	@When("^the contract is created$")
	public void the_contract_is_created() throws Throwable {
		this.productContract = this.deployContract(this.contractSeller, this.contractBuyer,
				            this.contractTitle, this.currencyName);
	}
	
	@Then("^the contract seller must be \"([^\"]*)\"$")
	public void the_contract_seller_must_be(String arg1) throws Throwable {
	    String sellerValue = this.productContract.getSeller().send();
	    assertThat(sellerValue).isEqualTo(arg1);
	}

	@Then("^the contract buyer must be \"([^\"]*)\"$")
	public void the_contract_buyer_must_be(String arg1) throws Throwable {
	    String buyerValue = this.productContract.getBuyer().send();
	    assertThat(buyerValue).isEqualTo(arg1);
	}

	@Then("^the contract title must be \"([^\"]*)\"$")
	public void the_contract_title_must_be(String arg1) throws Throwable {
	    String titleValue = this.productContract.getTitle().send();
	    assertThat(titleValue).isEqualTo(arg1);
	}
	
	@Then("^the currency must be \"([^\"]*)\"$")
	public void the_currency_must_be(String arg1) throws Throwable {
		BigInteger currencyValue = this.productContract.getCurrency().send();
		switch(arg1) {
		  case "USD":
			assertThat(currencyValue).isEqualTo(0);
			break;
		  case "CAN":
		    assertThat(currencyValue).isEqualTo(1);
		    break;
		  case "BRL":
			assertThat(currencyValue).isEqualTo(2);
			break;
		  default:
		    Assertions.fail("Unknown currency "+arg1);
		}
	}

	@Then("^the total price of the contract must be \"([^\"]*)\"$")
	public void the_total_price_of_the_contract_must_be(String arg1) throws Throwable {
		BigInteger totalPriceValue = this.productContract.getContractTotalPrice().send();
		assertThat(totalPriceValue).isNotNull().isEqualByComparingTo(new BigInteger(arg1));
	}
	
	private ProductSaleContract deployContract(String seller, String buyer, String title, String currency) throws Exception {
		Web3j web3j = Web3j.build(new HttpService(ETHEREUM_URL));
		Credentials credentials = Credentials.create(WALLET_PRIVATE_KEY);
		return ProductSaleContract.deploy(web3j, credentials, new DefaultGasProvider(), seller, buyer, title, currency).send();
	}
	
	//OTHER METHODS FROM THE SCENARIO ADD PRODUCTS TO A MEAT CONTRACT
	
	public String productName = "";
	public String productDescription = "";
	public String productUnit = "";
	public String productUnitPrice = "";
	public String productQuantity = "";
	public String productExpirationDate = "";
	public String productContractTotalPrice = "";
	
	@Given("^the product \"([^\"]*)\" with description \"([^\"]*)\" that has the unit \"([^\"]*)\"$")
	public void the_product_with_description_that_has_the_unit(String arg1, String arg2, String arg3) throws Throwable {
	    this.productName = arg1;
	    this.productDescription = arg2;
	    this.productUnit = arg3;
	}

	@Given("^the product costs \"([^\"]*)\" per unit$")
	public void the_product_costs_per_unit(String arg1) throws Throwable {
		this.productUnitPrice = arg1;
	}

	@Given("^the quantity of the product per unit is \"([^\"]*)\"$")
	public void the_quantity_of_the_product_per_unit_is(String arg1) throws Throwable {
		this.productQuantity = arg1;
	}

	@Given("^the expiration date is \"([^\"]*)\"$")
	public void the_expiration_date_is(String arg1) throws Throwable {
		this.productExpirationDate = arg1;
	}
	
    @When("^the product is added to the contract$")
	public void the_product_is_added_to_the_contract() throws Throwable {
		this.productContract.addProduct(productName, productDescription, productUnit, 
				new BigInteger(productUnitPrice), new BigInteger(productQuantity), new BigInteger(productExpirationDate)).send();
    }
	  
    @Then("^the total price of the new contract must be \"([^\"]*)\"$")
    public void the_total_price_of_the_new_contract_must_be(String arg1) throws Throwable {
 		BigInteger totalPriceValue = this.productContract.getContractTotalPrice().send();
		assertThat(totalPriceValue).isNotNull().isEqualByComparingTo(new BigInteger(arg1));
    }
  
}
