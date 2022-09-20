// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "Data/TSBC_Types.h"
#include "Math/TSBC_uint256.h"

/**
 * Sends a JSON-RPC request to call "eth_getBalance".
 */
class TSBC_PLUGIN_RUNTIME_API CTSBC_EthGetBalance
{
public:
    DECLARE_DELEGATE_ThreeParams(
        FTSBC_EthGetBalance_Delegate,
        const bool /* bSuccess */,
        const FTSBC_JsonRpcResponse& /* Response */,
        const FTSBC_uint256& /* Balance */);

    /**
     * Gets the balance in Wei for the specified account.
     *
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent
     *                         successfully.
     * @param URL The URL to send the request to.
     * @param ID The identifier to correlate the context between two objects.
     * @param AccountAddress Address of the account to check.
     * @param BlockIdentifier The block number to use. (Default: "latest")
     */
    static void EthGetBalance(
        FTSBC_EthGetBalance_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ID,
        const FString& AccountAddress,
        const ETSBC_EthBlockIdentifier BlockIdentifier = ETSBC_EthBlockIdentifier::Latest);
};