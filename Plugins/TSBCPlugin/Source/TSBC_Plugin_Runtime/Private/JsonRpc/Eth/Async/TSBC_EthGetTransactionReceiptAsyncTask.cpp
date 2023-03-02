// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/Async/TSBC_EthGetTransactionReceiptAsyncTask.h"

UTSBC_EthGetTransactionReceiptAsyncTask* UTSBC_EthGetTransactionReceiptAsyncTask::K2_EthGetTransactionReceiptAsync(
    const FString& URL,
    const FString& ID,
    const FString& Hash)
{
    UTSBC_EthGetTransactionReceiptAsyncTask* AsyncTask = NewObject<UTSBC_EthGetTransactionReceiptAsyncTask>();
    AsyncTask->_URL = URL;
    AsyncTask->_ID = ID;
    AsyncTask->_Hash = Hash;

    return AsyncTask;
}

void UTSBC_EthGetTransactionReceiptAsyncTask::Activate()
{
    CTSBC_EthGetTransactionReceipt::FTSBC_EthGetTransactionReceipt_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [OnCompleted=OnCompleted](
        const bool bSuccess,
        const FTSBC_JsonRpcResponse& Response,
        const bool bReceiptFound,
        const FTSBC_EthTransactionReceipt& TransactionReceipt)
        {
            OnCompleted.Broadcast(bSuccess, Response, bReceiptFound, TransactionReceipt);
        });

    CTSBC_EthGetTransactionReceipt::EthGetTransactionReceipt(
        InternalDelegate,
        _URL,
        _ID,
        _Hash);
}