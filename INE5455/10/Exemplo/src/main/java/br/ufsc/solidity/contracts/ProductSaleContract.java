package br.ufsc.solidity.contracts;

import java.math.BigInteger;
import java.util.Arrays;
import java.util.Collections;
import org.web3j.abi.FunctionEncoder;
import org.web3j.abi.TypeReference;
import org.web3j.abi.datatypes.Function;
import org.web3j.abi.datatypes.Type;
import org.web3j.abi.datatypes.Utf8String;
import org.web3j.abi.datatypes.generated.Int256;
import org.web3j.abi.datatypes.generated.Uint8;
import org.web3j.crypto.Credentials;
import org.web3j.protocol.Web3j;
import org.web3j.protocol.core.RemoteCall;
import org.web3j.protocol.core.RemoteFunctionCall;
import org.web3j.protocol.core.methods.response.TransactionReceipt;
import org.web3j.tx.Contract;
import org.web3j.tx.TransactionManager;
import org.web3j.tx.gas.ContractGasProvider;

/**
 * <p>Auto generated code.
 * <p><strong>Do not modify!</strong>
 * <p>Please use the <a href="https://docs.web3j.io/command_line.html">web3j command line tools</a>,
 * or the org.web3j.codegen.SolidityFunctionWrapperGenerator in the 
 * <a href="https://github.com/web3j/web3j/tree/master/codegen">codegen module</a> to update.
 *
 * <p>Generated with web3j version 4.6.4.
 */
