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
class TSBC_PLUGIN_RUNTIME_API CTSBC_LoopringQueryNftInfoByLoopringsNftData
{
public:
    DECLARE_DELEGATE_TwoParams(
        FTSBC_LoopringQueryNftInfoByLoopringsNftData_Delegate,
        const bool /* bSuccess */,
        const FTSBC_HttpResponse& /* Response */);

    /**
     * Query NFT info by Loopring's nftData.
     * 
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent
     *                         successfully.
     * @param URL The URL to send the request to.
     * @param ApiKey API Key
     * @param NftDatas The Loopring's NFT token data identifier which is a hash string of NFT token address and NFT_ID
     */
    static void LoopringQueryNftInfoByLoopringsNftData(
        FTSBC_LoopringQueryNftInfoByLoopringsNftData_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ApiKey,
        const FString& NftDatas);
};