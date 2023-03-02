// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/Async/TSBC_EthGetTransactionByHashAsyncTask.h"

UTSBC_EthGetTransactionByHashAsyncTask* UTSBC_EthGetTransactionByHashAsyncTask::K2_EthGetTransactionByHashAsync(
    const FString& URL,
    const FString& ID,
    const FString& Hash)
{
    UTSBC_EthGetTransactionByHashAsyncTask* AsyncTask = NewObject<UTSBC_EthGetTransactionByHashAsyncTask>();
    AsyncTask->_URL = URL;
    AsyncTask->_ID = ID;
    AsyncTask->_Hash = Hash;

    return AsyncTask;
}

void UTSBC_EthGetTransactionByHashAsyncTask::Activate()
{
    CTSBC_EthGetTransactionByHash::FTSBC_EthGetTransactionByHash_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [OnCompleted=OnCompleted](
        const bool bSuccess,
        const FTSBC_JsonRpcResponse& Response,
        const bool bTransactionFound,
        const FTSBC_EthTransactionInfo& TransactionInfo)
        {
            OnCompleted.Broadcast(bSuccess, Response, bTransactionFound, TransactionInfo);
        });

    CTSBC_EthGetTransactionByHash::EthGetTransactionByHash(
        InternalDelegate,
        _URL,
        _ID,
        _Hash);
}