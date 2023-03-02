// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "Data/TSBC_Types.h"

/**
 * Sends a JSON-RPC request to call "eth_getTransactionCount".
 */
class TSBC_PLUGIN_RUNTIME_API CTSBC_EthGetTransactionCount
{
public:
    DECLARE_DELEGATE_ThreeParams(
        FTSBC_EthGetTransactionCount_Delegate,
        const bool /* bSuccess */,
        const FTSBC_JsonRpcResponse& /* Response */,
        const int32& /* Transaction Count */);

    /**
     * Gets the transaction count for a specific address.
     * 
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent successfully.
     * @param URL The URL to send the request to.
     * @param Address The address for which to get the transaction count for.
     * @param BlockIdentifier The block number to use. (Default: "latest")
     * @param ID The identifier to correlate the context between two objects.
     */
    static void EthGetTransactionCount(
        FTSBC_EthGetTransactionCount_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ID,
        const FString& Address,
        const ETSBC_EthBlockIdentifier BlockIdentifier = ETSBC_EthBlockIdentifier::Latest);
};