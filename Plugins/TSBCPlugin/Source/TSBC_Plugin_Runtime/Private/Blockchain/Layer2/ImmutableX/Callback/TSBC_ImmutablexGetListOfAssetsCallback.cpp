// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Blockchain/Layer2/ImmutableX/Callback/TSBC_ImmutablexGetListOfAssetsCallback.h"

#include "Blockchain/Layer2/ImmutableX/TSBC_ImmutablexGetListOfAssets.h"

void UTSBC_ImmutablexGetListOfAssetsCallback::K2_ImmutablexGetListOfAssetsCallback(
    FTSBC_K2_ImmutablexGetListOfAssetsCallback_Delegate ResponseDelegate,
    const FString& URL,
    const FString& PageSize,
    const FString& Cursor,
    const FString& OrderBy,
    const FString& Direction,
    const FString& User,
    const FString& Status,
    const FString& Name,
    const FString& Metadata,
    const FString& SellOrders,
    const FString& BuyOrders,
    const bool bIncludeFees,
    const FString& Collection,
    const FString& UpdatedMinTimestamp,
    const FString& UpdatedMaxTimestamp,
    const FString& AuxillaryFeePercentages,
    const FString& AuxillaryFeeRecipients)
{
    CTSBC_ImmutablexGetListOfAssets::FTSBC_ImmutablexGetListOfAssets_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [ResponseDelegate](const bool bSuccess, const FTSBC_HttpResponse& Response)
        {
            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(bSuccess, Response);
        });

    CTSBC_ImmutablexGetListOfAssets::ImmutablexGetListOfAssets(
        InternalDelegate,
        URL,
        PageSize,
        Cursor,
        OrderBy,
        Direction,
        User,
        Status,
        Name,
        Metadata,
        SellOrders,
        BuyOrders,
        bIncludeFees,
        Collection,
        UpdatedMinTimestamp,
        UpdatedMaxTimestamp,
        AuxillaryFeePercentages,
        AuxillaryFeeRecipients);
}