// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Data/TSBC_Types.h"
#include "Math/TSBC_uint256.h"

#include "TSBC_EthGetBalanceAsyncTask.generated.h"

/**
 * Sends a JSON-RPC request to call "eth_getBalance" using an AsyncTask.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_EthGetBalanceAsyncTask : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
        FTSBC_K2_EthGetBalanceAsyncTask_Delegate,
        const bool,
        bSuccess,
        const FTSBC_JsonRpcResponse&,
        Response,
        const FTSBC_uint256&,
        Balance);

public:
    UPROPERTY(BlueprintAssignable, Category="3Studio|Blockchain|Ethereum|Methods")
    FTSBC_K2_EthGetBalanceAsyncTask_Delegate OnCompleted;

private:
    UPROPERTY()
    FString _URL;

    UPROPERTY()
    FString _ID;

    UPROPERTY()
    FString _AccountAddress;

    UPROPERTY()
    ETSBC_EthBlockIdentifier _BlockIdentifier;

public:
    /**
     * Gets the balance in Wei for the specified account.
     *
     * @param URL The URL to send the request to.
     * @param ID The identifier to correlate the context between two objects.
     * @param AccountAddress Address of the account to check.
     * @param BlockIdentifier The block number to use. (Default: "latest")
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="eth_getBalance [Async]",
        Category = "3Studio|Blockchain|Ethereum|Methods",
        Meta = (BlueprintInternalUseOnly="true"))
    static UTSBC_EthGetBalanceAsyncTask* K2_EthGetBalanceAsync(
        const FString& URL,
        const FString& ID,
        const FString& AccountAddress,
        const ETSBC_EthBlockIdentifier BlockIdentifier = ETSBC_EthBlockIdentifier::Latest);

    virtual void Activate() override;
};