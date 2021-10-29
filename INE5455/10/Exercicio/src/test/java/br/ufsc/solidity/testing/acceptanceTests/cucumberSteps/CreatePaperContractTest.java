package br.ufsc.solidity.testing.acceptanceTests.cucumberSteps;

import cucumber.api.java.en.Given;
import cucumber.api.java.en.Then;
import cucumber.api.java.en.When;

import org.assertj.core.api.Assertions;
import org.web3j.crypto.Credentials;
import org.web3j.protocol.Web3j;
import org.web3j.protocol.http.HttpService;
import org.web3j.tx.gas.DefaultGasProvider;

import br.ufsc.solidity.contracts.PaperContract;

import static org.assertj.core.api.Assertions.assertThat;

import java.math.BigInteger;

public class CreatePaperContractTest {

	private static final String ETHEREUM_URL = "http://localhost:7545";
	private static final String WALLET_PRIVATE_KEY = "0ac85ac8f30c2cc801f062a20e76cc65d68bcdabe516fe5edef270b5c13ac0e4";
			
	private PaperContract paperContract;
	
	public String contractSeller;
	public String contractBuyer;
	public String buyerAddress;
	public String effectiveDay;
	
	
@Given("^the seller name is \"([^\"]*)\"$")
public void the_seller_name_is(String arg1) throws Throwable {
	this.contractSeller = arg1;
}

@Given("^the buyer name is \"([^\"]*)\"$")
public void the_buyer_name_is(String arg1) throws Throwable {
	this.contractBuyer = arg1;
}

@Given("^the buyer address is \"([^\"]*)\"$")
public void the_buyer_address_is(String arg1) throws Throwable {
	this.buyerAddress = arg1;
}

@Given("^the contract creation day is \"([^\"]*)\"$")
public void the_contract_creation_day_is(String arg1) throws Throwable {
	 this.effectiveDay = arg1;
}

@When("^the contract is created$")
public void the_contract_is_created() throws Throwable {
	this.paperContract = this.deployContract(this.contractSeller, this.contractBuyer);
	this.paperContract.setBuyerAddress(this.buyerAddress).send();
	this.paperContract.setEffectiveDay(new BigInteger(this.effectiveDay)).send();
}

private PaperContract deployContract(String seller, String buyer) throws Exception {
	Web3j web3j = Web3j.build(new HttpService(ETHEREUM_URL));
	Credentials credentials = Credentials.create(WALLET_PRIVATE_KEY);
	return PaperContract.deploy(web3j, credentials, new DefaultGasProvider(), seller, buyer).send();
}

@When("^the contract is activated$")
public void the_contract_is_activated() throws Throwable {
	this.paperContract.activate().send();
}

@Then("^the contract status is \"([^\"]*)\"$")
public void the_contract_status_is(String arg1) throws Throwable {
	BigInteger statusValue = this.paperContract.getStatus().send();
	switch(arg1) {
	  case "Created":
		assertThat(statusValue).isEqualTo(0);
		break;
	  case "InEffect":
	    assertThat(statusValue).isEqualTo(1);
	    break;
	  case "SuccessfulTermination":
		assertThat(statusValue).isEqualTo(2);
		break;
	  case "UnsuccessfulTermination":
		assertThat(statusValue).isEqualTo(3);
		break;
	  default:
	    Assertions.fail("Unknown status "+arg1);
	}
}


	// Exercício 1

	@Given("^the buyer made a solicitation on the day \"([^\"]*)\"$")
	public void the_buyer_made_a_solicitation_on_the_day(String arg1) throws Throwable {
		this.paperContract.setSolicitationDay(new BigInteger(arg1)).send();
	}

	@Given("^included \"([^\"]*)\" sheets of \"([^\"]*)\" of group \"([^\"]*)\"$")
	public void included_sheets_of_of_group(String arg1, String arg2, String arg3) throws Throwable {
			this.paperContract.addProduct(arg2, arg3, new BigInteger(arg1)).send();
	}

	@When("^a friendly termination occurs$")
	public void a_friendly_termination_occurs() throws Throwable {
			this.paperContract.unsuccessfulTerminate().send();
	}

	// Implementados

	@When("^the seller delivers all required paper$")
	public void the_seller_delivers_all_required_paper() throws Throwable {
		this.paperContract.deliverAll().send();
	}

	@When("the seller delivers the paper on the day {string}")
	public void the_seller_delivers_the_paper_on_the_day(String string) throws Throwable {
		this.paperContract.setDeliveryDay(new BigInteger(string)).send();
	}

	@When("the buyer pays on the day {string}")
	public void the_buyer_pays_on_the_day(String string) throws Throwable {
		this.paperContract.setPaymentDay(new BigInteger(string)).send();
	}

	@Then("the contract is updated and its status is {string}")
	public void the_contract_is_updated_and_its_status_is(String string) throws Throwable {
		this.paperContract.verifyContractTermination().send();
		BigInteger statusValue = this.paperContract.getStatus().send();

		switch(string) {
			case "Created":
			  assertThat(statusValue).isEqualTo(0);
			break;
			
			case "InEffect":
			  assertThat(statusValue).isEqualTo(1);
			break;
			
			case "SuccessfulTermination":
			  assertThat(statusValue).isEqualTo(2);
			break;
			
			case "UnsuccessfulTermination":
			  assertThat(statusValue).isEqualTo(3);
			break;
			
			default:
				Assertions.fail("Unknown status " + string);
		}
	}

	@When("the buyer does not pay in the due date")
	public void the_buyer_does_not_pay_in_the_due_date() throws Throwable {
		this.paperContract.setPaymentDay(new BigInteger("51")).send();
	}

	// Exercício 2

	@When("the seller notifies the buyer on the day {string} a delay of {string} days")
	public void the_seller_notifies_the_buyer_on_the_day_a_delay_of_days(String string, String string2) throws Throwable {
		this.paperContract.notifiesDelay(new BigInteger(string), new BigInteger(string2)).send();
	}

	@When("the buyer pays on the day {string} with a fine")
	public void the_buyer_pays_on_the_day_with_a_fine(String string) throws Throwable {
		this.paperContract.payedFine().send();
		this.paperContract.setPaymentDay(new BigInteger(string)).send();
	}

}
