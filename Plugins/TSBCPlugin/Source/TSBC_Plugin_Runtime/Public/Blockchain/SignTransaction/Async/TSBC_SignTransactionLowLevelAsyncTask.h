// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Data/TSBC_Types.h"

#include "TSBC_SignTransactionLowLevelAsyncTask.generated.h"

/**
 * Signs a transaction using an AsyncTask.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_SignTransactionLowLevelAsyncTask : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(
        FTSBC_K2_SignTransactionLowLevelAsyncTask_Delegate,
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
    FTSBC_K2_SignTransactionLowLevelAsyncTask_Delegate OnCompleted;

private:
    UPROPERTY()
    FString _PrivateKey;

    UPROPERTY()
    FTSBC_EthTransaction _Transaction;

    UPROPERTY()
    bool _bSignDeterministically;

public:
    /**
     * Signs a transaction.
     * 
     * @param PrivateKey The private key to use for signing the transaction.
     * @param Transaction The transaction parameters.
     * @param bSignDeterministically If true, the transaction will be signed deterministically which means the same
     *                               inputs result in the same output. Otherwise, the output will be randomized even
     *                               for the same inputs.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Sign Transaction [LOW LEVEL] [Async]",
        Category="3Studio|Blockchain|Ethereum|Advanced",
        Meta=(BlueprintInternalUseOnly="true"))
    static UTSBC_SignTransactionLowLevelAsyncTask* K2_SignTransactionLowLevelAsync(
        const FString& PrivateKey,
        const FTSBC_EthTransaction& Transaction,
        const bool bSignDeterministically);

    virtual void Activate() override;
};