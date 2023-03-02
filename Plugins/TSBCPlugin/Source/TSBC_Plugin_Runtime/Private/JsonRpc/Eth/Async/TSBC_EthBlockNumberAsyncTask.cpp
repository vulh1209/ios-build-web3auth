// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/Async/TSBC_EthBlockNumberAsyncTask.h"

#include "JsonRpc/Eth/TSBC_EthBlockNumber.h"

UTSBC_EthBlockNumberAsyncTask* UTSBC_EthBlockNumberAsyncTask::K2_EthBlockNumberAsync(
    const FString& URL,
    const FString& ID)
{
    UTSBC_EthBlockNumberAsyncTask* AsyncTask = NewObject<UTSBC_EthBlockNumberAsyncTask>();
    AsyncTask->_URL = URL;
    AsyncTask->_ID = ID;

    return AsyncTask;
}

void UTSBC_EthBlockNumberAsyncTask::Activate()
{
    CTSBC_EthBlockNumber::FTSBC_EthBlockNumber_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [OnCompleted=OnCompleted](
        const bool bSuccess,
        const FTSBC_JsonRpcResponse& Response,
        const int64 BlockNumber)
        {
            OnCompleted.Broadcast(bSuccess, Response, BlockNumber);
        }
    );

    CTSBC_EthBlockNumber::EthBlockNumber(
        InternalDelegate,
        _URL,
        _ID);
}