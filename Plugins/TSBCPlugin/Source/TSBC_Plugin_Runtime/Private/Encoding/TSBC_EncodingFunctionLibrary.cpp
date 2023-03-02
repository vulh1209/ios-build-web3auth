// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Encoding/TSBC_EncodingFunctionLibrary.h"

#include "Encoding/TSBC_ContractAbiParsing.h"
#include "Encoding/TSBC_Base58.h"
#include "Encoding/TSBC_ContractAbiDecoding.h"
#include "Encoding/TSBC_ContractAbiEncoding.h"

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
    CTSBC_ContractAbiParsing::ParseAbiFromJson(
        ContractAbiJson,
        bSuccess,
        ErrorMessage,
        ContractAbi);
}

void UTSBC_EncodingFunctionLibrary::EncodeAbi(
    bool& bSuccess,
    FString& ErrorMessage,
    const UTSBC_ContractAbiDataAsset* ContractAbiDataAsset,
    const FString& FunctionName,
    const TArray<FTSBC_SolidityValueList>& FunctionArguments,
    FString& FunctionHashAndEncodedArguments)
{
    return CTSBC_ContractAbiEncoding::EncodeAbi(
        bSuccess,
        ErrorMessage,
        ContractAbiDataAsset->ContractAbi,
        FunctionName,
        FunctionArguments,
        FunctionHashAndEncodedArguments);
}

void UTSBC_EncodingFunctionLibrary::DecodeAbi(
    bool& bSuccess,
    FString& ErrorMessage,
    const UTSBC_ContractAbiDataAsset* ContractAbiDataAsset,
    const FString& FunctionName,
    const FString& DataToDecode,
    TArray<FTSBC_SolidityValueList>& DecodedAbiValues)
{
    return CTSBC_ContractAbiDecoding::DecodeAbi(
        bSuccess,
        ErrorMessage,
        ContractAbiDataAsset->ContractAbi,
        FunctionName,
        DataToDecode,
        DecodedAbiValues);
}