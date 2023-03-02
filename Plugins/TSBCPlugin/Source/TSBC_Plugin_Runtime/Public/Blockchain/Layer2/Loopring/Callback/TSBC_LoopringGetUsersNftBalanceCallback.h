// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/TSBC_Types.h"

#include "TSBC_LoopringGetUsersNftBalanceCallback.generated.h"

UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_LoopringGetUsersNftBalanceCallback : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_DELEGATE_TwoParams(
        FTSBC_K2_LoopringGetUsersNftBalanceCallback_Delegate,
        const bool,
        bSuccess,
        const FTSBC_HttpResponse&,
        Response);

public:
    /**
     * Get users NFT balance, besides amount, it also includes tokenId and nftData
     *
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent
     *                         successfully.
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
        DisplayName="Loopring Get Users NFT Balance [Callback]",
        Category="3Studio|Blockchain|Layer 2|Methods")
    static void K2_LoopringGetUsersNftBalanceCallback(
        FTSBC_K2_LoopringGetUsersNftBalanceCallback_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ApiKey,
        const FString& AccountId,
        const FString& NftDatas,
        const FString& TokenAddrs,
        const FString& TokenIds,
        const FString& Offset,
        const FString& Limit,
        const FString& NonZero);
};