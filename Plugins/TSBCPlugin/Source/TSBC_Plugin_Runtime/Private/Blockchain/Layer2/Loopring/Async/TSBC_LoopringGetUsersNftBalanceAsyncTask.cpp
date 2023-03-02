// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Blockchain/Layer2/Loopring/Async/TSBC_LoopringGetUsersNftBalanceAsyncTask.h"

#include "Blockchain/Layer2/Loopring/TSBC_LoopringGetUsersNftBalance.h"

UTSBC_LoopringGetUsersNftBalanceAsyncTask*
UTSBC_LoopringGetUsersNftBalanceAsyncTask::K2_LoopringGetUsersNftBalanceAsync(
    const FString& URL,
    const FString& ApiKey,
    const FString& AccountId,
    const FString& NftDatas,
    const FString& TokenAddrs,
    const FString& TokenIds,
    const FString& Offset,
    const FString& Limit,
    const FString& NonZero)
{
    UTSBC_LoopringGetUsersNftBalanceAsyncTask* AsyncTask = NewObject<UTSBC_LoopringGetUsersNftBalanceAsyncTask>();
    AsyncTask->_URL = URL;
    AsyncTask->_ApiKey = ApiKey;
    AsyncTask->_AccountId = AccountId;
    AsyncTask->_NftDatas = NftDatas;
    AsyncTask->_TokenAddrs = TokenAddrs;
    AsyncTask->_TokenIds = TokenIds;
    AsyncTask->_Offset = Offset;
    AsyncTask->_Limit = Limit;
    AsyncTask->_NonZero = NonZero;

    return AsyncTask;
}

void UTSBC_LoopringGetUsersNftBalanceAsyncTask::Activate()
{
    CTSBC_LoopringGetUsersNftBalance::FTSBC_LoopringGetUsersNftBalance_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [OnCompleted=OnCompleted](const bool bSuccess, const FTSBC_HttpResponse& Response)
        {
            OnCompleted.Broadcast(bSuccess, Response);
        });

    CTSBC_LoopringGetUsersNftBalance::LoopringGetUsersNftBalance(
        InternalDelegate,
        _URL,
        _ApiKey,
        _AccountId,
        _NftDatas,
        _TokenAddrs,
        _TokenIds,
        _Offset,
        _Limit,
        _NonZero);
}