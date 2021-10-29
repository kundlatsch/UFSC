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
import org.web3j.abi.datatypes.generated.Uint256;
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
public class PaperContract extends Contract {
    public static final String BINARY = "6080604052600b805462ffffff1916905534801561001c57600080fd5b50604051610b05380380610b058339818101604052604081101561003f57600080fd5b810190808051604051939291908464010000000082111561005f57600080fd5b90830190602082018581111561007457600080fd5b825164010000000081118282018810171561008e57600080fd5b82525081516020918201929091019080838360005b838110156100bb5781810151838201526020016100a3565b50505050905090810190601f1680156100e85780820380516001836020036101000a031916815260200191505b506040526020018051604051939291908464010000000082111561010b57600080fd5b90830190602082018581111561012057600080fd5b825164010000000081118282018810171561013a57600080fd5b82525081516020918201929091019080838360005b8381101561016757818101518382015260200161014f565b50505050905090810190601f1680156101945780820380516001836020036101000a031916815260200191505b50604052505082516101ae915060009060208501906101e8565b5080516101c29060019060208401906101e8565b5050600060038190556005805460ff1916905560078190556009819055600a5550610283565b828054600181600116156101000203166002900490600052602060002090601f016020900481019282601f1061022957805160ff1916838001178555610256565b82800160010185558215610256579182015b8281111561025657825182559160200191906001019061023b565b50610262929150610266565b5090565b61028091905b80821115610262576000815560010161026c565b90565b610873806102926000396000f3fe608060405234801561001057600080fd5b506004361061010b5760003560e01c80637d8730d6116100a2578063ca7a82bd11610071578063ca7a82bd1461028c578063e4173b2c14610294578063e4a302c0146103c3578063efa3ca5f14610469578063fa0c1513146104715761010b565b80637d8730d6146102305780637faf6b161461024d5780638d56bd941461026a5780639481da53146102845761010b565b80634e69d560116100de5780634e69d5601461016257806354adef8b1461018e5780635789c7d014610196578063603daf9a146101b35761010b565b80630eedb04c146101105780630f15f4c01461011a5780630f64601714610122578063333a2bff14610145575b600080fd5b610118610479565b005b610118610558565b6101186004803603604081101561013857600080fd5b508035906020013561056b565b6101186004803603602081101561015b57600080fd5b5035610584565b61016a610589565b6040518082600381111561017a57fe5b60ff16815260200191505060405180910390f35b610118610593565b610118600480360360208110156101ac57600080fd5b50356105a2565b6101bb6105af565b6040805160208082528351818301528351919283929083019185019080838360005b838110156101f55781810151838201526020016101dd565b50505050905090810190601f1680156102225780820380516001836020036101000a031916815260200191505b509250505060405180910390f35b6101186004803603602081101561024657600080fd5b5035610644565b6101186004803603602081101561026357600080fd5b5035610649565b61027261064e565b60408051918252519081900360200190f35b610272610654565b61011861065a565b610118600480360360608110156102aa57600080fd5b8101906020810181356401000000008111156102c557600080fd5b8201836020820111156102d757600080fd5b803590602001918460018302840111640100000000831117156102f957600080fd5b91908080601f016020809104026020016040519081016040528093929190818152602001838380828437600092019190915250929594936020810193503591505064010000000081111561034c57600080fd5b82018360208201111561035e57600080fd5b8035906020019184600183028401116401000000008311171561038057600080fd5b91908080601f016020809104026020016040519081016040528093929190818152602001838380828437600092019190915250929550509135925061066e915050565b610118600480360360208110156103d957600080fd5b8101906020810181356401000000008111156103f457600080fd5b82018360208201111561040657600080fd5b8035906020019184600183028401116401000000008311171561042857600080fd5b91908080601f016020809104026020016040519081016040528093929190818152602001838380828437600092019190915250929550610703945050505050565b610118610716565b6101bb610727565b600b5460ff6101009091041615156001141561052a57600954601e01600a54116104c357600854600954116104be57600580546002919060ff19166001835b02179055505b610525565b600954601e01600a541180156104e05750600b5460ff1615156001145b156104f957600580546002919060ff19166001836104b8565b600954601e01600a541180156105125750600b5460ff16155b15610525576005805460ff191660031790555b610556565b600b5462010000900460ff1615156001141561055657600580546003919060ff19166001835b02179055505b565b600580546001919060ff19168280610550565b60026008540382116105805760088054820190555b5050565b600955565b60055460ff165b90565b600b805460ff19166001179055565b6007819055600a01600855565b60018054604080516020601f6002600019610100878916150201909516949094049384018190048102820181019092528281526060939092909183018282801561063a5780601f1061060f5761010080835404028352916020019161063a565b820191906000526020600020905b81548152906001019060200180831161061d57829003601f168201915b5050505050905090565b600a55565b600655565b60065490565b60035490565b600580546003919060ff1916600183610550565b610676610785565b8381526020808201849052604082018390526004805460018101808355600092909252835180519293859360039093027f8a35acfbc15ff81a39ae7d344fd709f28e8600b4aa8c65c6b64bfe7fe36bd19b01926106d692849201906107a6565b5060208281015180516106ef92600185019201906107a6565b506040820151816002015550505050505050565b80516105809060029060208401906107a6565b600b805461ff001916610100179055565b60028054604080516020601f600019610100600187161502019094168590049384018190048102820181019092528281526060939092909183018282801561063a5780601f1061060f5761010080835404028352916020019161063a565b60405180606001604052806060815260200160608152602001600081525090565b828054600181600116156101000203166002900490600052602060002090601f016020900481019282601f106107e757805160ff1916838001178555610814565b82800160010185558215610814579182015b828111156108145782518255916020019190600101906107f9565b50610820929150610824565b5090565b61059091905b80821115610820576000815560010161082a56fea265627a7a7231582090db0db3077b653d9fecb5117d08dfb9def451f42a0da82ac2fcaf9a87e157ba64736f6c63430005110032";

