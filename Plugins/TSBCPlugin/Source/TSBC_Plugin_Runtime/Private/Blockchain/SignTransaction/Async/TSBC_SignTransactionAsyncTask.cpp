// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Blockchain/SignTransaction/Async/TSBC_SignTransactionAsyncTask.h"

#include "Blockchain/SignTransaction/TSBC_SignTransaction.h"

UTSBC_SignTransactionAsyncTask* UTSBC_SignTransactionAsyncTask::K2_SignTransactionAsync(
    const TArray<uint8>& PrivateKey,
    const int32 Nonce,
    const FTSBC_uint256& GasPrice,
    const FTSBC_uint256& GasLimit,
    const FString& ToAddress,
    const FTSBC_uint256& Value,
    const FString& Data,
    const int32 ChainId)
{
    UTSBC_SignTransactionAsyncTask* AsyncTask = NewObject<UTSBC_SignTransactionAsyncTask>();

    AsyncTask->_PrivateKey = PrivateKey;
    AsyncTask->_Nonce = Nonce;
    AsyncTask->_GasPrice = GasPrice;
    AsyncTask->_GasLimit = GasLimit;
    AsyncTask->_ToAddress = ToAddress;
    AsyncTask->_Value = Value;
    AsyncTask->_Data = Data;
    AsyncTask->_ChainId = ChainId;

    return AsyncTask;
}

void UTSBC_SignTransactionAsyncTask::Activate()
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

    CTSBC_SignTransaction::SignTransactionAsync(
        InternalDelegate,
        _PrivateKey,
        _Nonce,
        _GasPrice,
        _GasLimit,
        _ToAddress,
        _Value,
        _Data,
        _ChainId);
}