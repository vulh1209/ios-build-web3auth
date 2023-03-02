// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/TSBC_EthGasPrice.h"

#include "Module/TSBC_RuntimeLogCategories.h"
#include "JsonRpc/Generic/TSBC_SendJsonRpcRequest.h"

void CTSBC_EthGasPrice::EthGasPrice(FTSBC_EthGasPrice_Delegate ResponseDelegate, const FString& URL, const FString& ID)
{
    CTSBC_SendJsonRpcRequest::FTSBC_JsonRpcResponse_Delegate InternalCallback;
    InternalCallback.BindLambda(
        [ResponseDelegate](const FTSBC_JsonRpcResponse& Response)
        {
            FTSBC_uint256 GasPrice;

            if(Response.bSuccess)
            {
                TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
                const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response.Body);
                if(FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
                {
                    if(JsonObject->HasField("result"))
                    {
                        const FString GasPriceHex = JsonObject->GetStringField("result");
                        if(!FTSBC_uint256::ParseFromHexString(GasPriceHex, GasPrice))
                        {
                            TSBC_LOG(Error, TEXT("Could not parse gas price in response: '%s'"), *GasPriceHex);
                        }
                    }
                    else
                    {
                        TSBC_LOG(Error, TEXT("Missing expected field 'result' in response body"));
                    }
                }
            }

            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(Response.bSuccess, Response, GasPrice);
        });

    CTSBC_SendJsonRpcRequest::SendJsonRpcRequest(
        InternalCallback,
        URL,
        ID,
        "eth_gasPrice",
        "");
}