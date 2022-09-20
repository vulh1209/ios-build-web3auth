// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/TSBC_Types.h"

#include "TSBC_EthSendRawTransactionCallback.generated.h"

/**
 * Sends a JSON-RPC request to call "eth_SendRawTransaction" using a Callback.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_EthSendRawTransactionCallback : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_DELEGATE_ThreeParams(
        FTSBC_K2_EthSendRawTransactionCallback_Delegate,
        const bool,
        bSuccess,
        const FTSBC_JsonRpcResponse&,
        Response,
        const FString&,
        TransactionHash);

public:
    /**
     * Sends a raw transaction.
     *
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent
     *        successfully.
     * @param URL The URL to send the request to.
     * @param ID The identifier to correlate the context between two objects.
     * @param SignedTransactionData The encoded and signed transaction data.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="eth_sendRawTransaction [Callback]",
        Category="3Studio|Blockchain|Ethereum|Methods")
    static void K2_EthSendRawTransactionCallback(
        FTSBC_K2_EthSendRawTransactionCallback_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ID,
        const FString SignedTransactionData);
};