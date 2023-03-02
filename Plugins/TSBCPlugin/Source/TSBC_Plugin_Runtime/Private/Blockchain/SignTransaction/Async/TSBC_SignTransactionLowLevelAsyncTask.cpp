// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Blockchain/SignTransaction/Async/TSBC_SignTransactionLowLevelAsyncTask.h"

#include "Blockchain/SignTransaction/TSBC_SignTransaction.h"

UTSBC_SignTransactionLowLevelAsyncTask* UTSBC_SignTransactionLowLevelAsyncTask::K2_SignTransactionLowLevelAsync(
    const FString& PrivateKey,
    const FTSBC_EthTransaction& Transaction)
{
    UTSBC_SignTransactionLowLevelAsyncTask* AsyncTask = NewObject<UTSBC_SignTransactionLowLevelAsyncTask>();

    AsyncTask->_PrivateKey = PrivateKey;
    AsyncTask->_Transaction = Transaction;

    return AsyncTask;
}

void UTSBC_SignTransactionLowLevelAsyncTask::Activate()
{
    CTSBC_SignTransaction::FTSBC_SignTransaction_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [OnCompleted=OnCompleted](
        const bool bSuccess,
        const FString& ErrorMessage,
        const FString& SignedTransaction,
        const FString& MessageHash,
        const FString& TransactionHash)
        {
            OnCompleted.Broadcast(
                bSuccess,
                ErrorMessage,
                SignedTransaction,
                MessageHash,
                TransactionHash);
        });

    CTSBC_SignTransaction::SignTransactionLowLevelAsync(
        InternalDelegate,
        _PrivateKey,
        _Transaction);
}