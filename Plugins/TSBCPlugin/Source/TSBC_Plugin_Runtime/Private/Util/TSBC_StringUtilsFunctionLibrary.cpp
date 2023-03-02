// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Util/TSBC_StringUtilsFunctionLibrary.h"

#include "Util/TSBC_StringUtils.h"

FString UTSBC_StringUtilsFunctionLibrary::K2_StringToHex(const FString& TextString, const bool bWithPrefix)
{
    return TSBC_StringUtils::StringToHex(TextString, bWithPrefix);
}

FString UTSBC_StringUtilsFunctionLibrary::K2_HexToString(const FString& HexString)
{
    return TSBC_StringUtils::HexToString(HexString);
}

FString UTSBC_StringUtilsFunctionLibrary::BytesToString(const TArray<uint8>& Bytes)
{
    return TSBC_StringUtils::BytesToString(Bytes);
}

FString UTSBC_StringUtilsFunctionLibrary::BytesToHex(const TArray<uint8>& Bytes, const bool bWithPrefix)
{
    return TSBC_StringUtils::BytesToHex(Bytes, bWithPrefix);
}

TArray<uint8> UTSBC_StringUtilsFunctionLibrary::StringToBytes(const FString& String)
{
    return TSBC_StringUtils::StringToBytes(String);
}

bool UTSBC_StringUtilsFunctionLibrary::StringToBool(const FString& String)
{
    return String.ToBool();
}

TArray<uint8> UTSBC_StringUtilsFunctionLibrary::HexToBytes(const FString& HexString)
{
    return TSBC_StringUtils::HexToBytes(HexString);
}

