// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Data/TSBC_Types.h"

#include "TSBC_LoopringQueryNftHoldersByLoopringsNftDataAsyncTask.generated.h"

/**
 * Send a Loopring REST-API call using an AsyncTask.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_LoopringQueryNftHoldersByLoopringsNftDataAsyncTask
    : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
        FTSBC_K2_LoopringQueryNftHoldersByLoopringsNftDataAsyncTask_Delegate,
        const bool,
        bSuccess,
        const FTSBC_HttpResponse&,
        Response);

public:
    UPROPERTY(BlueprintAssignable, Category="3Studio|HTTP")
    FTSBC_K2_LoopringQueryNftHoldersByLoopringsNftDataAsyncTask_Delegate OnCompleted;

private:
    UPROPERTY()
    FString _URL;

    UPROPERTY()
    FString _ApiKey;

    UPROPERTY()
    FString _NftData;

    UPROPERTY()
    FString _Offset;

    UPROPERTY()
    FString _Limit;

public:
    /**
     * Query NFT holders by Loopring's nftData.
     * 
     * @param URL URL the request is sent to.
     * @param ApiKey API Key
     * @param NftData The Loopring's NFT token data identifier which is a hash string of NFT token address and NFT_ID
     * @param Offset Number of records to skip
     * @param Limit Number of records to return
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Loopring Query NFT Holders by Loopring's NFT Data [Async]",
        Category="3Studio|Blockchain|Layer 2|Methods",
        Meta=(BlueprintInternalUseOnly="true"))
    static UTSBC_LoopringQueryNftHoldersByLoopringsNftDataAsyncTask* K2_LoopringQueryNftHoldersByLoopringsNftDataAsync(
        const FString& URL,
        const FString& ApiKey,
        const FString& NftData,
        const FString& Offset,
        const FString& Limit);

    virtual void Activate() override;
};