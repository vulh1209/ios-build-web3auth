// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Generic/Async/TSBC_SendJsonRpcRequestAsyncTask.h"

#include "JsonRpc/Generic/TSBC_SendJsonRpcRequest.h"

UTSBC_SendJsonRpcRequestAsyncTask* UTSBC_SendJsonRpcRequestAsyncTask::K2_SendJsonRpcRequestAsync(
    const FString& URL,
    const FString& ID,
    const FString& Method,
    const FString& Params)
{
    UTSBC_SendJsonRpcRequestAsyncTask* AsyncTask = NewObject<UTSBC_SendJsonRpcRequestAsyncTask>();
    AsyncTask->_URL = URL;
    AsyncTask->_ID = ID;
    AsyncTask->_Method = Method;
    AsyncTask->_Params = Params;

    return AsyncTask;
}

void UTSBC_SendJsonRpcRequestAsyncTask::Activate()
{
    CTSBC_SendJsonRpcRequest::FTSBC_JsonRpcResponse_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [OnCompleted=OnCompleted](const FTSBC_JsonRpcResponse& Response)
        {
            OnCompleted.Broadcast(Response);
        });

    CTSBC_SendJsonRpcRequest::SendJsonRpcRequest(
        InternalDelegate,
        _URL,
        _ID,
        _Method,
        _Params);
}