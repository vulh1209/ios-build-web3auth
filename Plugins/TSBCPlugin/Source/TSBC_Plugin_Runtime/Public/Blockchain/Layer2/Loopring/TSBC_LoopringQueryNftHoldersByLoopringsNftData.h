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
class TSBC_PLUGIN_RUNTIME_API CTSBC_LoopringQueryNftHoldersByLoopringsNftData
{
public:
    DECLARE_DELEGATE_TwoParams(
        FTSBC_LoopringQueryNftHoldersByLoopringsNftData_Delegate,
        const bool /* bSuccess */,
        const FTSBC_HttpResponse& /* Response */);

    /**
     * Query NFT holders by Loopring's nftData.
     * 
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent
     *                         successfully.
     * @param URL The URL to send the request to.
     * @param ApiKey API Key
     * @param NftData The Loopring's NFT token data identifier which is a hash string of NFT token address and NFT_ID
     * @param Offset Number of records to skip
     * @param Limit Number of records to return
     */
    static void LoopringQueryNftHoldersByLoopringsNftData(
        FTSBC_LoopringQueryNftHoldersByLoopringsNftData_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ApiKey,
        const FString& NftData,
        const FString& Offset,
        const FString& Limit);
};