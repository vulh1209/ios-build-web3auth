// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Blockchain/Layer2/ImmutableX/Async/TSBC_ImmutablexGetListOfAssetsAsyncTask.h"

#include "Blockchain/Layer2/ImmutableX/TSBC_ImmutablexGetListOfAssets.h"

UTSBC_ImmutablexGetListOfAssetsAsyncTask*
UTSBC_ImmutablexGetListOfAssetsAsyncTask::K2_ImmutablexGetListOfAssetsAsync(
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
    UTSBC_ImmutablexGetListOfAssetsAsyncTask* AsyncTask = NewObject<UTSBC_ImmutablexGetListOfAssetsAsyncTask>();
    AsyncTask->_URL = URL;
    AsyncTask->_PageSize = PageSize;
    AsyncTask->_Cursor = Cursor;
    AsyncTask->_OrderBy = OrderBy;
    AsyncTask->_Direction = Direction;
    AsyncTask->_User = User;
    AsyncTask->_Status = Status;
    AsyncTask->_Name = Name;
    AsyncTask->_Metadata = Metadata;
    AsyncTask->_SellOrders = SellOrders;
    AsyncTask->_BuyOrders = BuyOrders;
    AsyncTask->_bIncludeFees = bIncludeFees;
    AsyncTask->_Collection = Collection;
    AsyncTask->_UpdatedMinTimestamp = UpdatedMinTimestamp;
    AsyncTask->_UpdatedMaxTimestamp = UpdatedMaxTimestamp;
    AsyncTask->_AuxillaryFeePercentages = AuxillaryFeePercentages;
    AsyncTask->_AuxillaryFeeRecipients = AuxillaryFeeRecipients;

    return AsyncTask;
}

void UTSBC_ImmutablexGetListOfAssetsAsyncTask::Activate()
{
    CTSBC_ImmutablexGetListOfAssets::FTSBC_ImmutablexGetListOfAssets_Delegate InternalDelegate;
    InternalDelegate.BindLambda(
        [OnCompleted=OnCompleted](const bool bSuccess, const FTSBC_HttpResponse& Response)
        {
            OnCompleted.Broadcast(bSuccess, Response);
        });

    CTSBC_ImmutablexGetListOfAssets::ImmutablexGetListOfAssets(
        InternalDelegate,
        _URL,
        _PageSize,
        _Cursor,
        _OrderBy,
        _Direction,
        _User,
        _Status,
        _Name,
        _Metadata,
        _SellOrders,
        _BuyOrders,
        _bIncludeFees,
        _Collection,
        _UpdatedMinTimestamp,
        _UpdatedMaxTimestamp,
        _AuxillaryFeePercentages,
        _AuxillaryFeeRecipients);
}