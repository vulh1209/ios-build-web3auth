// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "JsonRpc/Eth/TSBC_EthGetTransactionReceipt.h"

#include "JsonRpc/Generic/TSBC_SendJsonRpcRequest.h"
#include "Module/TSBC_RuntimeLogCategories.h"

void CTSBC_EthGetTransactionReceipt::EthGetTransactionReceipt(
    FTSBC_EthGetTransactionReceipt_Delegate ResponseDelegate,
    const FString& URL,
    const FString& ID,
    const FString& Hash)
{
    CTSBC_SendJsonRpcRequest::FTSBC_JsonRpcResponse_Delegate InternalCallback;
    InternalCallback.BindLambda(
        [ResponseDelegate](const FTSBC_JsonRpcResponse& Response)
        {
            bool bReceiptFound;
            FTSBC_EthTransactionReceipt Receipt;
            ProcessResponse(Response, bReceiptFound, Receipt);

            if(ResponseDelegate.IsBound())
            {
                ResponseDelegate.Execute(Response.bSuccess, Response, bReceiptFound, Receipt);
            }
        });

    CTSBC_SendJsonRpcRequest::SendJsonRpcRequest(
        InternalCallback,
        URL,
        ID,
        "eth_getTransactionReceipt",
        FString::Printf(
            TEXT("\"%s\""),
            *Hash));
}

void CTSBC_EthGetTransactionReceipt::ProcessResponse(
    const FTSBC_JsonRpcResponse& Response,
    bool& OutReceiptFound,
    FTSBC_EthTransactionReceipt& OutReceipt)
{
    OutReceiptFound = false;
    OutReceipt = FTSBC_EthTransactionReceipt{};
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
        TSBC_LOG(Error, TEXT("Error while parsing 'result' from response body into transaction receipt"))
        return;
    }

    OutReceiptFound = true;

    ParseResponseField(ResultObject, "transactionHash", OutReceipt.TransactionHash, false);
    ParseResponseField(ResultObject, "transactionIndex", OutReceipt.TransactionIndex, false);
    ParseResponseField(ResultObject, "blockHash", OutReceipt.BlockHash, false);
    ParseResponseField(ResultObject, "blockNumber", OutReceipt.BlockNumber, false);
    ParseResponseField(ResultObject, "from", OutReceipt.From, false);
    ParseResponseField(ResultObject, "to", OutReceipt.To, true);
    ParseResponseField(ResultObject, "cumulativeGasUsed", OutReceipt.CumulativeGasUsed, false);
    ParseResponseField(ResultObject, "effectiveGasPrice", OutReceipt.EffectiveGasPrice, false);
    ParseResponseField(ResultObject, "gasUsed", OutReceipt.GasUsed, false);
    ParseResponseField(ResultObject, "contractAddress", OutReceipt.ContractAddress, true);
    ParseResponseField(ResultObject, "logs", OutReceipt.Logs, false);
    ParseResponseField(ResultObject, "logsBloom", OutReceipt.LogsBloom, false);
    ParseResponseField(ResultObject, "type", OutReceipt.Type, false);

    if(!ResultObject->TryGetStringField("root", OutReceipt.Root))
    {
        FString StatusString = "";
        if(ResultObject->TryGetStringField("status", StatusString))
        {
            OutReceipt.bStatus = FParse::HexNumber(ToCStr(StatusString)) == 1; /* 1 := success, 0 := failure */
        }
        else
        {
            TSBC_LOG(Error, TEXT("Missing expected field 'status' in response body"));
        }
    }
}

