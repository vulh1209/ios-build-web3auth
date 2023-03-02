// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/TSBC_Types.h"
#include "JsonRpc/Eth/TSBC_EthGetTransactionByHash.h"

#include "TSBC_EthGetTransactionByHashCallback.generated.h"

/**
 * Sends a JSON-RPC request to call "eth_getTransactionByHash" using a Callback.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_EthGetTransactionByHashCallback : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_DELEGATE_FourParams(
        FTSBC_K2_EthGetTransactionByHashCallback_Delegate,
        const bool,
        bSuccess,
        const FTSBC_JsonRpcResponse&,
        Response,
        bool,
        bTransactionFound,
        const FTSBC_EthTransactionInfo&,
        TransactionInfo);

public:
    /**
    * Gets the information about a transaction requested by transaction hash.
    * 
    * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent
    *                         successfully.
    * @param URL The URL to send the request to.
    * @param ID The identifier to correlate the context between two objects.
    * @param Hash Transaction hash
    */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="eth_getTransactionByHash [Callback]",
        Category="3Studio|Blockchain|Ethereum|Methods")
    static void K2_EthGetTransactionByHash(
        FTSBC_K2_EthGetTransactionByHashCallback_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ID,
        const FString& Hash
    );
};