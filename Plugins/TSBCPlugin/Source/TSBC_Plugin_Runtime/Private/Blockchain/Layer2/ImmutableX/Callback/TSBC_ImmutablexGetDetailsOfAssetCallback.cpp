// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Blockchain/Layer2/ImmutableX/Callback/TSBC_ImmutablexGetDetailsOfAssetCallback.h"

#include "Blockchain/Layer2/ImmutableX/TSBC_ImmutablexGetDetailsOfAsset.h"

void UTSBC_ImmutablexGetDetailsOfAssetCallback::K2_ImmutablexGetDetailsOfAssetsCallback(
    FTSBC_K2_ImmutablexGetListOfAssetsCallback_Delegate ResponseDelegate,
    const FString& URL,
    const FString& TokenAddress,
    const FString& TokenId,
    const bool bIncludeFees)
{
    CTSBC_ImmutablexGetDetailsOfAsset::FTSBC_ImmutablexGetDetailsOfAsset_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [ResponseDelegate](const bool bSuccess, const FTSBC_HttpResponse& Response)
        {
            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(bSuccess, Response);
        });

    CTSBC_ImmutablexGetDetailsOfAsset::ImmutablexGetDetailsOfAsset(
        InternalDelegate,
        URL,
        TokenAddress,
        TokenId,
        bIncludeFees);
}