void CTSBC_EthGetTransactionReceipt::ParseResponseField(
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

void CTSBC_EthGetTransactionReceipt::ParseResponseField(
    const TSharedPtr<FJsonObject>& Object,
    const FString& FieldName,
    bool& OutValue,
    const bool bAllowNull)
{
    if(!Object->TryGetBoolField(FieldName, OutValue))
    {
        TSBC_LOG_COND(!bAllowNull, Error, TEXT("Missing expected field '%s' in response body"), *FieldName);
    }
}

void CTSBC_EthGetTransactionReceipt::ParseResponseField(
    const TSharedPtr<FJsonObject>& Object,
    const FString& FieldName,
    int32& OutValue,
    const bool bAllowNull)
{
    FString StringField = "";
    ParseResponseField(Object, FieldName, StringField, bAllowNull);

    OutValue = FParse::HexNumber(ToCStr(StringField));
}

void CTSBC_EthGetTransactionReceipt::ParseResponseField(
    const TSharedPtr<FJsonObject>& Object,
    const FString& FieldName,
    int64& OutValue,
    const bool bAllowNull)
{
    FString StringField = "";
    ParseResponseField(Object, FieldName, StringField, bAllowNull);

    OutValue = FParse::HexNumber64(ToCStr(StringField));
}

void CTSBC_EthGetTransactionReceipt::ParseResponseField(
    const TSharedPtr<FJsonObject>& Object,
    const FString& FieldName,
    FTSBC_uint256& OutValue,
    const bool bAllowNull)
{
    FString StringField = "";
    ParseResponseField(Object, FieldName, StringField, bAllowNull);

    if(!FTSBC_uint256::ParseFromHexString(StringField, OutValue))
    {
        TSBC_LOG(
            Error,
            TEXT("Could not parse field '%s' in response: '%s'"),
            *FieldName,
            *StringField);
    }
}

void CTSBC_EthGetTransactionReceipt::ParseResponseField(
    const TSharedPtr<FJsonObject>& Object,
    const FString& FieldName,
    ETSBC_EthTransactionType& OutValue,
    const bool bAllowNull)
{
    FString StringField = "";
    ParseResponseField(Object, FieldName, StringField, bAllowNull);

    uint8 Type = static_cast<uint8>(FParse::HexNumber(ToCStr(StringField)));
    if(Type >= static_cast<uint8>(ETSBC_EthTransactionType::MAX))
    {
        TSBC_LOG(Error, TEXT("Could not parse field '%s' in response: '%s'"), *FieldName, *StringField);
        return;
    }

    OutValue = static_cast<ETSBC_EthTransactionType>(Type);
}

void CTSBC_EthGetTransactionReceipt::ParseResponseField(
    const TSharedPtr<FJsonObject>& Object,
    const FString& FieldName,
    TArray<FString>& OutValue,
    const bool bAllowNull)
{
    if(!Object->TryGetStringArrayField(FieldName, OutValue))
    {
        TSBC_LOG_COND(!bAllowNull, Error, TEXT("Missing expected array field '%s' in response body"), *FieldName)
    }
}

void CTSBC_EthGetTransactionReceipt::ParseResponseField(
    const TSharedPtr<FJsonObject>& Object,
    const FString& FieldName,
    TArray<FTSBC_EthLog>& OutValue,
    const bool bAllowNull)
{
    const TArray<TSharedPtr<FJsonValue>>* LogValues;
    if(!Object->TryGetArrayField(FieldName, LogValues))
    {
        TSBC_LOG_COND(!bAllowNull, Error, TEXT("Could not parse field '%s' in response"), *FieldName);
        return;
    }

    OutValue.Empty(LogValues->Num());

    for(const TSharedPtr<FJsonValue>& LogValue : *LogValues)
    {
        const TSharedPtr<FJsonObject>& LogObject = LogValue->AsObject();
        if(!LogObject.IsValid())
        {
            TSBC_LOG(Error, TEXT("'%s' field expected to find objects, but type was different"), *FieldName);
            return;
        }

        FTSBC_EthLog Log{};
        Log.FilterType = ETSBC_EthLogFilterType::General;

        ParseResponseField(LogObject, "removed", Log.bRemoved, false);
        ParseResponseField(LogObject, "logIndex", Log.LogIndex, false);
        ParseResponseField(LogObject, "transactionHash", Log.TransactionHash, false);
        ParseResponseField(LogObject, "transactionIndex", Log.TransactionIndex, false);
        ParseResponseField(LogObject, "blockHash", Log.BlockHash, false);
        ParseResponseField(LogObject, "blockNumber", Log.BlockNumber, false);
        ParseResponseField(LogObject, "address", Log.Address, false);
        ParseResponseField(LogObject, "data", Log.Data, false);
        ParseResponseField(LogObject, "topics", Log.Topics, false);

        OutValue.Add(Log);
    }
}