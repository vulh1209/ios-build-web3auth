// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Data/TSBC_Types.h"

#include "TSBC_EthCallAsyncTask.generated.h"

/**
 * Sends a JSON-RPC request to call "eth_call" using an AsyncTask.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_EthCallAsyncTask : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
        FTSBC_K2_EthCallAsyncTask_Delegate,
        const bool,
        bSuccess,
        const FTSBC_JsonRpcResponse&,
        Response,
        const FString&,
        ResponseData);

public:
    UPROPERTY(BlueprintAssignable, Category="3Studio|Blockchain|Ethereum|Methods")
    FTSBC_K2_EthCallAsyncTask_Delegate OnCompleted;

private:
    UPROPERTY()
    FString _URL;

    UPROPERTY()
    FString _ID;

    UPROPERTY()
    FString _FromAddress;

    UPROPERTY()
    FString _ToAddress;

    UPROPERTY()
    FString _Data;

    UPROPERTY()
    ETSBC_EthBlockIdentifier _BlockIdentifier;

public:
    /**
     * Calls a function of a smart contract.
     * 
     * @param URL The URL to send the request to.
     * @param ID The identifier to correlate the context between two objects.
     * @param FromAddress The address the transaction is sent from.
     * @param ToAddress The address the transaction is directed to.
     * @param Data The hash of the invoked method signature and encoded parameters (ABI).
     * @param BlockIdentifier The block number to use. (Default: "latest")
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="eth_call [Async]",
        Category = "3Studio|Blockchain|Ethereum|Methods",
        Meta = (BlueprintInternalUseOnly="true"))
    static UTSBC_EthCallAsyncTask* K2_EthCallAsync(
        const FString& URL,
        const FString& ID,
        const FString& FromAddress,
        const FString& ToAddress,
        const FString& Data,
        const ETSBC_EthBlockIdentifier BlockIdentifier = ETSBC_EthBlockIdentifier::Latest);

    virtual void Activate() override;
};