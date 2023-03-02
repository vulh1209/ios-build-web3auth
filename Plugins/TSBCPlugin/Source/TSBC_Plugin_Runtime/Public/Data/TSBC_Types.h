// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"

#include "TSBC_Types.generated.h"

/**
 * Defines properties about a Blockchain like its RPC URL, network name, etc.
 */
USTRUCT(BlueprintType)
struct FTSBC_BlockchainConfig
{
    GENERATED_BODY()

    /**
     * The RPC URL of this Blockchain
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString RpcUrl = "";

    /**
     * The network name of this Blockchain, e.g. "Ethereum MainNet", "Ethereum Testnet (Rinkeby)", etc.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString NetworkName = "";

    /**
     * The Chain ID of this Blockchain
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    int32 ChainId = 0;

    /**
     * The Symbol of this Blockchain, e.g. "ETH", "BNB", etc.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString Symbol = "";
};

/**
 * Holds the response data for an HTTP request.
 */
USTRUCT(BlueprintType)
struct FTSBC_HttpResponse
{
    GENERATED_BODY()

    /**
     * True if a response was received from a server and the status code is 2xx -> "Success".
     *
     * NOTE: If this is true it does only mean that there was no error on the data transport layer (HTTP).
     *       You also need to check the response body to determine the actual outcome of the call.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|HTTP")
    bool bSuccess = false;

    /**
     * Response status code (HTTP status code, e.g. 200 -> "OK")
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|HTTP")
    int32 StatusCode = -1;

    /**
     * Response headers in the format of "Key: Value"
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|HTTP")
    TArray<FString> Headers;

    /**
     * Response body
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|HTTP")
    FString Body = "";
};

/**
 * Holds the response data for a JSON-RPC request.
 */
USTRUCT(BlueprintType)
struct FTSBC_JsonRpcResponse
{
    GENERATED_BODY()

    /**
     * True if a response was received from a server and the status code is 2xx -> "Success".
     *
     * NOTE: If this is true it does only mean that there was no error on the data transport layer (HTTP).
     *       You also need to check the response body to determine the actual outcome of the called JSON-RPC method.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|JSON-RPC")
    bool bSuccess = false;

    /**
     * Response status code (HTTP status code, e.g. 200 -> "OK")
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|JSON-RPC")
    int32 StatusCode = -1;

    /**
     * Response headers in the format of "Key: Value"
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|JSON-RPC")
    TArray<FString> Headers;

    /**
     * Response body
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="3Studio|JSON-RPC")
    FString Body = "";
};

/**
 * Supported Ethereum networks.
 */
UENUM(BlueprintType)
enum class ETSBC_EthereumNetwork : uint8
{
    // @formatter:off
    PolygonMainNet              UMETA(DisplayName = "Polygon (MainNet)"),
    EthMainNet                  UMETA(DisplayName = "Ethereum (MainNet)"),
    EthRopstenTestNet           UMETA(DisplayName = "Ethereum Ropsten (TestNet)"),
    EthRinkebyTestNet           UMETA(DisplayName = "Ethereum Rinkeby (TestNet)"),
    PolygonMumbaiTestNet        UMETA(DisplayName = "Polygon Mumbai (TestNet)"),
    EthGoerliTestNet            UMETA(DisplayName = "Ethereum Goerli (TestNet)"),
    EthSepoliaTestNet           UMETA(DisplayName = "Ethereum Sepolia (TestNet)"),
    AvalancheMainNet            UMETA(DisplayName = "Avalanche (Mainnet)"),
    AvalancheFujiTestNet        UMETA(DisplayName = "Avalanche Fuji (TestNet)"),
    BinanceSmartChainMainNet    UMETA(DisplayName = "Binance Smart Chain (MainNet)"),
    BinanceSmartChainTestNet    UMETA(DisplayName = "Binance Smart Chain (TestNet)"),
    CronosMainNet               UMETA(DisplayName = "Cronos (MainNet)"),
    CronosTestNet               UMETA(DisplayName = "Cronos (TestNet)"),
    HuobiEcoChainMainNet        UMETA(DisplayName = "Huobi Eco Chain (MainNet)"),
    HuobiEcoChainTestNet        UMETA(DisplayName = "Huobi Eco Chain (TestNet)"),
    // @formatter:on
};

/**
 * Ethereum units.
 */
UENUM(BlueprintType)
enum class ETSBC_EthereumUnit : uint8
{
    // @formatter:off
    Wei     UMETA(DisplayName = "Wei"),
    Kwei    UMETA(DisplayName = "Kwei (Also: Babbage, Femtoether)"),
    Mwei    UMETA(DisplayName = "Mwei (Also: Lovelace, Picoether)"),
    Gwei    UMETA(DisplayName = "Gwei (Also: Shannon, Nanoether, Nano)"),
    Szabo   UMETA(DisplayName = "Szabo (Also: Microether, Micro)"),
    Finney  UMETA(DisplayName = "Finney (Also: Milliether, Milli)"),
    Ether   UMETA(DisplayName = "Ether"),
    Kether  UMETA(DisplayName = "Kether (Also: Grand)"),
    Mether  UMETA(DisplayName = "Mether"),
    Gether  UMETA(DisplayName = "Gether"),
    Tether  UMETA(DisplayName = "Tether")
    // @formatter:on
};

/**
 * The block number that will be used for any JSON-RPC method that accepts a block identifier.
 */
UENUM(BlueprintType)
enum class ETSBC_EthBlockIdentifier : uint8
{
    // @formatter:off
    Latest    UMETA(DisplayName = "Latest"),
    Earliest  UMETA(DisplayName = "Earliest"),
    Pending   UMETA(DisplayName = "Pending"),
    // @formatter:on
};

/**
 * Holds information about a transaction that can be sent to an Ethereum Blockchain.
 *
 * Every value needs to be specified in hex notation including a "0x" prefix.
 */
USTRUCT(BlueprintType)
struct FTSBC_EthTransaction
{
    GENERATED_BODY()

