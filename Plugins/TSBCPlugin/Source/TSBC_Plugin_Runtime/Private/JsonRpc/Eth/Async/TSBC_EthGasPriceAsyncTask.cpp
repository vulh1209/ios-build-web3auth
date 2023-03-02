// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/Async/TSBC_EthGasPriceAsyncTask.h"

#include "JsonRpc/Eth/TSBC_EthGasPrice.h"

UTSBC_EthGasPriceAsyncTask* UTSBC_EthGasPriceAsyncTask::K2_EthGasPriceAsync(
    const FString& URL,
    const FString& ID)
{
    UTSBC_EthGasPriceAsyncTask* AsyncTask = NewObject<UTSBC_EthGasPriceAsyncTask>();
    AsyncTask->_URL = URL;
    AsyncTask->_ID = ID;

    return AsyncTask;
}

void UTSBC_EthGasPriceAsyncTask::Activate()
{
    CTSBC_EthGasPrice::FTSBC_EthGasPrice_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [OnCompleted=OnCompleted](
        const bool bSuccess,
        const FTSBC_JsonRpcResponse& Response,
        const FTSBC_uint256& GasPrice)
        {
            OnCompleted.Broadcast(bSuccess, Response, GasPrice);
        });

    CTSBC_EthGasPrice::EthGasPrice(
        InternalDelegate,
        _URL,
        _ID);
}