// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Encoding/TSBC_EncodingFunctionLibrary.h"
#include "Encoding/TSBC_Base58.h"
#include "Encoding/TSBC_RLP.h"

UTSBC_EncodingFunctionLibrary::UTSBC_EncodingFunctionLibrary()
{
}

FString UTSBC_EncodingFunctionLibrary::EncodeBase58(const TArray<uint8>& ByteArray)
{
    return CTSBC_Base58::Encode(ByteArray);
}

TArray<uint8> UTSBC_EncodingFunctionLibrary::DecodeBase58(const FString& Base58)
{
    return CTSBC_Base58::Decode(Base58);
}


TArray<uint8> UTSBC_EncodingFunctionLibrary::EncodeRLP(const FString& InHex)
{
    return UTSBC_RLP::Encode(InHex);
}

void UTSBC_EncodingFunctionLibrary::ParseAbiFromJson(
    bool& bSuccess,
    FString& ErrorMessage,
    const FString& ContractAbiJson,
    FTSBC_ContractAbi& ContractAbi)
{
    CTSBC_ContractAbi::ParseAbiFromJson(
        bSuccess,
        ErrorMessage,
        ContractAbiJson,
        ContractAbi);
}

void UTSBC_EncodingFunctionLibrary::EncodeAbi(
    bool& bSuccess,
    FString& ErrorMessage,
    const FTSBC_ContractAbi& ContractAbi,
    const FString& FunctionName,
    const TArray<FString>& FunctionArguments,
    FString& FunctionHashAndEncodedArguments)
{
    return CTSBC_ContractAbi::EncodeAbi(
        bSuccess,
        ErrorMessage,
        ContractAbi,
        FunctionName,
        FunctionArguments,
        FunctionHashAndEncodedArguments);
}

void UTSBC_EncodingFunctionLibrary::DecodeAbi(
    bool& bSuccess,
    FString& ErrorMessage,
    const FTSBC_ContractAbi& ContractAbi,
    const FString& FunctionName,
    FString DataToDecode,
    TArray<FString>& DecodedABIValues)
{
    return CTSBC_ContractAbi::DecodeAbi(
        bSuccess,
        ErrorMessage,
        ContractAbi,
        FunctionName,
        DataToDecode,
        DecodedABIValues);
}