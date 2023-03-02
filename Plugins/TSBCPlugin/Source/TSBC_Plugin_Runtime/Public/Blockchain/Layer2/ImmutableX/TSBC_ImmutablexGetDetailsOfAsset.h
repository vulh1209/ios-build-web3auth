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
 * Sends a ImmutableX REST-API call.
 */
class TSBC_PLUGIN_RUNTIME_API CTSBC_ImmutablexGetDetailsOfAsset
{
public:
    DECLARE_DELEGATE_TwoParams(
        FTSBC_ImmutablexGetDetailsOfAsset_Delegate,
        const bool /* bSuccess */,
        const FTSBC_HttpResponse& /* Response */);

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
    static void ImmutablexGetDetailsOfAsset(
        FTSBC_ImmutablexGetDetailsOfAsset_Delegate ResponseDelegate,
        const FString& URL,
        const FString& TokenAddress,
        const FString& TokenId,
        const bool bIncludeFees);
};