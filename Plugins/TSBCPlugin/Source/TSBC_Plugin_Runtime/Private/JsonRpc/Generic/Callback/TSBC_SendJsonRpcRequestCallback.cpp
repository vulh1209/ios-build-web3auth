// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Generic/Callback/TSBC_SendJsonRpcRequestCallback.h"

#include "JsonRpc/Generic/TSBC_SendJsonRpcRequest.h"

void UTSBC_SendJsonRpcRequestCallback::K2_SendJsonRpcRequestCallback(
    FTSBC_K2_JsonRpcResponseCallback_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ID,
    const FString& Method,
    const FString& Params)
{
    CTSBC_SendJsonRpcRequest::FTSBC_JsonRpcResponse_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [ResponseDelegate](const FTSBC_JsonRpcResponse& Response)
        {
            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(Response);
        });

    CTSBC_SendJsonRpcRequest::SendJsonRpcRequest(InternalDelegate, URL, ID, Method, Params);
}