// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Data/TSBC_Types.h"

#include "TSBC_EthGetTransactionCountAsyncTask.generated.h"

/**
 * Sends a JSON-RPC request to call "eth_getTransactionCount" using an AsyncTask.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_EthGetTransactionCountAsyncTask : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
        FTSBC_K2_EthGetTransactionCountAsyncTask_Delegate,
        const bool,
        bSuccess,
        const FTSBC_JsonRpcResponse&,
        Response,
        const int32&,
        TransactionCount);

public:
    UPROPERTY(BlueprintAssignable, Category="3Studio|Blockchain|Ethereum|Methods")
    FTSBC_K2_EthGetTransactionCountAsyncTask_Delegate OnCompleted;

private:
    UPROPERTY()
    FString _URL;

    UPROPERTY()
    FString _ID;

    UPROPERTY()
    FString _Address;

    UPROPERTY()
    ETSBC_EthBlockIdentifier _BlockIdentifier;

public:
    /**
     * Gets the transaction count for a specific address.
     * 
     * @param URL The URL to send the request to.
     * @param Address The address for which to get the transaction count for.
     * @param BlockIdentifier The block number to use. (Default: "latest")
     * @param ID The identifier to correlate the context between two objects.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="eth_getTransactionCount [Async]",
        Category = "3Studio|Blockchain|Ethereum|Methods",
        Meta = (BlueprintInternalUseOnly="true"))
    static UTSBC_EthGetTransactionCountAsyncTask* K2_EthGetTransactionCountAsync(
        const FString& URL,
        const FString& ID,
        const FString& Address,
        const ETSBC_EthBlockIdentifier BlockIdentifier);

    virtual void Activate() override;
};