// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Data/TSBC_Types.h"

#include "TSBC_LoopringGetUsersNftBalanceAsyncTask.generated.h"

/**
 * Send a Loopring REST-API call using an AsyncTask.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_LoopringGetUsersNftBalanceAsyncTask : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
        FTSBC_K2_LoopringGetUsersNftBalanceAsyncTask_Delegate,
        const bool,
        bSuccess,
        const FTSBC_HttpResponse&,
        Response);

public:
    UPROPERTY(BlueprintAssignable, Category="3Studio|HTTP")
    FTSBC_K2_LoopringGetUsersNftBalanceAsyncTask_Delegate OnCompleted;

private:
    UPROPERTY()
    FString _URL;

    UPROPERTY()
    FString _ApiKey;

    UPROPERTY()
    FString _AccountId;

    UPROPERTY()
    FString _NftDatas;

    UPROPERTY()
    FString _TokenAddrs;

    UPROPERTY()
    FString _TokenIds;

    UPROPERTY()
    FString _Offset;

    UPROPERTY()
    FString _Limit;

    UPROPERTY()
    FString _NonZero;

public:
    /**
     * Get users NFT balance, besides amount, it also includes tokenId and nftData
     * 
     * @param URL URL the request is sent to.
     * @param ApiKey API Key
     * @param AccountId Account ID
     * @param NftDatas The Loopring's NFT token data identifier which is a hash string of NFT token address and NFT_ID
     * @param TokenAddrs NFT token address
     * @param TokenIds The token slot ID in loopring DEX.
     * @param Offset Number of records to skip
     * @param Limit Number of records to return
     * @param NonZero Hide 0 balance NFT token, default is true
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Loopring Get Users NFT Balance [Async]",
        Category="3Studio|Blockchain|Layer 2|Methods",
        Meta=(BlueprintInternalUseOnly="true"))
    static UTSBC_LoopringGetUsersNftBalanceAsyncTask* K2_LoopringGetUsersNftBalanceAsync(
        const FString& URL,
        const FString& ApiKey,
        const FString& AccountId,
        const FString& NftDatas,
        const FString& TokenAddrs,
        const FString& TokenIds,
        const FString& Offset,
        const FString& Limit,
        const FString& NonZero);

    virtual void Activate() override;
};