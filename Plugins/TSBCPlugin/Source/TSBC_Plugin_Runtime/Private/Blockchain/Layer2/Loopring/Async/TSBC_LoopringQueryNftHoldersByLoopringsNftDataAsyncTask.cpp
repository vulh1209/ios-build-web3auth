// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Blockchain/Layer2/Loopring/Async/TSBC_LoopringQueryNftHoldersByLoopringsNftDataAsyncTask.h"

#include "Blockchain/Layer2/Loopring/TSBC_LoopringQueryNftHoldersByLoopringsNftData.h"

UTSBC_LoopringQueryNftHoldersByLoopringsNftDataAsyncTask*
UTSBC_LoopringQueryNftHoldersByLoopringsNftDataAsyncTask::K2_LoopringQueryNftHoldersByLoopringsNftDataAsync(
    const FString& URL,
    const FString& ApiKey,
    const FString& NftData,
    const FString& Offset,
    const FString& Limit)
{
    UTSBC_LoopringQueryNftHoldersByLoopringsNftDataAsyncTask* AsyncTask =
        NewObject<UTSBC_LoopringQueryNftHoldersByLoopringsNftDataAsyncTask>();
    AsyncTask->_URL = URL;
    AsyncTask->_ApiKey = ApiKey;
    AsyncTask->_NftData = NftData;
    AsyncTask->_Offset = Offset;
    AsyncTask->_Limit = Limit;

    return AsyncTask;
}

void UTSBC_LoopringQueryNftHoldersByLoopringsNftDataAsyncTask::Activate()
{
    CTSBC_LoopringQueryNftHoldersByLoopringsNftData::FTSBC_LoopringQueryNftHoldersByLoopringsNftData_Delegate
        InternalDelegate;
    InternalDelegate.BindLambda(
        [OnCompleted=OnCompleted](const bool bSuccess, const FTSBC_HttpResponse& Response)
        {
            OnCompleted.Broadcast(bSuccess, Response);
        });

    CTSBC_LoopringQueryNftHoldersByLoopringsNftData::LoopringQueryNftHoldersByLoopringsNftData(
        InternalDelegate,
        _URL,
        _ApiKey,
        _NftData,
        _Offset,
        _Limit);
}