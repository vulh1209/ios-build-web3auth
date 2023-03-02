// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/Callback/TSBC_EthCallCallback.h"

#include "JsonRpc/Eth/TSBC_EthCall.h"

void UTSBC_EthCallCallback::K2_EthCallCallback(
    FTSBC_K2_EthCallCallback_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ID,
    const FString& FromAddress,
    const FString& ToAddress,
    const FString& Data,
    const ETSBC_EthBlockIdentifier BlockIdentifier)
{
    CTSBC_EthCall::FTSBC_EthCall_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [ResponseDelegate](
        const bool bSuccess,
        const FTSBC_JsonRpcResponse& Response,
        const FString& ResponseData)
        {
            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(bSuccess, Response, ResponseData);
        });

    CTSBC_EthCall::EthCall(
        InternalDelegate,
        URL,
        ID,
        FromAddress,
        ToAddress,
        Data,
        BlockIdentifier);
}