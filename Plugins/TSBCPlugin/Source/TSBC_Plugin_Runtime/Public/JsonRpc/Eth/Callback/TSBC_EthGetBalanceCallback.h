// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/TSBC_Types.h"
#include "Math/TSBC_uint256.h"

#include "TSBC_EthGetBalanceCallback.generated.h"

/**
 * Sends a JSON-RPC request to call "eth_getBalance" using a Callback.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_EthGetBalanceCallback : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_DELEGATE_ThreeParams(
        FTSBC_K2_EthGetBalanceCallback_Delegate,
        const bool,
        bSuccess,
        const FTSBC_JsonRpcResponse&,
        Response,
        const FTSBC_uint256&,
        Balance);

public:
    /**
     * Gets the balance in Wei for the specified account.
     *
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent
     *                         successfully.
     * @param URL The URL to send the request to.
     * @param ID The identifier to correlate the context between two objects.
     * @param AccountAddress Address of the account to check.
     * @param BlockIdentifier The block number to use. (Default: "latest")
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="eth_getBalance [Callback]",
        Category="3Studio|Blockchain|Ethereum|Methods")
    static void K2_EthGetBalanceCallback(
        FTSBC_K2_EthGetBalanceCallback_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ID,
        const FString AccountAddress,
        const ETSBC_EthBlockIdentifier BlockIdentifier = ETSBC_EthBlockIdentifier::Latest);
};