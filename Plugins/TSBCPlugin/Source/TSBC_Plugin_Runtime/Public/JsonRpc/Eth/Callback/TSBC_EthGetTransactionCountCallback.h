// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/TSBC_Types.h"

#include "TSBC_EthGetTransactionCountCallback.generated.h"

/**
 * Sends a JSON-RPC request to call "eth_getTransactionCount" using a Callback.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_EthGetTransactionCountCallback : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_DELEGATE_ThreeParams(
        FTSBC_K2_EthGetTransactionCountCallback_Delegate,
        const bool,
        bSuccess,
        const FTSBC_JsonRpcResponse&,
        Response,
        const int32&,
        TransactionCount);

public:
    /**
     * Gets the transaction count for a specific address.
     * 
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent
     *                         successfully.
     * @param URL The URL to send the request to.
     * @param Address The address for which to get the transaction count for.
     * @param BlockIdentifier The block number to use. (Default: "latest")
     * @param ID The identifier to correlate the context between two objects.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="eth_getTransactionCount [Callback]",
        Category="3Studio|Blockchain|Ethereum|Methods")
    static void K2_EthGetTransactionCount(
        FTSBC_K2_EthGetTransactionCountCallback_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ID,
        const FString& Address,
        const ETSBC_EthBlockIdentifier BlockIdentifier = ETSBC_EthBlockIdentifier::Latest);
};