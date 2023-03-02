// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Math/TSBC_uint256.h"

#include "TSBC_SignTransactionAsyncTask.generated.h"

/**
 * Signs a transaction using an AsyncTask.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_SignTransactionAsyncTask : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(
        FTSBC_K2_SignTransactionAsyncTask_Delegate,
        bool,
        bSuccess,
        FString,
        ErrorMessage,
        FString,
        SignedTransaction,
        FString,
        MessageHash,
        FString,
        TransactionHash);

public:
    UPROPERTY(BlueprintAssignable, Category="3Studio|Blockchain|Ethereum")
    FTSBC_K2_SignTransactionAsyncTask_Delegate OnCompleted;

private:
    UPROPERTY()
    TArray<uint8> _PrivateKey;

    UPROPERTY()
    int32 _Nonce;

    UPROPERTY()
    FTSBC_uint256 _GasPrice;

    UPROPERTY()
    FTSBC_uint256 _GasLimit;

    UPROPERTY()
    FString _ToAddress;

    UPROPERTY()
    FTSBC_uint256 _Value;

    UPROPERTY()
    FString _Data;

    UPROPERTY()
    int32 _ChainId;

public:
    /**
     * Signs a transaction.
     * 
     * @param PrivateKey The private key to use for signing the transaction.
     * @param Nonce The nonce.
     * @param GasPrice The gas price used for each gas unit.
     * @param GasLimit Gas limit provided for the transaction to execute.
     * @param ToAddress The address the transaction is directed to.
     * @param Value The amount of Ether (in Wei) to send with this transaction.
     * @param Data The hash of the invoked method signature and encoded parameters (ABI).
     * @param ChainId The Blockchain ID.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Sign Transaction [Async]",
        Category="3Studio|Blockchain|Ethereum",
        Meta=(BlueprintInternalUseOnly="true"))
    static UTSBC_SignTransactionAsyncTask* K2_SignTransactionAsync(
        const TArray<uint8>& PrivateKey,
        const int32 Nonce,
        const FTSBC_uint256& GasPrice,
        const FTSBC_uint256& GasLimit,
        const FString& ToAddress,
        const FTSBC_uint256& Value,
        const FString& Data,
        const int32 ChainId);

    virtual void Activate() override;
};