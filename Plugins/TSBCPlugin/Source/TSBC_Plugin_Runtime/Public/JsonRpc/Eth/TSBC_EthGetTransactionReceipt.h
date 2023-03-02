// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "Data/TSBC_Types.h"
#include "Data/TSBC_EthTransactionTypes.h"

/**
 * Sends a JSON-RPC request to call "eth_getTransactionReceipt".
 */
class TSBC_PLUGIN_RUNTIME_API CTSBC_EthGetTransactionReceipt
{
public:
    DECLARE_DELEGATE_FourParams(
        FTSBC_EthGetTransactionReceipt_Delegate,
        const bool /* bSuccess */,
        const FTSBC_JsonRpcResponse& /* Response */,
        const bool /* Receipt Found */,
        const FTSBC_EthTransactionReceipt& /* Transaction Receipt */);

    /**
     * Gets the receipt of a transaction by transaction hash.
     *
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent successfully.
     *
     * @param URL The URL to send the request to.
     * @param ID The identifier to correlate the context between two objects.
     * @param Hash Transaction hash.
     */
    static void EthGetTransactionReceipt(
        FTSBC_EthGetTransactionReceipt_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ID,
        const FString& Hash
    );

private:
    static void ProcessResponse(
        const FTSBC_JsonRpcResponse& Response,
        bool& OutReceiptFound,
        FTSBC_EthTransactionReceipt& OutReceipt);

    static void ParseResponseField(
        const TSharedPtr<FJsonObject>& Object,
        const FString& FieldName,
        FString& OutValue,
        const bool bAllowNull);

    static void ParseResponseField(
        const TSharedPtr<FJsonObject>& Object,
        const FString& FieldName,
        bool& OutValue,
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

    static void ParseResponseField(
        const TSharedPtr<FJsonObject>& Object,
        const FString& FieldName,
        ETSBC_EthTransactionType& OutValue,
        const bool bAllowNull);

    static void ParseResponseField(
        const TSharedPtr<FJsonObject>& Object,
        const FString& FieldName,
        TArray<FString>& OutValue,
        const bool bAllowNull);

    static void ParseResponseField(
        const TSharedPtr<FJsonObject>& Object,
        const FString& FieldName,
        TArray<FTSBC_EthLog>& OutValue,
        const bool bAllowNull);

};