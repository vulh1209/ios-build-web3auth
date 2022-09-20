// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/Async/TSBC_EthEstimateGasAsyncTask.h"

#include "JsonRpc/Eth/TSBC_EthEstimateGas.h"

UTSBC_EthEstimateGasAsyncTask* UTSBC_EthEstimateGasAsyncTask::K2_EthEstimateGasAsync(
    const FString& URL,
    const FString& ID,
    const FString& FromAddress,
    const FString& ToAddress,
    const FString& Data)
{
    UTSBC_EthEstimateGasAsyncTask* AsyncTask = NewObject<UTSBC_EthEstimateGasAsyncTask>();
    AsyncTask->_URL = URL;
    AsyncTask->_ID = ID;
    AsyncTask->_FromAddress = FromAddress;
    AsyncTask->_ToAddress = ToAddress;
    AsyncTask->_Data = Data;

    return AsyncTask;
}

void UTSBC_EthEstimateGasAsyncTask::Activate()
{
    CTSBC_EthEstimateGas::FTSBC_EthEstimateGas_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [OnCompleted=OnCompleted](
        const bool bSuccess,
        const FTSBC_JsonRpcResponse& Response,
        const FTSBC_uint256& EstimatedGas)
        {
            OnCompleted.Broadcast(bSuccess, Response, EstimatedGas);
        });

    CTSBC_EthEstimateGas::EthEstimateGas(
        InternalDelegate,
        _URL,
        _ID,
        _FromAddress,
        _ToAddress,
        _Data);
}