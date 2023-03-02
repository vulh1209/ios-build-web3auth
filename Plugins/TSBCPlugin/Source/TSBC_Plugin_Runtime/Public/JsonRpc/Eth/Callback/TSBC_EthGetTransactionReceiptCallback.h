// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/TSBC_Types.h"
#include "JsonRpc/Eth/TSBC_EthGetTransactionReceipt.h"

#include "TSBC_EthGetTransactionReceiptCallback.generated.h"

/**
 * Sends a JSON-RPC request to call "eth_getTransactionByHash" using a Callback.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_EthGetTransactionReceiptCallback : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_DELEGATE_FourParams(
        FTSBC_K2_EthGetTransactionCallback_Delegate,
        const bool,
        bSuccess,
        const FTSBC_JsonRpcResponse&,
        Response,
        const bool,
        bReceiptFound,
        const FTSBC_EthTransactionReceipt&,
        TransactionReceipt);

public:
    /**
     * Gets the receipt of a transaction by transaction hash.
     *
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent successfully.
     *
     * @param URL The URL to send the request to.
     * @param ID The identifier to correlate the context between two objects.
     * @param Hash Transaction hash.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName = "eth_getTransactionReceipt [Callback]",
        Category="3Studio|Blockchain|Ethereum|Methods")
    static void K2_EthTransactionReceiptCallback(
        FTSBC_K2_EthGetTransactionCallback_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ID,
        const FString& Hash);
};