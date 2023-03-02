// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/TSBC_EthEstimateGas.h"

#include "Module/TSBC_RuntimeLogCategories.h"
#include "JsonRpc/Generic/TSBC_SendJsonRpcRequest.h"

void CTSBC_EthEstimateGas::EthEstimateGas(
    FTSBC_EthEstimateGas_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ID,
    const FString& FromAddress,
    const FString& ToAddress,
    const FString& Data,
    const FTSBC_uint256 Value)
{
    CTSBC_SendJsonRpcRequest::FTSBC_JsonRpcResponse_Delegate InternalCallback;
    InternalCallback.BindLambda(
        [ResponseDelegate](const FTSBC_JsonRpcResponse& Response)
        {
            FTSBC_uint256 EstimatedGas;

            if(Response.bSuccess)
            {
                TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
                const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response.Body);
                if(FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
                {
                    if(JsonObject->HasField("result"))
                    {
                        const FString EstimatedGasHex = JsonObject->GetStringField("result");
                        if(!FTSBC_uint256::ParseFromHexString(EstimatedGasHex, EstimatedGas))
                        {
                            TSBC_LOG(
                                Error,
                                TEXT("Could not parse estimated gas in response: '%s'"),
                                *EstimatedGasHex);
                        }
                    }
                    else
                    {
                        TSBC_LOG(Error, TEXT("Missing expected field 'result' in response body"));
                    }
                }
            }

            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(Response.bSuccess, Response, EstimatedGas);
        });

    CTSBC_SendJsonRpcRequest::SendJsonRpcRequest(
        InternalCallback,
        URL,
        ID,
        "eth_estimateGas",
        Value == 0
        ? FString::Printf(
            TEXT("{\"from\":\"%s\",\"to\":\"%s\",\"data\":\"%s\"}"),
            *FromAddress,
            *ToAddress,
            *Data)
        : FString::Printf(
            TEXT("{\"from\":\"%s\",\"to\":\"%s\",\"value\":\"%s\",\"data\":\"%s\"}"),
            *FromAddress,
            *ToAddress,
            *Value.ToHexString(),
            *Data));
}