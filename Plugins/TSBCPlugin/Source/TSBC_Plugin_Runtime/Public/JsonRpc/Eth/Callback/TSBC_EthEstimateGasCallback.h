// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/TSBC_Types.h"
#include "Math/TSBC_uint256.h"

#include "TSBC_EthEstimateGasCallback.generated.h"

/**
 * Sends a JSON-RPC request to call "eth_estimateGas" using a Callback.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_EthEstimateGasCallback : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_DELEGATE_ThreeParams(
        FTSBC_K2_EthEstimateGasCallback_Delegate,
        const bool,
        bSuccess,
        const FTSBC_JsonRpcResponse&,
        Response,
        const FTSBC_uint256&,
        EstimatedGas);

public:
    /**
     * Gets the estimated gas price for the specified transaction.
     * 
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent
     *                         successfully.
     * @param URL The URL to send the request to.
     * @param ID The identifier to correlate the context between two objects.
     * @param FromAddress The address the transaction is sent from.
     * @param ToAddress The address the transaction is directed to.
     * @param Data The hash of the invoked method signature and encoded parameters (ABI).
     * @param Value The value sent with this transaction (Optional).
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="eth_estimateGas [Callback]",
        Category="3Studio|Blockchain|Ethereum|Methods")
    static void K2_EthEstimateGasCallback(
        FTSBC_K2_EthEstimateGasCallback_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ID,
        const FString& FromAddress,
        const FString& ToAddress,
        const FString& Data,
        const FTSBC_uint256 Value);
};