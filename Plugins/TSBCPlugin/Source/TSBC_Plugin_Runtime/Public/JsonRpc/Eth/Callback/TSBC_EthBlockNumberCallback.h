// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/TSBC_Types.h"

#include "TSBC_EthBlockNumberCallback.generated.h"

/**
 * Sends a JSON-RPC request to call "eth_blockNumber" using a Callback.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_EthBlockNumberCallback : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_DELEGATE_ThreeParams(
        FTSBC_K2_EthBlockNumberCallback_Delegate,
        const bool,
        bSuccess,
        const FTSBC_JsonRpcResponse&,
        Response,
        const int64,
        BlockNumber);

public:
    /**
     * Gets the number of the most recent block.
     *
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent successfully.
     *
     * @param URL The URL to send this request to.
     * @param ID The identifier to correlate the context between two objects.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="eth_blockNumber [Callback]",
        Category="3Studio|Blockchain|Ethereum|Methods")
    static void K2_EthBlockNumberCallback(
        FTSBC_K2_EthBlockNumberCallback_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ID);
};