    public static final String FUNC_ACTIVATE = "activate";

    public static final String FUNC_ADDPRODUCT = "addProduct";

    public static final String FUNC_DELIVERALL = "deliverAll";

    public static final String FUNC_GETBUYER = "getBuyer";

    public static final String FUNC_GETBUYERADDRESS = "getBuyerAddress";

    public static final String FUNC_GETCONTRACTTOTALPRICE = "getContractTotalPrice";

    public static final String FUNC_GETEFFECTIVEDAY = "getEffectiveDay";

    public static final String FUNC_GETSTATUS = "getStatus";

    public static final String FUNC_NOTIFIESDELAY = "notifiesDelay";

    public static final String FUNC_PAYEDFINE = "payedFine";

    public static final String FUNC_SETBUYERADDRESS = "setBuyerAddress";

    public static final String FUNC_SETDELIVERYDAY = "setDeliveryDay";

    public static final String FUNC_SETEFFECTIVEDAY = "setEffectiveDay";

    public static final String FUNC_SETPAYMENTDAY = "setPaymentDay";

    public static final String FUNC_SETSOLICITATIONDAY = "setSolicitationDay";

    public static final String FUNC_UNSUCCESSFULTERMINATE = "unsuccessfulTerminate";

    public static final String FUNC_VERIFYCONTRACTTERMINATION = "verifyContractTermination";

    @Deprecated
    protected PaperContract(String contractAddress, Web3j web3j, Credentials credentials, BigInteger gasPrice, BigInteger gasLimit) {
        super(BINARY, contractAddress, web3j, credentials, gasPrice, gasLimit);
    }

    protected PaperContract(String contractAddress, Web3j web3j, Credentials credentials, ContractGasProvider contractGasProvider) {
        super(BINARY, contractAddress, web3j, credentials, contractGasProvider);
    }

    @Deprecated
    protected PaperContract(String contractAddress, Web3j web3j, TransactionManager transactionManager, BigInteger gasPrice, BigInteger gasLimit) {
        super(BINARY, contractAddress, web3j, transactionManager, gasPrice, gasLimit);
    }

    protected PaperContract(String contractAddress, Web3j web3j, TransactionManager transactionManager, ContractGasProvider contractGasProvider) {
        super(BINARY, contractAddress, web3j, transactionManager, contractGasProvider);
    }