@SuppressWarnings("rawtypes")
public class ProductSaleContract extends Contract {
    public static final String BINARY = "6080604052600080546001600160a01b031916331790553480156200002357600080fd5b5060405162000e6938038062000e69833981810160405260808110156200004957600080fd5b81019080805160405193929190846401000000008211156200006a57600080fd5b9083019060208201858111156200008057600080fd5b82516401000000008111828201881017156200009b57600080fd5b82525081516020918201929091019080838360005b83811015620000ca578181015183820152602001620000b0565b50505050905090810190601f168015620000f85780820380516001836020036101000a031916815260200191505b50604052602001805160405193929190846401000000008211156200011c57600080fd5b9083019060208201858111156200013257600080fd5b82516401000000008111828201881017156200014d57600080fd5b82525081516020918201929091019080838360005b838110156200017c57818101518382015260200162000162565b50505050905090810190601f168015620001aa5780820380516001836020036101000a031916815260200191505b5060405260200180516040519392919084640100000000821115620001ce57600080fd5b908301906020820185811115620001e457600080fd5b8251640100000000811182820188101715620001ff57600080fd5b82525081516020918201929091019080838360005b838110156200022e57818101518382015260200162000214565b50505050905090810190601f1680156200025c5780820380516001836020036101000a031916815260200191505b50604052602001805160405193929190846401000000008211156200028057600080fd5b9083019060208201858111156200029657600080fd5b8251640100000000811182820188101715620002b157600080fd5b82525081516020918201929091019080838360005b83811015620002e0578181015183820152602001620002c6565b50505050905090810190601f1680156200030e5780820380516001836020036101000a031916815260200191505b50604052505084516200032a91506001906020870190620004e3565b50825162000340906002906020860190620004e3565b50815162000356906003906020850190620004e3565b5060405160200180806210949360ea1b815250600301905060405160208183030381529060405280519060200120816040516020018082805190602001908083835b60208310620003b95780518252601f19909201916020918201910162000398565b6001836020036101000a0380198251168184511680821785525050505050509050019150506040516020818303038152906040528051906020012014156200041657600480546002919060ff19166001835b0217905550620004d9565b60405160200180806221a0a760e91b815250600301905060405160208183030381529060405280519060200120816040516020018082805190602001908083835b60208310620004785780518252601f19909201916020918201910162000457565b6001836020036101000a038019825116818451168082178552505050505050905001915050604051602081830303815290604052805190602001201415620004ce57600480546001919060ff191682806200040b565b6004805460ff191690555b5050505062000588565b828054600181600116156101000203166002900490600052602060002090601f016020900481019282601f106200052657805160ff191683800117855562000556565b8280016001018555821562000556579182015b828111156200055657825182559160200191906001019062000539565b506200056492915062000568565b5090565b6200058591905b808211156200056457600081556001016200056f565b90565b6108d180620005986000396000f3fe608060405234801561001057600080fd5b50600436106100625760003560e01c8063603daf9a146100675780636945c1fd146100e45780639481da5314610110578063a0597ca91461012a578063dbd0e1b6146102eb578063ff3c1a8f146102f3575b600080fd5b61006f6102fb565b6040805160208082528351818301528351919283929083019185019080838360005b838110156100a9578181015183820152602001610091565b50505050905090810190601f1680156100d65780820380516001836020036101000a031916815260200191505b509250505060405180910390f35b6100ec61038f565b604051808260028111156100fc57fe5b60ff16815260200191505060405180910390f35b610118610398565b60408051918252519081900360200190f35b6102e9600480360360c081101561014057600080fd5b81019060208101813564010000000081111561015b57600080fd5b82018360208201111561016d57600080fd5b8035906020019184600183028401116401000000008311171561018f57600080fd5b91908080601f01602080910402602001604051908101604052809392919081815260200183838082843760009201919091525092959493602081019350359150506401000000008111156101e257600080fd5b8201836020820111156101f457600080fd5b8035906020019184600183028401116401000000008311171561021657600080fd5b91908080601f016020809104026020016040519081016040528093929190818152602001838380828437600092019190915250929594936020810193503591505064010000000081111561026957600080fd5b82018360208201111561027b57600080fd5b8035906020019184600183028401116401000000008311171561029d57600080fd5b91908080601f016020809104026020016040519081016040528093929190818152602001838380828437600092019190915250929550508235935050506020810135906040013561039e565b005b61006f6106ea565b61006f61074a565b60028054604080516020601f60001961010060018716150201909416859004938401819004810282018101909252828152606093909290918301828280156103845780601f1061035957610100808354040283529160200191610384565b820191906000526020600020905b81548152906001019060200180831161036757829003601f168201915b505050505090505b90565b60045460ff1690565b60055490565b6103a66107ab565b8681526020808201879052336040808401919091528051614b6760f01b8184015281518082036002018152602282019092528151918301919091208751909288926042019182918401908083835b602083106104135780518252601f1990920191602091820191016103f4565b6001836020036101000a038019825116818451168082178552505050505050905001915050604051602081830303815290604052805190602001201415610470576060810160005b9081600281111561046857fe5b9052506105d5565b6040516020018080652634ba3a32b960d11b815250600601905060405160208183030381529060405280519060200120856040516020018082805190602001908083835b602083106104d35780518252601f1990920191602091820191016104b4565b6001836020036101000a03801982511681845116808217855250505050505090500191505060405160208183030381529060405280519060200120141561051f5760608101600161045b565b6040516020018080642237bd32b760d91b815250600501905060405160208183030381529060405280519060200120856040516020018082805190602001908083835b602083106105815780518252601f199092019160209182019101610562565b6001836020036101000a0380198251168184511680821785525050505050509050019150506040516020818303038152906040528051906020012014156105cd5760608101600261045b565b600060608201525b6080810184905260a0810183905282840260c082015260e0810182905260068054600181018083556000929092528251805184926007027ff652222313e28459528d920b65115c16c04f3efc82aaedc97be59f3f377c0d3f019161063e91839160200190610804565b5060208281015180516106579260018501920190610804565b506040820151600282810180546001600160a01b0319166001600160a01b03909316929092178083556060850151929160ff60a01b1990911690600160a01b9084908111156106a257fe5b02179055506080820151816003015560a0820151816004015560c0820151816005015560e082015181600601555050508060c001516005540160058190555050505050505050565b60018054604080516020601f600260001961010087891615020190951694909404938401819004810282018101909252828152606093909290918301828280156103845780601f1061035957610100808354040283529160200191610384565b60038054604080516020601f60026000196101006001881615020190951694909404938401819004810282018101909252828152606093909290918301828280156103845780601f1061035957610100808354040283529160200191610384565b604051806101000160405280606081526020016060815260200160006001600160a01b03168152602001600060028111156107e257fe5b8152602001600081526020016000815260200160008152602001600081525090565b828054600181600116156101000203166002900490600052602060002090601f016020900481019282601f1061084557805160ff1916838001178555610872565b82800160010185558215610872579182015b82811115610872578251825591602001919060010190610857565b5061087e929150610882565b5090565b61038c91905b8082111561087e576000815560010161088856fea265627a7a72315820aeae1f47197cd71d47f1cb9b2b20c23d5d59154d1c880a05d416799819ba67fa64736f6c63430005100032";

