// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Data/TSBC_Types.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "TSBC_SignTransactionLowLevelCallback.generated.h"

/**
 * Signs a transaction using a Callback.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_SignTransactionLowLevelCallback : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_DELEGATE_FiveParams(
        FTSBC_K2_SignTransactionLowLevelCallback_Delegate,
        const bool,
        bSuccess,
        const FString,
        ErrorMessage,
        const FString,
        SignedTransaction,
        const FString,
        MessageHash,
        const FString,
        TransactionHash);

public:
    /**
     * Signs a transaction.
     * 
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a signing the transaction fails.
     * @param PrivateKey The private key to use for signing the transaction.
     * @param Transaction The transaction parameters.
     * @param bSignDeterministically If true, the transaction will be signed deterministically which means the same
     *                               inputs result in the same output. Otherwise, the output will be randomized even
     *                               for the same inputs.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Sign Transaction [LOW LEVEL] [Callback]",
        Category="3Studio|Blockchain|Ethereum|Advanced")
    static void SignTransactionLowLevel(
        FTSBC_K2_SignTransactionLowLevelCallback_Delegate ResponseDelegate,
        const FString PrivateKey,
        const FTSBC_EthTransaction& Transaction,
        const bool bSignDeterministically = false);
};