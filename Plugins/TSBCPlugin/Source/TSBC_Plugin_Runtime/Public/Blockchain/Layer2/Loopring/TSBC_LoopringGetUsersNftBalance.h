// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "Data/TSBC_Types.h"

// =============================================================================
// These includes are needed to prevent plugin build failures.
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
// =============================================================================

/**
 * Sends a Loopring REST-API call.
 */
class TSBC_PLUGIN_RUNTIME_API CTSBC_LoopringGetUsersNftBalance
{
public:
    DECLARE_DELEGATE_TwoParams(
        FTSBC_LoopringGetUsersNftBalance_Delegate,
        const bool /* bSuccess */,
        const FTSBC_HttpResponse& /* Response */);

    /**
     * Get users NFT balance, besides amount, it also includes tokenId and nftData
     * 
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent
     *                         successfully.
     * @param URL The URL to send the request to.
     * @param ApiKey API Key
     * @param AccountId Account ID
     * @param NftDatas The Loopring's NFT token data identifier which is a hash string of NFT token address and NFT_ID
     * @param TokenAddrs NFT token address
     * @param TokenIds The token slot ID in Loopring DEX.
     * @param Offset Number of records to skip
     * @param Limit Number of records to return
     * @param NonZero Hide 0 balance NFT token, default is true
     */
    static void LoopringGetUsersNftBalance(
        FTSBC_LoopringGetUsersNftBalance_Delegate ResponseDelegate,
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