    public static final String FUNC_ADDPRODUCT = "addProduct";

    public static final String FUNC_GETBUYER = "getBuyer";

    public static final String FUNC_GETCONTRACTTOTALPRICE = "getContractTotalPrice";

    public static final String FUNC_GETCURRENCY = "getCurrency";

    public static final String FUNC_GETSELLER = "getSeller";

    public static final String FUNC_GETTITLE = "getTitle";

    @Deprecated
    protected ProductSaleContract(String contractAddress, Web3j web3j, Credentials credentials, BigInteger gasPrice, BigInteger gasLimit) {
        super(BINARY, contractAddress, web3j, credentials, gasPrice, gasLimit);
    }

    protected ProductSaleContract(String contractAddress, Web3j web3j, Credentials credentials, ContractGasProvider contractGasProvider) {
        super(BINARY, contractAddress, web3j, credentials, contractGasProvider);
    }

    @Deprecated
    protected ProductSaleContract(String contractAddress, Web3j web3j, TransactionManager transactionManager, BigInteger gasPrice, BigInteger gasLimit) {
        super(BINARY, contractAddress, web3j, transactionManager, gasPrice, gasLimit);
    }

    protected ProductSaleContract(String contractAddress, Web3j web3j, TransactionManager transactionManager, ContractGasProvider contractGasProvider) {
        super(BINARY, contractAddress, web3j, transactionManager, contractGasProvider);
    }

    public RemoteFunctionCall<TransactionReceipt> addProduct(String name, String description, String unit, BigInteger unitPrice, BigInteger quantityPerUnit, BigInteger expirationDate) {
        final Function function = new Function(
                FUNC_ADDPRODUCT, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Utf8String(name), 
                new org.web3j.abi.datatypes.Utf8String(description), 
                new org.web3j.abi.datatypes.Utf8String(unit), 
                new org.web3j.abi.datatypes.generated.Int256(unitPrice), 
                new org.web3j.abi.datatypes.generated.Int256(quantityPerUnit), 
                new org.web3j.abi.datatypes.generated.Uint256(expirationDate)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<String> getBuyer() {
        final Function function = new Function(FUNC_GETBUYER, 
                Arrays.<Type>asList(), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Utf8String>() {}));
        return executeRemoteCallSingleValueReturn(function, String.class);
    }

    public RemoteFunctionCall<BigInteger> getContractTotalPrice() {
        final Function function = new Function(FUNC_GETCONTRACTTOTALPRICE, 
                Arrays.<Type>asList(), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Int256>() {}));
        return executeRemoteCallSingleValueReturn(function, BigInteger.class);
    }

