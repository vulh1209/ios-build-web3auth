// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Math/TSBC_uint256.h"

#include "TSBC_EthTransactionTypes.generated.h"

/**
 * The type of the transaction
 */
UENUM(BlueprintType)
enum class ETSBC_EthTransactionType : uint8
{
    // @formatter:off
    Legacy     = 0x00   UMETA(DisplayName = "Legacy Transaction"),
    AccessList = 0x01   UMETA(DisplayName = "Access List Type"),
    DynamicFee = 0x02   UMETA(DisplayName = "Dynamic Fee"),
    MAX                 UMETA(Hidden)
    // @formatter:on
};

/**
 * Contains Transaction Receipt data
 */
USTRUCT(BlueprintType)
struct FTSBC_EthTransactionReceipt
{
    GENERATED_BODY()

    /**
     * Hash of the transaction.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString TransactionHash = "";

    /**
     * Integer of the transaction's index position in the block.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    int64 TransactionIndex = 0;

    /**
     * Hash of the block where this transaction was in.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString BlockHash = "";

    /**
     * Block number where this transaction was in.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    int64 BlockNumber = 0;

    /**
     * Address of the sender.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString From = "";

    /**
     * Address of the receiver. NULL when it's a contract creation transaction.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString To = "";

    /**
     * The total amount of gas used when this transaction was executed in the block.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FTSBC_uint256 CumulativeGasUsed = 0;

    /**
     * The sum of the base fee and tip paid per unit of gas.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FTSBC_uint256 EffectiveGasPrice = 0;

    /**
     * The amount of gas used by this specific transaction alone.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FTSBC_uint256 GasUsed = 0;

    /**
     * The contract address created, if the transaction was a contract creation, otherwise NULL.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString ContractAddress = "";

    /**
     * Array of log objects, which this transaction generated.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    TArray<FTSBC_EthLog> Logs;

    /**
     * Bloom filter for light clients to quickly retrieve related logs.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString LogsBloom = "";

    /**
     * The type of the transaction.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    ETSBC_EthTransactionType Type = ETSBC_EthTransactionType::Legacy;

    /**
     * 32 bytes of post-transaction stateroot (pre Byzantium).
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString Root = "";

    /**
     * Success status, valid only when Root is not.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    bool bStatus = false;
};

/**
 * Contains Transaction data.
 */
USTRUCT(BlueprintType)
struct FTSBC_EthTransactionInfo
{
    GENERATED_BODY()

    /**
     * Hash of the block where this transaction was in. NULL when it's pending.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString BlockHash = "";

    /**
     * Block number where this transaction was in. NULL when it's pending.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    int64 BlockNumber = 0;

    /**
     * Address of the sender.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString From = "";

    /**
     * Gas provided by the sender.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FTSBC_uint256 Gas = 0;

    /**
     * Gas price provided by the sender in Wei.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FTSBC_uint256 GasPrice = 0;

    /**
     * Hash of the transaction.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString Hash = "";

    /**
     * The data sent along with the transaction.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString Input = "";

    /**
     * The number of transactions made by the sender prior to this one.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    int32 Nonce = 0;

    /**
     * Address of the receiver. NULL when it's a contract creation transaction.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString To = "";

    /**
     * Integer of the transaction's index position in the block. NULL when it's pending.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    int64 TransactionIndex = 0;

    /**
     * Value transferred in Wei.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FTSBC_uint256 Value = 0;

    /**
     * ECDSA recovery id
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString V = "";

    /**
     * ECDSA signature r
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString R = "";

    /**
     * ECDSA signature s
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="3Studio|Blockchain")
    FString S = "";
};