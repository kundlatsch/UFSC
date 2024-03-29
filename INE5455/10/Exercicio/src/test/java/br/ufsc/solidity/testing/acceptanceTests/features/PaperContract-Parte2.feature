Feature: SC_Paper_Sale_UFSC Termination
  
Background:
Given the seller name is "Dicapel"
  And the buyer name is "UFSC"
  And the buyer address is "Reitor..." 
  And the contract creation day is "2"
  And the contract is created
  And the contract is activated


Scenario: Successful termination #1 of SC_Paper_Sale_UFSC contract
Given the buyer made a solicitation on the day "10"
  And included "200" sheets of "paper" of group "6"
  And included "500" sheets of "paper" of group "2"
When the seller delivers the paper on the day "15"
 And the seller delivers all required paper
 And the buyer pays on the day "25"
Then the contract is updated and its status is "SuccessfulTermination"

   
Scenario: Successful termination #2 of SC_Paper_Sale_UFSC contract
Given the buyer made a solicitation on the day "10"
  And included "200" sheets of "paper" of group "6"
  And included "500" sheets of "paper" of group "2"
When the seller notifies the buyer on the day "15" a delay of "10" days
 And the seller delivers the paper on the day "25"
 And the seller delivers all required paper
 And the buyer pays on the day "50"
Then the contract is updated and its status is "SuccessfulTermination"

 
Scenario: Successful termination #3 of SC_Paper_Sale_UFSC contract
Given the buyer made a solicitation on the day "10"
  And included "200" sheets of "paper" of group "6"
  And included "500" sheets of "paper" of group "2"
When the seller delivers the paper on the day "15"  
 And the seller delivers all required paper
 And the buyer does not pay in the due date
 And the buyer pays on the day "60" with a fine
Then the contract is updated and its status is "SuccessfulTermination"


Scenario: Unsuccessful termination #1 of SC_Paper_Sale_UFSC contract
When a friendly termination occurs
Then the contract status is "UnsuccessfulTermination"


Scenario: Unsuccessful termination #2 of SC_Paper_Sale_UFSC contract
Given the buyer made a solicitation on the day "10"
  And included "200" sheets of "paper" of group "6"
  And included "500" sheets of "paper" of group "2"
When the seller delivers the paper on the day "15"
 And the seller delivers all required paper
 And the buyer does not pay in the due date
Then the contract is updated and its status is "UnsuccessfulTermination"