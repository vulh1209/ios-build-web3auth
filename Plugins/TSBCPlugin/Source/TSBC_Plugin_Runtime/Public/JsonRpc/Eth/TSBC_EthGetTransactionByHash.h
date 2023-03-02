// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
// =============================================================================
// These includes are needed to prevent plugin build failures.
#include "Dom/JsonObject.h"
// =============================================================================
#include "Data/TSBC_Types.h"
#include "Data/TSBC_EthTransactionTypes.h"

/**
 * Sends a JSON-RPC request to call "eth_getTransactionByHash".
 */
class TSBC_PLUGIN_RUNTIME_API CTSBC_EthGetTransactionByHash
{
public:
    DECLARE_DELEGATE_FourParams(
        FTSBC_EthGetTransactionByHash_Delegate,
        const bool /* bSuccess */,
        const FTSBC_JsonRpcResponse& /* Response */,
        bool /* Transaction Found */,
        const FTSBC_EthTransactionInfo& /* Transaction Info */)

    /**
     * Gets the information about a transaction requested by transaction hash.
     * 
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent
     *                         successfully.
     * @param URL The URL to send the request to.
     * @param ID The identifier to correlate the context between two objects.
     * @param Hash Transaction hash
     */
    static void EthGetTransactionByHash(
        FTSBC_EthGetTransactionByHash_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ID,
        const FString& Hash);

private:
    static void ParseResponseField(
        const TSharedPtr<FJsonObject>& Object,
        const FString& FieldName,
        FString& OutValue,
        const bool bAllowNull);

    static void ParseResponseField(
        const TSharedPtr<FJsonObject>& Object,
        const FString& FieldName,
        int32& OutValue,
        const bool bAllowNull);

    static void ParseResponseField(
        const TSharedPtr<FJsonObject>& Object,
        const FString& FieldName,
        int64& OutValue,
        const bool bAllowNull);

    static void ParseResponseField(
        const TSharedPtr<FJsonObject>& Object,
        const FString& FieldName,
        FTSBC_uint256& OutValue,
        const bool bAllowNull);

    static void ProcessResponse(
        const FTSBC_JsonRpcResponse& Response,
        bool& OutTransactionFound,
        FTSBC_EthTransactionInfo& OutTransaction);
};