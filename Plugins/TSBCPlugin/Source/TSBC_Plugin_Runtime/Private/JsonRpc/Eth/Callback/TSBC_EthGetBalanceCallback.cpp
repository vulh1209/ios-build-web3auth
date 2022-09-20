// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/Callback/TSBC_EthGetBalanceCallback.h"

#include "JsonRpc/Eth/TSBC_EthGetBalance.h"

void UTSBC_EthGetBalanceCallback::K2_EthGetBalanceCallback(
    FTSBC_K2_EthGetBalanceCallback_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ID,
    const FString AccountAddress,
    const ETSBC_EthBlockIdentifier BlockIdentifier)
{
    CTSBC_EthGetBalance::FTSBC_EthGetBalance_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [ResponseDelegate](
        const bool bSuccess,
        const FTSBC_JsonRpcResponse& Response,
        const FTSBC_uint256& Balance)
        {
            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(bSuccess, Response, Balance);
        });

    CTSBC_EthGetBalance::EthGetBalance(
        InternalDelegate,
        URL,
        ID,
        AccountAddress,
        BlockIdentifier);
}