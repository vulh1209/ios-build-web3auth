// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/Callback/TSBC_EthGetTransactionCountCallback.h"

#include "JsonRpc/Eth/TSBC_EthGetTransactionCount.h"

void UTSBC_EthGetTransactionCountCallback::K2_EthGetTransactionCount(
    FTSBC_K2_EthGetTransactionCountCallback_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ID,
    const FString& Address,
    const ETSBC_EthBlockIdentifier BlockIdentifier)
{
    CTSBC_EthGetTransactionCount::FTSBC_EthGetTransactionCount_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [ResponseDelegate](
        const bool bSuccess,
        const FTSBC_JsonRpcResponse& Response,
        const int32& TransactionCount)
        {
            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(bSuccess, Response, TransactionCount);
        });

    CTSBC_EthGetTransactionCount::EthGetTransactionCount(
        InternalDelegate,
        URL,
        ID,
        Address,
        BlockIdentifier);
}