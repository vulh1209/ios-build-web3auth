// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "Data/TSBC_Types.h"

// =============================================================================
// These includes are needed to prevent plugin build failures.
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
// =============================================================================

class TSBC_PLUGIN_RUNTIME_API CTSBC_SendJsonRpcRequest
{
public:
    DECLARE_DELEGATE_OneParam(
        FTSBC_JsonRpcResponse_Delegate,
        FTSBC_JsonRpcResponse);

    /**
     * Sends a JSON-RPC request to the specified URL in order to invoke a method.
     * Optionally, parameters can be specified (in JSON format) that will be used during the invocation of the method.
     * The ID argument can be used to correlate requests and responses.
     *
     * @param ResponseDelegate Delegate to handle the response. Will also be called if a request could not be sent
     *                         successfully.
     * @param URL JSON-RPC URL the request is sent to.
     * @param ID The identifier to correlate the context between two objects.
     * @param Method The name of the method to be invoked.
     * @param Params [Optional] Parameters (in JSON format) to be used during the invocation of the method.
     */
    static void SendJsonRpcRequest(
        FTSBC_JsonRpcResponse_Delegate ResponseDelegate,
        const FString& URL,
        const FString& ID,
        const FString& Method,
        const FString& Params);
};