// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/TSBC_Types.h"

#include "TSBC_ImmutablexGetListOfAssetsCallback.generated.h"

UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_ImmutablexGetListOfAssetsCallback : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_DELEGATE_TwoParams(
        FTSBC_K2_ImmutablexGetListOfAssetsCallback_Delegate,
        const bool,
        bSuccess,
        const FTSBC_HttpResponse&,
        Response);

public:
    /**
     * Get a list of assets.
     * 
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent
     *                         successfully.
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
        DisplayName="ImmutableX Get a list of assets [Callback]",
        Category="3Studio|Blockchain|Layer 2|Methods")
    static void K2_ImmutablexGetListOfAssetsCallback(
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
        const FString& AuxillaryFeeRecipients);
};