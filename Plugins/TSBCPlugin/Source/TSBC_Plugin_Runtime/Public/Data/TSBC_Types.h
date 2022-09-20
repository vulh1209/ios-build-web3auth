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
    PolygonMainNet     UMETA(DisplayName = "Polygon (MainNet)"),
    EthMainNet         UMETA(DisplayName = "Ethereum (MainNet)"),
    EthRopstenTestNet  UMETA(DisplayName = "Ethereum Ropsten (TestNet)"),
    EthRinkebyTestNet  UMETA(DisplayName = "Ethereum Rinkeby (TestNet)"),
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