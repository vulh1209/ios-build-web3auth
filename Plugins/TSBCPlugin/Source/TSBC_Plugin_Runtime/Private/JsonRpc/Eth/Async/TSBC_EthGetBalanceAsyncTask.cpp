// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/Async/TSBC_EthGetBalanceAsyncTask.h"

#include "JsonRpc/Eth/TSBC_EthGetBalance.h"

UTSBC_EthGetBalanceAsyncTask* UTSBC_EthGetBalanceAsyncTask::K2_EthGetBalanceAsync(
    const FString& URL,
    const FString& ID,
    const FString& AccountAddress,
    const ETSBC_EthBlockIdentifier BlockIdentifier)
{
    UTSBC_EthGetBalanceAsyncTask* AsyncTask = NewObject<UTSBC_EthGetBalanceAsyncTask>();
    AsyncTask->_URL = URL;
    AsyncTask->_ID = ID;
    AsyncTask->_AccountAddress = AccountAddress;
    AsyncTask->_BlockIdentifier = BlockIdentifier;

    return AsyncTask;
}

void UTSBC_EthGetBalanceAsyncTask::Activate()
{
    CTSBC_EthGetBalance::FTSBC_EthGetBalance_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [OnCompleted=OnCompleted](
        const bool bSuccess,
        const FTSBC_JsonRpcResponse& Response,
        const FTSBC_uint256& Balance)
        {
            OnCompleted.Broadcast(bSuccess, Response, Balance);
        });

    CTSBC_EthGetBalance::EthGetBalance(
        InternalDelegate,
        _URL,
        _ID,
        _AccountAddress,
        _BlockIdentifier);
}