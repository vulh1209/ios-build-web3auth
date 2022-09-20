// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/Async/TSBC_EthSendRawTransactionAsyncTask.h"

#include "JsonRpc/Eth/TSBC_EthSendRawTransaction.h"

UTSBC_EthSendRawTransactionAsyncTask* UTSBC_EthSendRawTransactionAsyncTask::K2_EthSendRawTransactionAsync(
    const FString& URL,
    const FString& ID,
    const FString& SignedTransactionData)
{
    UTSBC_EthSendRawTransactionAsyncTask* AsyncTask = NewObject<UTSBC_EthSendRawTransactionAsyncTask>();
    AsyncTask->_URL = URL;
    AsyncTask->_ID = ID;
    AsyncTask->_SignedTransactionData = SignedTransactionData;

    return AsyncTask;
}

void UTSBC_EthSendRawTransactionAsyncTask::Activate()
{
    CTSBC_EthSendRawTransaction::FTSBC_EthSendRawTransaction_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [OnCompleted=OnCompleted](
        const bool bSuccess,
        const FTSBC_JsonRpcResponse& Response,
        const FString& TransactionHash)
        {
            OnCompleted.Broadcast(bSuccess, Response, TransactionHash);
        });

    CTSBC_EthSendRawTransaction::EthSendRawTransaction(
        InternalDelegate,
        _URL,
        _ID,
        _SignedTransactionData);
}