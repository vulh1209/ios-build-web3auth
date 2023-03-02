// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Data/TSBC_Types.h"
#include "Math/TSBC_uint256.h"

#include "TSBC_EthEstimateGasAsyncTask.generated.h"

/**
 * Sends a JSON-RPC request to call "eth_estimateGas" using an AsyncTask.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_EthEstimateGasAsyncTask : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
        FTSBC_K2_EthEstimateGasAsyncTask_Delegate,
        const bool,
        bSuccess,
        const FTSBC_JsonRpcResponse&,
        Response,
        const FTSBC_uint256&,
        EstimatedGas);

public:
    UPROPERTY(BlueprintAssignable, Category = "3Studio|Blockchain|Ethereum|Methods")
    FTSBC_K2_EthEstimateGasAsyncTask_Delegate OnCompleted;

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
    FTSBC_uint256 _Value;

public:
    /**
     * Gets the estimated gas price for the specified transaction.
     * 
     * @param URL The URL to send the request to.
     * @param ID The identifier to correlate the context between two objects.
     * @param FromAddress The address the transaction is sent from.
     * @param ToAddress The address the transaction is directed to.
     * @param Data The hash of the invoked method signature and encoded parameters (ABI).
     * @param Value The value sent with this transaction (Optional).
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="eth_estimateGas [Async]",
        Category = "3Studio|Blockchain|Ethereum|Methods",
        Meta = (BlueprintInternalUseOnly="true"))
    static UTSBC_EthEstimateGasAsyncTask* K2_EthEstimateGasAsync(
        const FString& URL,
        const FString& ID,
        const FString& FromAddress,
        const FString& ToAddress,
        const FString& Data,
        const FTSBC_uint256 Value);

    virtual void Activate() override;
};