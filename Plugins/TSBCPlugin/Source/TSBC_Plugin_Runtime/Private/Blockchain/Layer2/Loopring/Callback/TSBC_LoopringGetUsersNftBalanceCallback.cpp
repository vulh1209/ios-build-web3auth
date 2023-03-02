// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Blockchain/Layer2/Loopring/Callback/TSBC_LoopringGetUsersNftBalanceCallback.h"

#include "Blockchain/Layer2/Loopring/TSBC_LoopringGetUsersNftBalance.h"

void UTSBC_LoopringGetUsersNftBalanceCallback::K2_LoopringGetUsersNftBalanceCallback(
    FTSBC_K2_LoopringGetUsersNftBalanceCallback_Delegate ResponseDelegate,
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
    CTSBC_LoopringGetUsersNftBalance::FTSBC_LoopringGetUsersNftBalance_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [ResponseDelegate](const bool bSuccess, const FTSBC_HttpResponse& Response)
        {
            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(bSuccess, Response);
        });

    CTSBC_LoopringGetUsersNftBalance::LoopringGetUsersNftBalance(
        InternalDelegate,
        URL,
        ApiKey,
        AccountId,
        NftDatas,
        TokenAddrs,
        TokenIds,
        Offset,
        Limit,
        NonZero);
}