// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "Data/TSBC_Types.h"
#include "Math/TSBC_uint256.h"

/**
 * Sends a JSON-RPC request to call "eth_gasPrice".
 */
class TSBC_PLUGIN_RUNTIME_API CTSBC_EthGasPrice
{
public:
    DECLARE_DELEGATE_ThreeParams(
        FTSBC_EthGasPrice_Delegate,
        const bool /* bSuccess */,
        const FTSBC_JsonRpcResponse& /* Response */,
        const FTSBC_uint256& /* GasPrice */);

    /**
     * Gets the current gas price in Wei.
     *
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent
     *                         successfully.
     * @param URL The URL to send the request to.
     * @param ID The identifier to correlate the context between two objects.
     */
    static void EthGasPrice(
        FTSBC_EthGasPrice_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ID);
};