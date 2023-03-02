// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/TSBC_Types.h"

#include "TSBC_ImmutablexGetDetailsOfAssetCallback.generated.h"

UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_ImmutablexGetDetailsOfAssetCallback : public UBlueprintFunctionLibrary
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
     * Get details of an asset.
     * 
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent
     *                         successfully.
     * @param URL The URL to send the request to.
     * @param TokenAddress (Required) Address of the ERC721 contract
     * @param TokenId (Required) Either ERC721 token ID or internal IMX ID
     * @param bIncludeFees Set flag to include fees associated with the asset
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="ImmutableX Get details of assets [Callback]",
        Category="3Studio|Blockchain|Layer 2|Methods")
    static void K2_ImmutablexGetDetailsOfAssetsCallback(
        FTSBC_K2_ImmutablexGetListOfAssetsCallback_Delegate ResponseDelegate,
        const FString& URL,
        const FString& TokenAddress,
        const FString& TokenId,
        const bool bIncludeFees);
};