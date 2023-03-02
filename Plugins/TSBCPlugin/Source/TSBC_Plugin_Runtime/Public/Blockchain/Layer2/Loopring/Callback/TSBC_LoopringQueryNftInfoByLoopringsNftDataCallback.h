// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/TSBC_Types.h"

#include "TSBC_LoopringQueryNftInfoByLoopringsNftDataCallback.generated.h"

UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_LoopringQueryNftInfoByLoopringsNftDataCallback
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_DELEGATE_TwoParams(
        FTSBC_K2_LoopringQueryNftInfoByLoopringsNftDataCallback_Delegate,
        const bool,
        bSuccess,
        const FTSBC_HttpResponse&,
        Response);

public:
    /**
     * Query NFT info by Loopring's nftData.
     *
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent
     *                         successfully.
     * @param URL URL the request is sent to.
     * @param ApiKey API Key
     * @param NftDatas The Loopring's NFT token data identifier which is a hash string of NFT token address and NFT_ID
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Loopring Query NFT Info by Loopring's NFT Data [Callback]",
        Category="3Studio|Blockchain|Layer 2|Methods")
    static void K2_TSBC_LoopringQueryNftInfoByLoopringsNftDataCallback(
        FTSBC_K2_LoopringQueryNftInfoByLoopringsNftDataCallback_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ApiKey,
        const FString& NftDatas);
};