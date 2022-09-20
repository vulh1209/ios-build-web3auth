// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/Async/TSBC_EthGetTransactionCountAsyncTask.h"

#include "JsonRpc/Eth/TSBC_EthGetTransactionCount.h"

UTSBC_EthGetTransactionCountAsyncTask* UTSBC_EthGetTransactionCountAsyncTask::K2_EthGetTransactionCountAsync(
    const FString& URL,
    const FString& ID,
    const FString& Address,
    const ETSBC_EthBlockIdentifier BlockIdentifier)
{
    UTSBC_EthGetTransactionCountAsyncTask* AsyncTask = NewObject<UTSBC_EthGetTransactionCountAsyncTask>();
    AsyncTask->_URL = URL;
    AsyncTask->_ID = ID;
    AsyncTask->_Address = Address;
    AsyncTask->_BlockIdentifier = BlockIdentifier;

    return AsyncTask;
}

void UTSBC_EthGetTransactionCountAsyncTask::Activate()
{
    CTSBC_EthGetTransactionCount::FTSBC_EthGetTransactionCount_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [OnCompleted=OnCompleted](
        const bool bSuccess,
        const FTSBC_JsonRpcResponse& Response,
        const int32& TransactionCount)
        {
            OnCompleted.Broadcast(bSuccess, Response, TransactionCount);
        });

    CTSBC_EthGetTransactionCount::EthGetTransactionCount(
        InternalDelegate,
        _URL,
        _ID,
        _Address,
        _BlockIdentifier);
}