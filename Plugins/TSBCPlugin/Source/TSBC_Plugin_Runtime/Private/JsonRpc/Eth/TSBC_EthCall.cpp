// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/TSBC_EthCall.h"

#include "Module/TSBC_RuntimeLogCategories.h"
#include "JsonRpc/Generic/TSBC_SendJsonRpcRequest.h"
#include "Blockchain/TSBC_EthereumBlockchainFunctionLibrary.h"

void CTSBC_EthCall::EthCall(
    FTSBC_EthCall_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ID,
    const FString& FromAddress,
    const FString& ToAddress,
    const FString& Data,
    const ETSBC_EthBlockIdentifier BlockIdentifier)
{
    // TODO Currently only supports reading data; Impl writing data as well.

    CTSBC_SendJsonRpcRequest::FTSBC_JsonRpcResponse_Delegate InternalCallback;
    InternalCallback.BindLambda(
        [ResponseDelegate](const FTSBC_JsonRpcResponse& Response)
        {
            FString ResponseData = "";

            if(Response.bSuccess)
            {
                TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
                const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response.Body);
                if(FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
                {
                    if(JsonObject->HasField("result"))
                    {
                        ResponseData = JsonObject->GetStringField("result");
                    }
                    else
                    {
                        TSBC_LOG(Error, TEXT("Missing expected field 'result' in response body"));
                    }
                }
            }

            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(Response.bSuccess, Response, ResponseData);
        });

    CTSBC_SendJsonRpcRequest::SendJsonRpcRequest(
        InternalCallback,
        URL,
        ID,
        "eth_call",
        FString::Printf(
            TEXT("{\"from\":\"%s\",\"to\":\"%s\",\"data\":\"%s\"}, \"%s\""),
            *FromAddress,
            *ToAddress,
            *Data,
            *UTSBC_EthereumBlockchainFunctionLibrary::BlockIdentifierFromEnum(BlockIdentifier)));
}