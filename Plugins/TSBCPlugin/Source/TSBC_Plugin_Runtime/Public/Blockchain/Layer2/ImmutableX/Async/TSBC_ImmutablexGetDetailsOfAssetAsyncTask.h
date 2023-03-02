// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Data/TSBC_Types.h"

#include "TSBC_ImmutablexGetDetailsOfAssetAsyncTask.generated.h"

/**
 * Send a ImmutableX REST-API call using an AsyncTask.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_ImmutablexGetDetailsOfAssetAsyncTask : public UBlueprintAsyncActionBase
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
    FString _TokenAddress;

    UPROPERTY()
    FString _TokenId;

    UPROPERTY()
    bool _bIncludeFees;

public:
    /**
     * Get details of an asset.
     * 
     * @param URL The URL to send the request to.
     * @param TokenAddress (Required) Address of the ERC721 contract
     * @param TokenId (Required) Either ERC721 token ID or internal IMX ID
     * @param bIncludeFees Set flag to include fees associated with the asset
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="ImmutableX Get details of assets [Async]",
        Category="3Studio|Blockchain|Layer 2|Methods",
        Meta=(BlueprintInternalUseOnly="true"))
    static UTSBC_ImmutablexGetDetailsOfAssetAsyncTask* K2_ImmutablexGetDetailsOfAssetsAsync(
        const FString& URL,
        const FString& TokenAddress,
        const FString& TokenId,
        const bool bIncludeFees);

    virtual void Activate() override;
};