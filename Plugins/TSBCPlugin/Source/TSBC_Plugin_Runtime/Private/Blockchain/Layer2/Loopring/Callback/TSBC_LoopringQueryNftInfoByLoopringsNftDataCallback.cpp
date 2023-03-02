// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Blockchain/Layer2/Loopring/Callback/TSBC_LoopringQueryNftInfoByLoopringsNftDataCallback.h"

#include "Blockchain/Layer2/Loopring/TSBC_LoopringQueryNftInfoByLoopringsNftData.h"

void UTSBC_LoopringQueryNftInfoByLoopringsNftDataCallback::K2_TSBC_LoopringQueryNftInfoByLoopringsNftDataCallback(
    FTSBC_K2_LoopringQueryNftInfoByLoopringsNftDataCallback_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ApiKey,
    const FString& NftDatas)
{
    CTSBC_LoopringQueryNftInfoByLoopringsNftData::FTSBC_LoopringQueryNftInfoByLoopringsNftData_Delegate
        InternalDelegate;
    InternalDelegate.BindLambda(
        [ResponseDelegate](const bool bSuccess, const FTSBC_HttpResponse& Response)
        {
            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(bSuccess, Response);
        });

    CTSBC_LoopringQueryNftInfoByLoopringsNftData::LoopringQueryNftInfoByLoopringsNftData(
        InternalDelegate,
        URL,
        ApiKey,
        NftDatas);
}