// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/TSBC_EthGetTransactionByHash.h"

#include "JsonRpc/Generic/TSBC_SendJsonRpcRequest.h"
#include "Module/TSBC_RuntimeLogCategories.h"

void CTSBC_EthGetTransactionByHash::ParseResponseField(
    const TSharedPtr<FJsonObject>& Object,
    const FString& FieldName,
    FString& OutValue,
    const bool bAllowNull)
{
    if(!Object->TryGetStringField(FieldName, OutValue))
    {
        TSBC_LOG_COND(!bAllowNull, Error, TEXT("Missing expected field '%s' in response body"), *FieldName);
    }
}

void CTSBC_EthGetTransactionByHash::ParseResponseField(
    const TSharedPtr<FJsonObject>& Object,
    const FString& FieldName,
    int32& OutValue,
    const bool bAllowNull)
{
    FString StringField = "";
    ParseResponseField(Object, FieldName, StringField, bAllowNull);

    OutValue = FParse::HexNumber(ToCStr(StringField));
}

void CTSBC_EthGetTransactionByHash::ParseResponseField(
    const TSharedPtr<FJsonObject>& Object,
    const FString& FieldName,
    int64& OutValue,
    const bool bAllowNull)
{
    FString StringField = "";
    ParseResponseField(Object, FieldName, StringField, bAllowNull);

    OutValue = FParse::HexNumber64(ToCStr(StringField));
}

void CTSBC_EthGetTransactionByHash::ParseResponseField(
    const TSharedPtr<FJsonObject>& Object,
    const FString& FieldName,
    FTSBC_uint256& OutValue,
    const bool bAllowNull)
{
    FString StringField = "";
    ParseResponseField(Object, FieldName, StringField, bAllowNull);

    if(!FTSBC_uint256::ParseFromHexString(StringField, OutValue))
    {
        TSBC_LOG_COND(
            !bAllowNull,
            Error,
            TEXT("Could not parse field '%s' in response: '%s'"),
            *FieldName,
            *StringField);
    }
}

void CTSBC_EthGetTransactionByHash::ProcessResponse(
    const FTSBC_JsonRpcResponse& Response,
    bool& OutTransactionFound,
    FTSBC_EthTransactionInfo& OutTransaction)
{
    OutTransactionFound = false;
    OutTransaction = FTSBC_EthTransactionInfo{};
    if(!Response.bSuccess)
    {
        return;
    }

    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
    const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response.Body);
    if(!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
    {
        TSBC_LOG(Error, TEXT("Could not deserialize response body '%s'"), *Response.Body);
        return;
    }

    if(JsonObject->HasTypedField<EJson::Null>("result"))
    {
        return;
    }

    if(!JsonObject->HasTypedField<EJson::Object>("result"))
    {
        TSBC_LOG(Error, TEXT("Missing expected object field 'result' in response body"));
        return;
    }

    const TSharedPtr<FJsonObject> ResultObject = JsonObject->GetObjectField("result");
    if(!ResultObject.IsValid())
    {
        TSBC_LOG(Error, TEXT("Error while parsing 'result' from response body into transaction info"))
        return;
    }

    OutTransactionFound = true;

    ParseResponseField(ResultObject, "blockHash", OutTransaction.BlockHash, true);
    ParseResponseField(ResultObject, "blockNumber", OutTransaction.BlockNumber, true);
    ParseResponseField(ResultObject, "from", OutTransaction.From, false);
    ParseResponseField(ResultObject, "gas", OutTransaction.Gas, false);
    ParseResponseField(ResultObject, "gasPrice", OutTransaction.GasPrice, true);
    ParseResponseField(ResultObject, "hash", OutTransaction.Hash, false);
    ParseResponseField(ResultObject, "input", OutTransaction.Input, false);
    ParseResponseField(ResultObject, "nonce", OutTransaction.Nonce, false);
    ParseResponseField(ResultObject, "to", OutTransaction.To, true);
    ParseResponseField(ResultObject, "transactionIndex", OutTransaction.TransactionIndex, true);
    ParseResponseField(ResultObject, "value", OutTransaction.Value, false);
    ParseResponseField(ResultObject, "v", OutTransaction.V, false);
    ParseResponseField(ResultObject, "r", OutTransaction.R, false);
    ParseResponseField(ResultObject, "s", OutTransaction.S, false);
}

void CTSBC_EthGetTransactionByHash::EthGetTransactionByHash(
    FTSBC_EthGetTransactionByHash_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ID,
    const FString& Hash)
{
    CTSBC_SendJsonRpcRequest::FTSBC_JsonRpcResponse_Delegate InternalCallback;
    InternalCallback.BindLambda(
        [ResponseDelegate](const FTSBC_JsonRpcResponse& Response)
        {
            bool bTransactionFound;
            FTSBC_EthTransactionInfo Transaction;
            ProcessResponse(Response, bTransactionFound, Transaction);

            if(ResponseDelegate.IsBound())
            {
                ResponseDelegate.Execute(Response.bSuccess, Response, bTransactionFound, Transaction);
            }
        });

    CTSBC_SendJsonRpcRequest::SendJsonRpcRequest(
        InternalCallback,
        URL,
        ID,
        "eth_getTransactionByHash",
        FString::Printf(
            TEXT("\"%s\""),
            *Hash));
}