// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/TSBC_Types.h"

#include "TSBC_LoopringQueryNftHoldersByLoopringsNftDataCallback.generated.h"

UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_LoopringQueryNftHoldersByLoopringsNftDataCallback : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_DELEGATE_TwoParams(
        FTSBC_K2_LoopringQueryNftHoldersByLoopringsNftDataCallback_Delegate,
        const bool,
        bSuccess,
        const FTSBC_HttpResponse&,
        Response);

public:
    /**
     * Query NFT holders by Loopring's nftData.
     *
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent
     *                         successfully.
     * @param URL URL the request is sent to.
     * @param ApiKey API Key
     * @param NftData The Loopring's NFT token data identifier which is a hash string of NFT token address and NFT_ID
     * @param Offset Number of records to skip
     * @param Limit Number of records to return
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Loopring Query NFT Holders by Loopring's NFT Data [Callback]",
        Category="3Studio|Blockchain|Layer 2|Methods")
    static void K2_LoopringQueryNftHoldersByLoopringsNftDataCallback(
        FTSBC_K2_LoopringQueryNftHoldersByLoopringsNftDataCallback_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ApiKey,
        const FString& NftData,
        const FString& Offset,
        const FString& Limit);
};