// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/Callback/TSBC_EthGasPriceCallback.h"

#include "JsonRpc/Eth/TSBC_EthGasPrice.h"

void UTSBC_EthGasPriceCallback::K2_EthGasPriceCallback(
    FTSBC_K2_EthGasPriceCallback_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ID)
{
    CTSBC_EthGasPrice::FTSBC_EthGasPrice_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [ResponseDelegate](
        const bool bSuccess,
        const FTSBC_JsonRpcResponse& Response,
        const FTSBC_uint256& GasPrice)
        {
            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(bSuccess, Response, GasPrice);
        });

    CTSBC_EthGasPrice::EthGasPrice(
        InternalDelegate,
        URL,
        ID);
}