// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Blockchain/Layer2/ImmutableX/Async/TSBC_ImmutablexGetDetailsOfAssetAsyncTask.h"

#include "Blockchain/Layer2/ImmutableX/TSBC_ImmutablexGetDetailsOfAsset.h"

UTSBC_ImmutablexGetDetailsOfAssetAsyncTask*
UTSBC_ImmutablexGetDetailsOfAssetAsyncTask::K2_ImmutablexGetDetailsOfAssetsAsync(
    const FString& URL,
    const FString& TokenAddress,
    const FString& TokenId,
    const bool bIncludeFees)
{
    UTSBC_ImmutablexGetDetailsOfAssetAsyncTask* AsyncTask = NewObject<UTSBC_ImmutablexGetDetailsOfAssetAsyncTask>();
    AsyncTask->_URL = URL;
    AsyncTask->_TokenAddress = TokenAddress;
    AsyncTask->_TokenId = TokenId;
    AsyncTask->_bIncludeFees = bIncludeFees;

    return AsyncTask;
}

void UTSBC_ImmutablexGetDetailsOfAssetAsyncTask::Activate()
{
    CTSBC_ImmutablexGetDetailsOfAsset::FTSBC_ImmutablexGetDetailsOfAsset_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [OnCompleted=OnCompleted](const bool bSuccess, const FTSBC_HttpResponse& Response)
        {
            OnCompleted.Broadcast(bSuccess, Response);
        });

    CTSBC_ImmutablexGetDetailsOfAsset::ImmutablexGetDetailsOfAsset(
        InternalDelegate,
        _URL,
        _TokenAddress,
        _TokenId,
        _bIncludeFees);
}