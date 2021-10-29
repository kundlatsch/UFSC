pragma solidity >=0.4.22 <0.6.0;

contract PaperContract {
     
   enum Status { Created, InEffect, SuccessfulTermination, UnsuccessfulTermination }
    
   struct Product {
        string name; 		
        string group; 
        int quantity;        
    }
    
    string seller; //contratado
    string buyer;  //contratante
    string buyerAddress;
    int contractTotalPrice;   
    Product[] products;

    Status status;

    uint effectiveDay;
	uint solicitationDay;
	uint dueDay;
	uint deliveryDay;
	uint paymentDay;
	
	bool fine = false;
    bool delivered = false;
    bool friendlyRecision = false;

    constructor( string memory _seller, string memory _buyer) public {
        seller = _seller;
        buyer = _buyer;
    	contractTotalPrice = 0;
    	status = Status.Created;
    	solicitationDay = 0;
	 	deliveryDay = 0;
		paymentDay = 0;
   }
   
   function activate () public {  	
    	status = Status.InEffect;
   }
    
   function setBuyerAddress (string memory _buyerAddress) public {  	
    	buyerAddress = _buyerAddress;
   }

   function setPaymentDay (uint _paymentDay) public {  	
    	paymentDay = _paymentDay;
   }

   function setDeliveryDay (uint _deliveryDay) public {  	
    	deliveryDay = _deliveryDay;
   }
    
   function setEffectiveDay (uint _effectiveDay) public {  	
    	effectiveDay = _effectiveDay;
   }
    
   function addProduct( string memory name, string memory group, int quantity) public {
   		Product memory product;
    	product.name = name;
    	product.group = group;          
		product.quantity = quantity;
	    products.push(product);
   } 
    
   function setSolicitationDay (uint _solicitationDay) public {  	
    	solicitationDay = _solicitationDay;
    	dueDay = solicitationDay + 10;
   }

   function deliverAll () public {
       delivered = true;
   }

   function payedFine () public {
       fine = true;
   }

   function verifyContractTermination () public {
        if (delivered == true) {
            if (paymentDay <= (deliveryDay + 30)) {
                if (deliveryDay <= dueDay) {
                    status = Status.SuccessfulTermination;
                }
            } else if (paymentDay > deliveryDay + 30 && fine == true) {
                status = Status.SuccessfulTermination;
            } else if (paymentDay > deliveryDay + 30 && fine == false) {
                status = Status.UnsuccessfulTermination;
            }
        } else if (friendlyRecision == true) {
            status = Status.UnsuccessfulTermination;
        }
    }

    function unsuccessfulTerminate () public {  	
    	status = Status.UnsuccessfulTermination;
    }
    
    function notifiesDelay (uint notificationDay, uint delay) public {
    // The seller notifies the buyer on notificationDay a delay.
    	if (notificationDay <= dueDay - 2){
    		dueDay = dueDay + delay;
    	}
    } 
    
    
    //GETTERS
    
    //view significa que nao tem transacao, nao precisa minerar (nao usa gas para executar)
    function getStatus() public view returns (Status) {
        return status;
    }
    
    function getBuyer() public view returns (string memory) {
        return buyer;
    }
    
    function getBuyerAddress() public view returns (string memory) {
        return buyerAddress;
    }
    
    function getEffectiveDay() public view returns (uint) {
        return effectiveDay;
    }
    
    function getContractTotalPrice() public view returns (int) {
        return contractTotalPrice;
    }     

  	
}