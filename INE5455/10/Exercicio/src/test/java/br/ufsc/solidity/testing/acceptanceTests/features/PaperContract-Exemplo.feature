Feature: SC_Paper_Sale_UFSC Creation

Background:
Given the seller name is "Dicapel"
  And the buyer name is "UFSC"
  And the buyer address is "Reitor..." 

Scenario: Create the SC_Paper_Sale_UFSC contract
Given the contract creation day is "2"  
When the contract is created
Then the contract status is "Created"

Scenario: Activate the SC_Paper_Sale_UFSC contract
Given the contract creation day is "2"  
 And the contract is created
When the contract is activated
Then the contract status is "InEffect"
