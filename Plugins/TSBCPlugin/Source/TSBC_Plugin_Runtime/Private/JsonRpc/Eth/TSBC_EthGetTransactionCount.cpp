// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/TSBC_EthGetTransactionCount.h"

#include "Module/TSBC_RuntimeLogCategories.h"
#include "JsonRpc/Generic/TSBC_SendJsonRpcRequest.h"
#include "Blockchain/TSBC_EthereumBlockchainFunctionLibrary.h"

void CTSBC_EthGetTransactionCount::EthGetTransactionCount(
    FTSBC_EthGetTransactionCount_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ID,
    const FString& Address,
    const ETSBC_EthBlockIdentifier BlockIdentifier)
{
    CTSBC_SendJsonRpcRequest::FTSBC_JsonRpcResponse_Delegate InternalCallback;
    InternalCallback.BindLambda(
        [ResponseDelegate](const FTSBC_JsonRpcResponse& Response)
        {
            int32 TransactionCount;

            if(Response.bSuccess)
            {
                TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
                const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response.Body);
                if(FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
                {
                    if(JsonObject->HasField("result"))
                    {
                        const FString TransactionCountHex = JsonObject->GetStringField("result");
                        TransactionCount = FParse::HexNumber(ToCStr(TransactionCountHex));
                    }
                    else
                    {
                        TSBC_LOG(Error, TEXT("Missing expected field 'result' in response body"));
                    }
                }
            }

            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(Response.bSuccess, Response, TransactionCount);
        });

    CTSBC_SendJsonRpcRequest::SendJsonRpcRequest(
        InternalCallback,
        URL,
        ID,
        "eth_getTransactionCount",
        FString::Printf(
            TEXT("\"%s\",\"%s\""),
            *Address,
            *UTSBC_EthereumBlockchainFunctionLibrary::BlockIdentifierFromEnum(BlockIdentifier)));
}