    public RemoteFunctionCall<BigInteger> getCurrency() {
        final Function function = new Function(FUNC_GETCURRENCY, 
                Arrays.<Type>asList(), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Uint8>() {}));
        return executeRemoteCallSingleValueReturn(function, BigInteger.class);
    }

    public RemoteFunctionCall<String> getSeller() {
        final Function function = new Function(FUNC_GETSELLER, 
                Arrays.<Type>asList(), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Utf8String>() {}));
        return executeRemoteCallSingleValueReturn(function, String.class);
    }

    public RemoteFunctionCall<String> getTitle() {
        final Function function = new Function(FUNC_GETTITLE, 
                Arrays.<Type>asList(), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Utf8String>() {}));
        return executeRemoteCallSingleValueReturn(function, String.class);
    }

    @Deprecated
    public static ProductSaleContract load(String contractAddress, Web3j web3j, Credentials credentials, BigInteger gasPrice, BigInteger gasLimit) {
        return new ProductSaleContract(contractAddress, web3j, credentials, gasPrice, gasLimit);
    }

    @Deprecated
    public static ProductSaleContract load(String contractAddress, Web3j web3j, TransactionManager transactionManager, BigInteger gasPrice, BigInteger gasLimit) {
        return new ProductSaleContract(contractAddress, web3j, transactionManager, gasPrice, gasLimit);
    }

    public static ProductSaleContract load(String contractAddress, Web3j web3j, Credentials credentials, ContractGasProvider contractGasProvider) {
        return new ProductSaleContract(contractAddress, web3j, credentials, contractGasProvider);
    }

    public static ProductSaleContract load(String contractAddress, Web3j web3j, TransactionManager transactionManager, ContractGasProvider contractGasProvider) {
        return new ProductSaleContract(contractAddress, web3j, transactionManager, contractGasProvider);
    }

    public static RemoteCall<ProductSaleContract> deploy(Web3j web3j, Credentials credentials, ContractGasProvider contractGasProvider, String _seller, String _buyer, String _title, String _currency) {
        String encodedConstructor = FunctionEncoder.encodeConstructor(Arrays.<Type>asList(new org.web3j.abi.datatypes.Utf8String(_seller), 
                new org.web3j.abi.datatypes.Utf8String(_buyer), 
                new org.web3j.abi.datatypes.Utf8String(_title), 
                new org.web3j.abi.datatypes.Utf8String(_currency)));
        return deployRemoteCall(ProductSaleContract.class, web3j, credentials, contractGasProvider, BINARY, encodedConstructor);
    }

    public static RemoteCall<ProductSaleContract> deploy(Web3j web3j, TransactionManager transactionManager, ContractGasProvider contractGasProvider, String _seller, String _buyer, String _title, String _currency) {
        String encodedConstructor = FunctionEncoder.encodeConstructor(Arrays.<Type>asList(new org.web3j.abi.datatypes.Utf8String(_seller), 
                new org.web3j.abi.datatypes.Utf8String(_buyer), 
                new org.web3j.abi.datatypes.Utf8String(_title), 
                new org.web3j.abi.datatypes.Utf8String(_currency)));
        return deployRemoteCall(ProductSaleContract.class, web3j, transactionManager, contractGasProvider, BINARY, encodedConstructor);
    }

    @Deprecated
    public static RemoteCall<ProductSaleContract> deploy(Web3j web3j, Credentials credentials, BigInteger gasPrice, BigInteger gasLimit, String _seller, String _buyer, String _title, String _currency) {
        String encodedConstructor = FunctionEncoder.encodeConstructor(Arrays.<Type>asList(new org.web3j.abi.datatypes.Utf8String(_seller), 
                new org.web3j.abi.datatypes.Utf8String(_buyer), 
                new org.web3j.abi.datatypes.Utf8String(_title), 
                new org.web3j.abi.datatypes.Utf8String(_currency)));
        return deployRemoteCall(ProductSaleContract.class, web3j, credentials, gasPrice, gasLimit, BINARY, encodedConstructor);
    }

    @Deprecated
    public static RemoteCall<ProductSaleContract> deploy(Web3j web3j, TransactionManager transactionManager, BigInteger gasPrice, BigInteger gasLimit, String _seller, String _buyer, String _title, String _currency) {
        String encodedConstructor = FunctionEncoder.encodeConstructor(Arrays.<Type>asList(new org.web3j.abi.datatypes.Utf8String(_seller), 
                new org.web3j.abi.datatypes.Utf8String(_buyer), 
                new org.web3j.abi.datatypes.Utf8String(_title), 
                new org.web3j.abi.datatypes.Utf8String(_currency)));
        return deployRemoteCall(ProductSaleContract.class, web3j, transactionManager, gasPrice, gasLimit, BINARY, encodedConstructor);
    }
}
