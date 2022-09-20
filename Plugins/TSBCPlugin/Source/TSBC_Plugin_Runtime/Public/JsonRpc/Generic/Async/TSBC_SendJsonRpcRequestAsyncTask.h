// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Data/TSBC_Types.h"

#include "TSBC_SendJsonRpcRequestAsyncTask.generated.h"

/**
 * Sends a JSON-RPC request using an AsyncTask.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_SendJsonRpcRequestAsyncTask : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
        FTSBC_K2_JsonRpcResponseAsyncTask_Delegate,
        const FTSBC_JsonRpcResponse&,
        Response);

public:
    UPROPERTY(BlueprintAssignable, Category="3Studio|JSON-RPC")
    FTSBC_K2_JsonRpcResponseAsyncTask_Delegate OnCompleted;

private:
    UPROPERTY()
    FString _URL;

    UPROPERTY()
    FString _ID;

    UPROPERTY()
    FString _Method;

    UPROPERTY()
    FString _Params;

public:
    /**
     * Sends a JSON-RPC request to the specified URL in order to invoke a method.
     * Optionally, parameters can be specified (in JSON format) that will be used during the invocation of the method.
     * The ID argument can be used to correlate requests and responses.
     *
     * @param URL JSON-RPC URL the request is sent to.
     * @param Method The name of the method to be invoked.
     * @param Params [Optional] Parameters (in JSON format) to be used during the invocation of the method.
     * @param ID The identifier to correlate the context between two objects.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Send JSON-RPC Request [Async]",
        Category="3Studio|JSON-RPC",
        Meta=(BlueprintInternalUseOnly="true"))
    static UTSBC_SendJsonRpcRequestAsyncTask* K2_SendJsonRpcRequestAsync(
        const FString& URL,
        const FString& ID,
        const FString& Method,
        const FString& Params);

    virtual void Activate() override;
};