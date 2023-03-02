// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Data/TSBC_Types.h"
#include "JsonRpc/Eth/TSBC_EthGetTransactionByHash.h"

#include "TSBC_EthGetTransactionByHashAsyncTask.generated.h"

/**
 * Sends a JSON-RPC request to call "eth_getTransactionByHash" using an AsyncTask.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_EthGetTransactionByHashAsyncTask : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
        FTSBC_K2_EthGetTransactionByHashAsyncTask_Delegate,
        const bool,
        bSuccess,
        const FTSBC_JsonRpcResponse&,
        Response,
        bool,
        bTransactionFound,
        const FTSBC_EthTransactionInfo&,
        TransactionInfo);

public:
    UPROPERTY(BlueprintAssignable, Category="3Studio|Blockchain|Ethereum|Methods")
    FTSBC_K2_EthGetTransactionByHashAsyncTask_Delegate OnCompleted;

private:
    UPROPERTY()
    FString _URL;

    UPROPERTY()
    FString _ID;

    UPROPERTY()
    FString _Hash;

public:
    /**
    * Gets the information about a transaction requested by transaction hash.
    * 
    * @param URL The URL to send the request to.
    * @param ID The identifier to correlate the context between two objects.
    * @param Hash Transaction hash
    */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="eth_getTransactionByHash [Async]",
        Category = "3Studio|Blockchain|Ethereum|Methods",
        Meta = (BlueprintInternalUseOnly = "true"))
    static UTSBC_EthGetTransactionByHashAsyncTask* K2_EthGetTransactionByHashAsync(
        const FString& URL,
        const FString& ID,
        const FString& Hash);

    virtual void Activate() override;
};