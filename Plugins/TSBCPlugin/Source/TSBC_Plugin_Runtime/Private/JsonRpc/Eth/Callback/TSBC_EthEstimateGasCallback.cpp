// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/Callback/TSBC_EthEstimateGasCallback.h"

#include "JsonRpc/Eth/TSBC_EthEstimateGas.h"

void UTSBC_EthEstimateGasCallback::K2_EthEstimateGasCallback(
    FTSBC_K2_EthEstimateGasCallback_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ID,
    const FString& FromAddress,
    const FString& ToAddress,
    const FString& Data)
{
    CTSBC_EthEstimateGas::FTSBC_EthEstimateGas_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [ResponseDelegate](
        const bool bSuccess,
        const FTSBC_JsonRpcResponse& Response,
        const FTSBC_uint256& EstimatedGas)
        {
            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(bSuccess, Response, EstimatedGas);
        });

    CTSBC_EthEstimateGas::EthEstimateGas(
        InternalDelegate,
        URL,
        ID,
        FromAddress,
        ToAddress,
        Data);
}