    /**
     * The nonce.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString Nonce = "";

    /**
     * The gas price used for each gas unit.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString GasPrice = "";

    /**
     * Gas limit provided for the transaction to execute.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString GasLimit = "";

    /**
     * The address the transaction is directed to. 
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString ToAddress = "";

    /**
     * The amount of Ether (in Wei) to send with this transaction.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString Value = "";

    /**
     * The hash of the invoked method signature and encoded parameters (ABI).
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString Data = "";

    /**
     * The Blockchain ID. 
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString ChainId = "";
};

UENUM(BlueprintType)
enum class ETSBC_NumberBase : uint8
{
    // @formatter:off
    Bin  UMETA(DisplayName = "Base  2 | Binary"),
    Oct  UMETA(DisplayName = "Base  7 | Octal"),
    Dec  UMETA(DisplayName = "Base 10 | Decimal"),
    Hex  UMETA(DisplayName = "Base 16 | Hex"),
    // @formatter:on
};

/**
 * Defines properties about a Layer2 network like its URL, network name, etc.
 */
USTRUCT(BlueprintType)
struct FTSBC_Layer2Config
{
    GENERATED_BODY()

    /**
     * The URL of this Blockchain
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString RpcUrl = "";

    /**
     * The network name of this Layer2 network, e.g. "Loopring (MainNet)", "ImmutableX (TestNet)", etc.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString NetworkName = "";
};

/**
 * Supported Layer2 networks.
 */
UENUM(BlueprintType)
enum class ETSBC_Layer2Network : uint8
{
    // @formatter:off
    LoopringMainNet             UMETA(DisplayName = "Loopring (MainNet)"),
    LoopringTestNet2            UMETA(DisplayName = "Loopring (TestNet2)"),
    LoopringTestNet3            UMETA(DisplayName = "Loopring (TestNet3)"),
    ImmutablexMainNet           UMETA(DisplayName = "ImmutableX (MainNet)"),
    ImmutablexSandboxTestNet    UMETA(DisplayName = "ImmutableX Sandbox (TestNet)"),
    // @formatter:on
};

/**
 * Type of the filter used in creation of a log
 */
UENUM(BlueprintType)
enum class ETSBC_EthLogFilterType : uint8
{
    // @formatter:off
    Block       UMETA(DisplayName="New Block"),
    Pending     UMETA(DisplayName="New Pending Transaction"),
    General     UMETA(DisplayName="General")
    // @formatter:on
};

/**
 * Holds information about an Ethereum log
 */
USTRUCT(BlueprintType)
struct FTSBC_EthLog
{
    GENERATED_BODY()

    /**
     * The filter type used to create this log object.
     *
     * Block   -> Only BlockHash will be provided
     * Pending -> Only TransactionHash will be provided
     * General -> All properties will be provided
     *
     * https://ethereum.org/en/developers/docs/apis/json-rpc/#eth_getfilterchanges
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    ETSBC_EthLogFilterType FilterType = ETSBC_EthLogFilterType::General;

    /**
     * True when the log was removed, due to a chain reorganization. False if it's a valid log.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    bool bRemoved = false;

    /**
     * Integer of the log index position in the block. NULL when it's pending log.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    int64 LogIndex = 0;

    /**
     * Integer of the transactions index position log was created from. NULL when it's pending log.
     * If FilterType is "New Pending Transaction", the transaction hash will be located here.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString TransactionHash = "";

    /**
     * Hash of the transactions this log was created from. NULL when it's pending log.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    int64 TransactionIndex = 0;

    /**
     * Hash of the block where this log was in. NULL when it's pending. NULL when it's pending log.
     * If FilterType is "New Block", the block hash will be located here.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString BlockHash = "";

    /**
     * The block number where this log was in. NULL when it's pending. NULL when it's pending log.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    int64 BlockNumber = 0;

    /**
     * Address from which this log originated.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString Address = "";

    /**
     * Contains one or more 32 Bytes non-indexed arguments of the log.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString Data = "";

    /**
     * Array of 0 to 4 32 Bytes DATA of indexed log arguments.
     * 
     * In Solidity:
     * The first topic is the hash of the signature of the event (e.g. Deposit(address,bytes32,uint256)),
     * except you declared the event with the anonymous specifier.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|BlockChain")
    TArray<FString> Topics;
};