Feature: Add Products to a Product Sale Contract
  Text...
  
Background:
Given the seller "Seller 01"
  And the buyer "Buyer 01"
  And the effective date of the contract "2019/2"
  And the contract title "YYZ01"
  And the currency "BRL"
  And the contract is created

@AddAProductTypeToContractAndVerifyTheTotalPrice
Scenario Outline: Add a product type to a contract and verify the total price
Given the product "<productName>" with description "<description>" that has the unit "<unit>" 
  And the product costs "<unitPrice>" per unit 
  And the quantity of the product per unit is "<quantity>" 
  And the expiration date is "<expirationDate>" 
 When the product is added to the contract
 Then the total price of the contract must be "<contractTotalPrice>"

Examples:
|productName |description |unit  |unitPrice |quantity |expirationDate |contractTotalPrice| 
|Apple       |red         |Kg    |2         |10       |2019           |20                |
|Egg         |white       |Dozen |3         |20       |2019           |60                |