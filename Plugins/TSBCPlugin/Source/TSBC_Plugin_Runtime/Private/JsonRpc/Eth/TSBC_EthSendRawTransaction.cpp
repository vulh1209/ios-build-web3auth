// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/TSBC_EthSendRawTransaction.h"

#include "Module/TSBC_RuntimeLogCategories.h"
#include "JsonRpc/Generic/TSBC_SendJsonRpcRequest.h"

void CTSBC_EthSendRawTransaction::EthSendRawTransaction(
    FTSBC_EthSendRawTransaction_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ID,
    const FString& SignedTransactionData)
{
    CTSBC_SendJsonRpcRequest::FTSBC_JsonRpcResponse_Delegate InternalCallback;
    InternalCallback.BindLambda(
        [ResponseDelegate](const FTSBC_JsonRpcResponse& Response)
        {
            FString TransactionHash = "";

            if(Response.bSuccess)
            {
                TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
                const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response.Body);
                if(FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
                {
                    if(JsonObject->HasField("result"))
                    {
                        TransactionHash = JsonObject->GetStringField("result");
                    }
                    else
                    {
                        TSBC_LOG(Error, TEXT("Missing expected field 'result' in response body"));
                    }
                }
            }

            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(Response.bSuccess, Response, TransactionHash);
        });

    CTSBC_SendJsonRpcRequest::SendJsonRpcRequest(
        InternalCallback,
        URL,
        ID,
        "eth_sendRawTransaction",
        FString::Printf(
            TEXT("\"%s\""),
            *SignedTransactionData));
}