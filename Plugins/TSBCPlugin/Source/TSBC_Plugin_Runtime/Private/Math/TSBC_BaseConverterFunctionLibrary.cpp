// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Math/TSBC_BaseConverterFunctionLibrary.h"

#include "Math/TSBC_BaseConverter.h"

void UTSBC_BaseConverterFunctionLibrary::K2_ConvertBigInt(
    const FString& InValue,
    bool& bSuccess,
    FString& OutValue,
    const ETSBC_NumberBase SourceBase,
    const ETSBC_NumberBase TargetBase,
    const int32 MinDigits)
{
    bSuccess = CTSBC_BaseConverter::ConvertBigInt(
        InValue,
        OutValue,
        SourceBase,
        TargetBase,
        MinDigits);
}

void UTSBC_BaseConverterFunctionLibrary::K2_ConvertBigInt_Hex2Dec(
    const FString& InValue,
    bool& bSuccess,
    FString& OutValue,
    const int32 MinDigits)
{
    bSuccess = CTSBC_BaseConverter::ConvertBigInt_Hex2Dec(
        InValue,
        OutValue,
        MinDigits);
}

void UTSBC_BaseConverterFunctionLibrary::K2_ConvertBigInt_Dec2Hex(
    const FString& InValue,
    bool& bSuccess,
    FString& OutValue,
    const int32 MinDigits)
{
    bSuccess = CTSBC_BaseConverter::ConvertBigInt_Dec2Hex(
        InValue,
        OutValue,
        MinDigits);
}