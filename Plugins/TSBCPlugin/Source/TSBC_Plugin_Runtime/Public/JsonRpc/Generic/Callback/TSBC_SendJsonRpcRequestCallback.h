// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/TSBC_Types.h"

#include "TSBC_SendJsonRpcRequestCallback.generated.h"

UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_SendJsonRpcRequestCallback : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_DELEGATE_OneParam(
        FTSBC_K2_JsonRpcResponseCallback_Delegate,
        const FTSBC_JsonRpcResponse&,
        Response);

public:
    /**
     * Sends a JSON-RPC request to the specified URL in order to invoke a method.
     * Optionally, parameters can be specified (in JSON format) that will be used during the invocation of the method.
     * The ID argument can be used to correlate requests and responses.
     *
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent
     *                         successfully.
     * @param URL JSON-RPC URL the request is sent to.
     * @param ID The identifier to correlate the context between two objects.
     * @param Method The name of the method to be invoked.
     * @param Params [Optional] Parameters (in JSON format) to be used during the invocation of the method.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Send JSON-RPC Request [Callback]",
        Category="3Studio|JSON-RPC")
    static void K2_SendJsonRpcRequestCallback(
        FTSBC_K2_JsonRpcResponseCallback_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ID,
        const FString& Method,
        const FString& Params);
};