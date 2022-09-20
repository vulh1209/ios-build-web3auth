// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/TSBC_Types.h"
#include "Math/TSBC_uint256.h"

#include "TSBC_EthGasPriceCallback.generated.h"

/**
 * Sends a JSON-RPC request to call "eth_gasPrice" using a Callback.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_EthGasPriceCallback : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_DELEGATE_ThreeParams(
        FTSBC_K2_EthGasPriceCallback_Delegate,
        const bool,
        bSuccess,
        const FTSBC_JsonRpcResponse&,
        Response,
        const FTSBC_uint256&,
        GasPrice);

public:
    /**
     * Gets the current gas price in Wei.
     *
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent
     *                         successfully.
     * @param URL The URL to send the request to.
     * @param ID The identifier to correlate the context between two objects.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="eth_gasPrice [Callback]",
        Category="3Studio|Blockchain|Ethereum|Methods")
    static void K2_EthGasPriceCallback(
        FTSBC_K2_EthGasPriceCallback_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ID);
};