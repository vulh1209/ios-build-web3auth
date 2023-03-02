// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Data/TSBC_Types.h"

#include "TSBC_EthSendRawTransactionAsyncTask.generated.h"

/**
 * Sends a JSON-RPC request to call "eth_SendRawTransaction" using an AsyncTask.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_EthSendRawTransactionAsyncTask : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
        FTSBC_K2_EthSendRawTransactionAsyncTask_Delegate,
        const bool,
        bSuccess,
        const FTSBC_JsonRpcResponse&,
        Response,
        const FString&,
        TransactionHash);

public:
    UPROPERTY(BlueprintAssignable, Category="3Studio|Blockchain|Ethereum|Methods")
    FTSBC_K2_EthSendRawTransactionAsyncTask_Delegate OnCompleted;

private:
    UPROPERTY()
    FString _URL;

    UPROPERTY()
    FString _ID;

    UPROPERTY()
    FString _SignedTransactionData;

public:
    /**
     * Sends a raw transaction.
     *
     * @param URL The URL to send the request to.
     * @param ID The identifier to correlate the context between two objects.
     * @param SignedTransactionData The encoded and signed transaction data.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="eth_sendRawTransaction [Async]",
        Category = "3Studio|Blockchain|Ethereum|Methods",
        Meta = (BlueprintInternalUseOnly="true"))
    static UTSBC_EthSendRawTransactionAsyncTask* K2_EthSendRawTransactionAsync(
        const FString& URL,
        const FString& ID,
        const FString& SignedTransactionData);

    virtual void Activate() override;
};