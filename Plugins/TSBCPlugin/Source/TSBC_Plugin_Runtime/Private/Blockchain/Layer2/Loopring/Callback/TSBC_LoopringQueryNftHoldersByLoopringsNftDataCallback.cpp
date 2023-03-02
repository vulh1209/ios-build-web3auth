// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Blockchain/Layer2/Loopring/Callback/TSBC_LoopringQueryNftHoldersByLoopringsNftDataCallback.h"

#include "Blockchain/Layer2/Loopring/TSBC_LoopringQueryNftHoldersByLoopringsNftData.h"

void UTSBC_LoopringQueryNftHoldersByLoopringsNftDataCallback::K2_LoopringQueryNftHoldersByLoopringsNftDataCallback(
    FTSBC_K2_LoopringQueryNftHoldersByLoopringsNftDataCallback_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ApiKey,
    const FString& NftData,
    const FString& Offset,
    const FString& Limit)
{
    CTSBC_LoopringQueryNftHoldersByLoopringsNftData::FTSBC_LoopringQueryNftHoldersByLoopringsNftData_Delegate
        InternalDelegate;
    InternalDelegate.BindLambda(
        [ResponseDelegate](const bool bSuccess, const FTSBC_HttpResponse& Response)
        {
            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(bSuccess, Response);
        });

    CTSBC_LoopringQueryNftHoldersByLoopringsNftData::LoopringQueryNftHoldersByLoopringsNftData(
        InternalDelegate,
        URL,
        ApiKey,
        NftData,
        Offset,
        Limit);
}