// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Data/TSBC_Types.h"

#include "TSBC_ImmutablexGetListOfAssetsAsyncTask.generated.h"

/**
 * Send a ImmutableX REST-API call using an AsyncTask.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_ImmutablexGetListOfAssetsAsyncTask : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
        FTSBC_K2_ImmutablexGetListOfAssetsAsyncTask_Delegate,
        const bool,
        bSuccess,
        const FTSBC_HttpResponse&,
        Response);

public:
    UPROPERTY(BlueprintAssignable, Category="3Studio|HTTP")
    FTSBC_K2_ImmutablexGetListOfAssetsAsyncTask_Delegate OnCompleted;

private:
    UPROPERTY()
    FString _URL;

    UPROPERTY()
    FString _PageSize;

    UPROPERTY()
    FString _Cursor;

    UPROPERTY()
    FString _OrderBy;

    UPROPERTY()
    FString _Direction;

    UPROPERTY()
    FString _User;

    UPROPERTY()
    FString _Status;

    UPROPERTY()
    FString _Name;

    UPROPERTY()
    FString _Metadata;

    UPROPERTY()
    FString _SellOrders;

    UPROPERTY()
    FString _BuyOrders;

    UPROPERTY()
    bool _bIncludeFees;

    UPROPERTY()
    FString _Collection;

    UPROPERTY()
    FString _UpdatedMinTimestamp;

    UPROPERTY()
    FString _UpdatedMaxTimestamp;

    UPROPERTY()
    FString _AuxillaryFeePercentages;

    UPROPERTY()
    FString _AuxillaryFeeRecipients;

public:
    /**
     * Get a list of assets.
     * 
     * @param URL The URL to send the request to.
     * @param PageSize Page size of the result
     * @param Cursor Cursor
     * @param OrderBy Property to sort by
     * @param Direction Direction to sort (asc/desc)
     * @param User Ethereum address of the user who owns these assets
     * @param Status Status of these assets
     * @param Name Name of the asset to search
     * @param Metadata URL JSON-encoded metadata filters for these assets. Javascript example: encodeURI(JSON.stringify({'proto':['1147'],'quality':['Meteorite']}))
     * @param SellOrders Set flag to true to fetch an array of sell order details with accepted status associated with the asset
     * @param BuyOrders Set flag to true to fetch an array of buy order details  with accepted status associated with the asset
     * @param bIncludeFees Set flag to include fees associated with the asset
     * @param Collection Collection contract address
     * @param UpdatedMinTimestamp Minimum timestamp for when these assets were last updated, in ISO 8601 UTC format. Example: '2022-05-27T00:10:22Z'
     * @param UpdatedMaxTimestamp Maximum timestamp for when these assets were last updated, in ISO 8601 UTC format. Example: '2022-05-27T00:10:22Z'
     * @param AuxillaryFeePercentages Comma separated string of fee percentages that are to be paired with auxiliary_fee_recipients
     * @param AuxillaryFeeRecipients Comma separated string of fee recipients that are to be paired with auxiliary_fee_percentages
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="ImmutableX Get a list of assets [Async]",
        Category="3Studio|Blockchain|Layer 2|Methods",
        Meta=(BlueprintInternalUseOnly="true"))
    static UTSBC_ImmutablexGetListOfAssetsAsyncTask* K2_ImmutablexGetListOfAssetsAsync(
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
        const FString& AuxillaryFeeRecipients);

    virtual void Activate() override;
};