    public RemoteFunctionCall<TransactionReceipt> activate() {
        final Function function = new Function(
                FUNC_ACTIVATE, 
                Arrays.<Type>asList(), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> addProduct(String name, String group, BigInteger quantity) {
        final Function function = new Function(
                FUNC_ADDPRODUCT, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Utf8String(name), 
                new org.web3j.abi.datatypes.Utf8String(group), 
                new org.web3j.abi.datatypes.generated.Int256(quantity)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> deliverAll() {
        final Function function = new Function(
                FUNC_DELIVERALL, 
                Arrays.<Type>asList(), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<String> getBuyer() {
        final Function function = new Function(FUNC_GETBUYER, 
                Arrays.<Type>asList(), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Utf8String>() {}));
        return executeRemoteCallSingleValueReturn(function, String.class);
    }

    public RemoteFunctionCall<String> getBuyerAddress() {
        final Function function = new Function(FUNC_GETBUYERADDRESS, 
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

    public RemoteFunctionCall<BigInteger> getEffectiveDay() {
        final Function function = new Function(FUNC_GETEFFECTIVEDAY, 
                Arrays.<Type>asList(), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Uint256>() {}));
        return executeRemoteCallSingleValueReturn(function, BigInteger.class);
    }

    public RemoteFunctionCall<BigInteger> getStatus() {
        final Function function = new Function(FUNC_GETSTATUS, 
                Arrays.<Type>asList(), 
                Arrays.<TypeReference<?>>asList(new TypeReference<Uint8>() {}));
        return executeRemoteCallSingleValueReturn(function, BigInteger.class);
    }

    public RemoteFunctionCall<TransactionReceipt> notifiesDelay(BigInteger notificationDay, BigInteger delay) {
        final Function function = new Function(
                FUNC_NOTIFIESDELAY, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(notificationDay), 
                new org.web3j.abi.datatypes.generated.Uint256(delay)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> payedFine() {
        final Function function = new Function(
                FUNC_PAYEDFINE, 
                Arrays.<Type>asList(), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> setBuyerAddress(String _buyerAddress) {
        final Function function = new Function(
                FUNC_SETBUYERADDRESS, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.Utf8String(_buyerAddress)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> setDeliveryDay(BigInteger _deliveryDay) {
        final Function function = new Function(
                FUNC_SETDELIVERYDAY, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(_deliveryDay)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> setEffectiveDay(BigInteger _effectiveDay) {
        final Function function = new Function(
                FUNC_SETEFFECTIVEDAY, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(_effectiveDay)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> setPaymentDay(BigInteger _paymentDay) {
        final Function function = new Function(
                FUNC_SETPAYMENTDAY, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(_paymentDay)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> setSolicitationDay(BigInteger _solicitationDay) {
        final Function function = new Function(
                FUNC_SETSOLICITATIONDAY, 
                Arrays.<Type>asList(new org.web3j.abi.datatypes.generated.Uint256(_solicitationDay)), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> unsuccessfulTerminate() {
        final Function function = new Function(
                FUNC_UNSUCCESSFULTERMINATE, 
                Arrays.<Type>asList(), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    public RemoteFunctionCall<TransactionReceipt> verifyContractTermination() {
        final Function function = new Function(
                FUNC_VERIFYCONTRACTTERMINATION, 
                Arrays.<Type>asList(), 
                Collections.<TypeReference<?>>emptyList());
        return executeRemoteCallTransaction(function);
    }

    @Deprecated
    public static PaperContract load(String contractAddress, Web3j web3j, Credentials credentials, BigInteger gasPrice, BigInteger gasLimit) {
        return new PaperContract(contractAddress, web3j, credentials, gasPrice, gasLimit);
    }

    @Deprecated
    public static PaperContract load(String contractAddress, Web3j web3j, TransactionManager transactionManager, BigInteger gasPrice, BigInteger gasLimit) {
        return new PaperContract(contractAddress, web3j, transactionManager, gasPrice, gasLimit);
    }

    public static PaperContract load(String contractAddress, Web3j web3j, Credentials credentials, ContractGasProvider contractGasProvider) {
        return new PaperContract(contractAddress, web3j, credentials, contractGasProvider);
    }

    public static PaperContract load(String contractAddress, Web3j web3j, TransactionManager transactionManager, ContractGasProvider contractGasProvider) {
        return new PaperContract(contractAddress, web3j, transactionManager, contractGasProvider);
    }

    public static RemoteCall<PaperContract> deploy(Web3j web3j, Credentials credentials, ContractGasProvider contractGasProvider, String _seller, String _buyer) {
        String encodedConstructor = FunctionEncoder.encodeConstructor(Arrays.<Type>asList(new org.web3j.abi.datatypes.Utf8String(_seller), 
                new org.web3j.abi.datatypes.Utf8String(_buyer)));
        return deployRemoteCall(PaperContract.class, web3j, credentials, contractGasProvider, BINARY, encodedConstructor);
    }

    public static RemoteCall<PaperContract> deploy(Web3j web3j, TransactionManager transactionManager, ContractGasProvider contractGasProvider, String _seller, String _buyer) {
        String encodedConstructor = FunctionEncoder.encodeConstructor(Arrays.<Type>asList(new org.web3j.abi.datatypes.Utf8String(_seller), 
                new org.web3j.abi.datatypes.Utf8String(_buyer)));
        return deployRemoteCall(PaperContract.class, web3j, transactionManager, contractGasProvider, BINARY, encodedConstructor);
    }

    @Deprecated
    public static RemoteCall<PaperContract> deploy(Web3j web3j, Credentials credentials, BigInteger gasPrice, BigInteger gasLimit, String _seller, String _buyer) {
        String encodedConstructor = FunctionEncoder.encodeConstructor(Arrays.<Type>asList(new org.web3j.abi.datatypes.Utf8String(_seller), 
                new org.web3j.abi.datatypes.Utf8String(_buyer)));
        return deployRemoteCall(PaperContract.class, web3j, credentials, gasPrice, gasLimit, BINARY, encodedConstructor);
    }

    @Deprecated
    public static RemoteCall<PaperContract> deploy(Web3j web3j, TransactionManager transactionManager, BigInteger gasPrice, BigInteger gasLimit, String _seller, String _buyer) {
        String encodedConstructor = FunctionEncoder.encodeConstructor(Arrays.<Type>asList(new org.web3j.abi.datatypes.Utf8String(_seller), 
                new org.web3j.abi.datatypes.Utf8String(_buyer)));
        return deployRemoteCall(PaperContract.class, web3j, transactionManager, gasPrice, gasLimit, BINARY, encodedConstructor);
    }
}
