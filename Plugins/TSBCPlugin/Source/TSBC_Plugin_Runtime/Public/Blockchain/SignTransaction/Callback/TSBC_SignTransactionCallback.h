// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Math/TSBC_uint256.h"

#include "TSBC_SignTransactionCallback.generated.h"

/**
 * Signs a transaction using a Callback.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_SignTransactionCallback : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_DELEGATE_FiveParams(
        FTSBC_K2_SignTransactionCallback_Delegate,
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
     * @param Nonce The nonce.
     * @param GasPrice The gas price used for each gas unit.
     * @param GasLimit Gas limit provided for the transaction to execute.
     * @param ToAddress The address the transaction is directed to.
     * @param Value The amount of Ether (in Wei) to send with this transaction.
     * @param Data The hash of the invoked method signature and encoded parameters (ABI).
     * @param ChainId The Blockchain ID.
     * @param bSignDeterministically If true, the transaction will be signed deterministically which means the same
     *                               inputs result in the same output. Otherwise, the output will be randomized even
     *                               for the same inputs.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Sign Transaction [Callback]",
        Category="3Studio|Blockchain|Ethereum")
    static void K2_SignTransactionCallback(
        FTSBC_K2_SignTransactionCallback_Delegate ResponseDelegate,
        const TArray<uint8> PrivateKey,
        const int32 Nonce,
        const FTSBC_uint256 GasPrice,
        const FTSBC_uint256 GasLimit,
        const FString ToAddress,
        const FTSBC_uint256 Value,
        const FString Data,
        const int32 ChainId,
        const bool bSignDeterministically = false);
};