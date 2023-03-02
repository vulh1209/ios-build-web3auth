// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/Callback/TSBC_EthBlockNumberCallback.h"

#include "JsonRpc/Eth/TSBC_EthBlockNumber.h"

void UTSBC_EthBlockNumberCallback::K2_EthBlockNumberCallback(
    FTSBC_K2_EthBlockNumberCallback_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ID)
{
    CTSBC_EthBlockNumber::FTSBC_EthBlockNumber_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [ResponseDelegate](
        const bool bSuccess,
        const FTSBC_JsonRpcResponse& Response,
        const int64 BlockNumber)
        {
            if(ResponseDelegate.IsBound())
            {
                ResponseDelegate.Execute(bSuccess, Response, BlockNumber);
            }
        });

    CTSBC_EthBlockNumber::EthBlockNumber(
        InternalDelegate,
        URL,
        ID);
}