// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Data/TSBC_Types.h"
#include "Math/TSBC_uint256.h"

#include "TSBC_EthGasPriceAsyncTask.generated.h"

/**
 * Sends a JSON-RPC request to call "eth_gasPrice" using an AsyncTask.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_EthGasPriceAsyncTask : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
        FTSBC_K2_EthGasPriceAsyncTask_Delegate,
        const bool,
        bSuccess,
        const FTSBC_JsonRpcResponse&,
        Response,
        const FTSBC_uint256&,
        GasPrice);

public:
    UPROPERTY(BlueprintAssignable, Category="3Studio|Blockchain|Ethereum|Methods")
    FTSBC_K2_EthGasPriceAsyncTask_Delegate OnCompleted;

private:
    UPROPERTY()
    FString _URL;

    UPROPERTY()
    FString _ID;

public:
    /**
     * Gets the current gas price in Wei.
     *
     * @param URL The URL to send the request to.
     * @param ID The identifier to correlate the context between two objects.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="eth_gasPrice [Async]",
        Category = "3Studio|Blockchain|Ethereum|Methods",
        Meta = (BlueprintInternalUseOnly="true"))
    static UTSBC_EthGasPriceAsyncTask* K2_EthGasPriceAsync(
        const FString& URL,
        const FString& ID);

    virtual void Activate() override;
};