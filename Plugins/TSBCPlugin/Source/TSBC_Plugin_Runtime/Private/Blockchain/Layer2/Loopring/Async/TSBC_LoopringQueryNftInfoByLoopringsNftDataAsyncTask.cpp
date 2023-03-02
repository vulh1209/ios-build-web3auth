// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Blockchain/Layer2/Loopring/Async/TSBC_LoopringQueryNftInfoByLoopringsNftDataAsyncTask.h"

#include "Blockchain/Layer2/Loopring/TSBC_LoopringQueryNftInfoByLoopringsNftData.h"

UTSBC_LoopringQueryNftInfoByLoopringsNftDataAsyncTask*
UTSBC_LoopringQueryNftInfoByLoopringsNftDataAsyncTask::K2_LoopringQueryNftHoldersByLoopringsNftDataAsync(
    const FString& URL,
    const FString& ApiKey,
    const FString& NftData)
{
    UTSBC_LoopringQueryNftInfoByLoopringsNftDataAsyncTask* AsyncTask =
        NewObject<UTSBC_LoopringQueryNftInfoByLoopringsNftDataAsyncTask>();
    AsyncTask->_URL = URL;
    AsyncTask->_ApiKey = ApiKey;
    AsyncTask->_NftData = NftData;

    return AsyncTask;
}

void UTSBC_LoopringQueryNftInfoByLoopringsNftDataAsyncTask::Activate()
{
    CTSBC_LoopringQueryNftInfoByLoopringsNftData::FTSBC_LoopringQueryNftInfoByLoopringsNftData_Delegate
        InternalDelegate;
    InternalDelegate.BindLambda(
        [OnCompleted=OnCompleted](const bool bSuccess, const FTSBC_HttpResponse& Response)
        {
            OnCompleted.Broadcast(bSuccess, Response);
        });

    CTSBC_LoopringQueryNftInfoByLoopringsNftData::LoopringQueryNftInfoByLoopringsNftData(
        InternalDelegate,
        _URL,
        _ApiKey,
        _NftData);
}