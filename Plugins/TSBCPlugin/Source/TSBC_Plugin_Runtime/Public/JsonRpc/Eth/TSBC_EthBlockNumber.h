// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "Data/TSBC_Types.h"

/**
 * Sends a JSON-RPC request to call "eth_blockNumber".
 */
class TSBC_PLUGIN_RUNTIME_API CTSBC_EthBlockNumber
{
public:
    DECLARE_DELEGATE_ThreeParams(
        FTSBC_EthBlockNumber_Delegate,
        const bool /* bSuccess */,
        const FTSBC_JsonRpcResponse& /* Response */,
        const int64 /* Block Number */);

    /**
     * Gets the number of the most recent block.
     *
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent successfully.
     *
     * @param URL The URL to send this request to.
     * @param ID The identifier to correlate the context between two objects.
     */
    static void EthBlockNumber(
        FTSBC_EthBlockNumber_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ID);
};