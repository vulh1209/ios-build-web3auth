// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/TSBC_EthGetBalance.h"

#include "Module/TSBC_RuntimeLogCategories.h"
#include "JsonRpc/Generic/TSBC_SendJsonRpcRequest.h"
#include "Blockchain/TSBC_EthereumBlockchainFunctionLibrary.h"

void CTSBC_EthGetBalance::EthGetBalance(
    FTSBC_EthGetBalance_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ID,
    const FString& AccountAddress,
    const ETSBC_EthBlockIdentifier BlockIdentifier)
{
    CTSBC_SendJsonRpcRequest::FTSBC_JsonRpcResponse_Delegate InternalCallback;
    InternalCallback.BindLambda(
        [ResponseDelegate](const FTSBC_JsonRpcResponse& Response)
        {
            FTSBC_uint256 Balance;

            if(Response.bSuccess)
            {
                TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
                const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response.Body);
                if(FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
                {
                    if(JsonObject->HasField("result"))
                    {
                        const FString BalanceHex = JsonObject->GetStringField("result");
                        if(!FTSBC_uint256::ParseFromHexString(BalanceHex, Balance))
                        {
                            TSBC_LOG(Error, TEXT("Could not parse balance in response: '%s'"), *BalanceHex);
                        }
                    }
                    else
                    {
                        TSBC_LOG(
                            Error,
                            TEXT("Missing expected field 'result' in response body, response %s"),
                            *Response.Body);
                    }
                }
            }

            // ReSharper disable once CppExpressionWithoutSideEffects
            ResponseDelegate.ExecuteIfBound(Response.bSuccess, Response, Balance);
        });

    CTSBC_SendJsonRpcRequest::SendJsonRpcRequest(
        InternalCallback,
        URL,
        ID,
        "eth_getBalance",
        FString::Printf(
            TEXT("\"%s\",\"%s\""),
            *AccountAddress,
            *UTSBC_EthereumBlockchainFunctionLibrary::BlockIdentifierFromEnum(BlockIdentifier)));
}