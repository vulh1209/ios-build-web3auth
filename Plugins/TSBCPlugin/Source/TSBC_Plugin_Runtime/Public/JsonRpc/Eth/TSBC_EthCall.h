// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "Data/TSBC_Types.h"

/**
 * Sends a JSON-RPC request to call "eth_call".
 */
class TSBC_PLUGIN_RUNTIME_API CTSBC_EthCall
{
public:
    DECLARE_DELEGATE_ThreeParams(
        FTSBC_EthCall_Delegate,
        const bool /* bSuccess */,
        const FTSBC_JsonRpcResponse& /* Response */,
        const FString& /* ResponseData */);

    /**
     * Calls a function of an Ethereum Smart Contract for reading values.
     * 
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent
     *                         successfully.
     * @param URL The URL to send the request to.
     * @param ID The identifier to correlate the context between two objects.
     * @param FromAddress The address the transaction is sent from.
     * @param ToAddress The address the transaction is directed to.
     * @param Data The hash of the invoked method signature and encoded parameters (ABI).
     * @param BlockIdentifier The block number to use. (Default: "latest")
     */
    static void EthCall(
        FTSBC_EthCall_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ID,
        const FString& FromAddress,
        const FString& ToAddress,
        const FString& Data,
        const ETSBC_EthBlockIdentifier BlockIdentifier = ETSBC_EthBlockIdentifier::Latest);
};