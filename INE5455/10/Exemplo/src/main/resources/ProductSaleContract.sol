pragma solidity >=0.4.22 <0.6.0;

contract ProductSaleContract {
    
    address owner = msg.sender; //dono do contrato é o criador

    enum MyCurrency { USD, CAN, BRL }
    enum MyUnit { Kg, Litter, Dozen }

    struct Product {
        string name;
        string description;
        address creator;
        MyUnit unit;
        int unitPrice;
        int quantityPerUnit;
        int totalPrice; 
        uint expirationDate;
    }
    
    string seller;
    string buyer;
    string title;
    MyCurrency currency;    
    int contractTotalPrice;   
    Product[] products;

    constructor( string memory _seller, string memory _buyer, string memory _title, 
                 string memory _currency ) public {
        seller = _seller;
        buyer = _buyer;
    	title = _title;
    	
    	if (keccak256(abi.encodePacked(_currency)) == keccak256(abi.encodePacked("BRL"))) {
              currency = MyCurrency.BRL;
    	} else { 
    		if (keccak256(abi.encodePacked(_currency)) == keccak256(abi.encodePacked("CAN"))) {
              currency = MyCurrency.CAN;
    		} else {   	
              currency = MyCurrency.USD;
            }
        }
    }
   
    //SETTERS
    
	function addProduct( string memory name, string memory description, string memory unit,
						int unitPrice, int quantityPerUnit, uint expirationDate) public {
    	
    	Product memory product;
    	
    	product.name = name;
        product.description = description;  
        product.creator = msg.sender;
        
        if (keccak256(abi.encodePacked(unit)) == keccak256(abi.encodePacked("Kg"))) {
              product.unit = MyUnit.Kg;
    	} else if (keccak256(abi.encodePacked(unit)) == keccak256(abi.encodePacked("Litter"))) {
              		product.unit = MyUnit.Litter;
    	       } else if (keccak256(abi.encodePacked(unit)) == keccak256(abi.encodePacked("Dozen"))) {
              				product.unit = MyUnit.Dozen;
				    	} else  {
				    		product.unit = MyUnit.Kg;
						        }
 
        product.unitPrice = unitPrice;
        product.quantityPerUnit = quantityPerUnit;
        product.totalPrice = product.unitPrice * product.quantityPerUnit; 
        product.expirationDate = expirationDate;

        products.push(product);
        contractTotalPrice = contractTotalPrice + product.totalPrice;
	}        
    
    //GETTERS
    
    //view significa que nao tem transacao, nao precisa minerar (nao usa gas para executar)
    function getSeller() public view returns (string memory) {
        return seller;
    }
    
    function getBuyer() public view returns (string memory) {
        return buyer;
    }
    
    function getTitle() public view returns (string memory) {
        return title;
    }
    
    function getCurrency() public view returns (MyCurrency) {
        return currency;
    }
    
    function getContractTotalPrice() public view returns (int) {
        return contractTotalPrice;
    } 

}