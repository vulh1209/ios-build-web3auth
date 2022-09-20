// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/Async/TSBC_EthCallAsyncTask.h"

#include "JsonRpc/Eth/TSBC_EthCall.h"

UTSBC_EthCallAsyncTask* UTSBC_EthCallAsyncTask::K2_EthCallAsync(
    const FString& URL,
    const FString& ID,
    const FString& FromAddress,
    const FString& ToAddress,
    const FString& Data,
    const ETSBC_EthBlockIdentifier BlockIdentifier)
{
    UTSBC_EthCallAsyncTask* AsyncTask = NewObject<UTSBC_EthCallAsyncTask>();
    AsyncTask->_URL = URL;
    AsyncTask->_ID = ID;
    AsyncTask->_FromAddress = FromAddress;
    AsyncTask->_ToAddress = ToAddress;
    AsyncTask->_Data = Data;
    AsyncTask->_BlockIdentifier = BlockIdentifier;

    return AsyncTask;
}

void UTSBC_EthCallAsyncTask::Activate()
{
    CTSBC_EthCall::FTSBC_EthCall_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [OnCompleted=OnCompleted](
        const bool bSuccess,
        const FTSBC_JsonRpcResponse& Response,
        const FString& ResponseData)
        {
            OnCompleted.Broadcast(bSuccess, Response, ResponseData);
        });

    CTSBC_EthCall::EthCall(
        InternalDelegate,
        _URL,
        _ID,
        _FromAddress,
        _ToAddress,
        _Data,
        _BlockIdentifier);
}