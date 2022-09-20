// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Util/TSBC_StringUtilsFunctionLibrary.h"

#include "Util/TSBC_StringUtils.h"

FString UTSBC_StringUtilsFunctionLibrary::K2_StringToHex(const FString &TextString, const bool bWithPrefix)
{
    return TSBC_StringUtils::StringToHex(TextString, bWithPrefix);
}

FString UTSBC_StringUtilsFunctionLibrary::K2_HexToString(const FString &HexString)
{
    return TSBC_StringUtils::HexToString(HexString);
}

FString UTSBC_StringUtilsFunctionLibrary::BytesToStringUtf8(const TArray<uint8> &Bytes)
{
    return TSBC_StringUtils::BytesToStringUtf8(Bytes);
}

FString UTSBC_StringUtilsFunctionLibrary::BytesToHex(const TArray<uint8> &Bytes)
{
    return TSBC_StringUtils::BytesToHex(Bytes);
}

TArray<uint8> UTSBC_StringUtilsFunctionLibrary::HexStringToBytes(const FString &HexString)
{
    return TSBC_StringUtils::HexStringToBytes(HexString);
}

TArray<uint8> UTSBC_StringUtilsFunctionLibrary::StringToBytesUtf8(const FString &String)
{
    return TSBC_StringUtils::StringToBytesUtf8(String);
}