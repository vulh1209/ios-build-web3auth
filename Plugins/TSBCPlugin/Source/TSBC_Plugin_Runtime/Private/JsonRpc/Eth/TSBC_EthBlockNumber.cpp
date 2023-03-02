// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/TSBC_EthBlockNumber.h"

#include "JsonRpc/Generic/TSBC_SendJsonRpcRequest.h"
#include "Module/TSBC_RuntimeLogCategories.h"

void CTSBC_EthBlockNumber::EthBlockNumber(
    FTSBC_EthBlockNumber_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ID)
{
    CTSBC_SendJsonRpcRequest::FTSBC_JsonRpcResponse_Delegate InternalCallback;
    InternalCallback.BindLambda(
        [ResponseDelegate](const FTSBC_JsonRpcResponse& Response)
        {
            int64 BlockNumber = 0;

            if(Response.bSuccess)
            {
                TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
                const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response.Body);
                if(FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
                {
                    if(JsonObject->HasField("result"))
                    {
                        const FString BlockNumberHex = JsonObject->GetStringField("result");
                        BlockNumber = FParse::HexNumber64(ToCStr(BlockNumberHex));
                    }
                    else
                    {
                        TSBC_LOG(Error, TEXT("Missing expected field 'result' in response body"));
                    }
                }
            }

            if(ResponseDelegate.IsBound())
            {
                ResponseDelegate.Execute(Response.bSuccess, Response, BlockNumber);
            }
        });

    CTSBC_SendJsonRpcRequest::SendJsonRpcRequest(
        InternalCallback,
        URL,
        ID,
        "eth_blockNumber",
        "");
}