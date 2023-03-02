// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "Data/TSBC_Types.h"

/**
 * Sends a JSON-RPC request to call "eth_SendRawTransaction".
 */
class TSBC_PLUGIN_RUNTIME_API CTSBC_EthSendRawTransaction
{
public:
    DECLARE_DELEGATE_ThreeParams(
        FTSBC_EthSendRawTransaction_Delegate,
        const bool /* bSuccess */,
        const FTSBC_JsonRpcResponse& /* Response */,
        const FString& /* TransactionHash */);

    /**
     * Sends a raw transaction.
     *
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent successfully.
     * @param URL The URL to send the request to.
     * @param ID The identifier to correlate the context between two objects.
     * @param SignedTransactionData The encoded and signed transaction data.
     */
    static void EthSendRawTransaction(
        FTSBC_EthSendRawTransaction_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ID,
        const FString& SignedTransactionData);
};