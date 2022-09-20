// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Math/TSBC_uint256FunctionLibrary.h"

void UTSBC_uint256FunctionLibrary::MakeLiteralUint256FromString(
    bool& bSuccess,
    FTSBC_uint256& OutValue,
    const FString& InValue)
{
    bSuccess = OutValue.ParseFromString(InValue);
}

void UTSBC_uint256FunctionLibrary::MakeLiteralUint256FromStringWithFractionalValue(
    bool& bSuccess,
    FTSBC_uint256& OutValue,
    const FString& InValue,
    const int32 Exponent)
{
    bSuccess = OutValue.ParseFromStringWithFractionalValue(InValue, Exponent);
}

FString UTSBC_uint256FunctionLibrary::Conv_Uint256ToString(const FTSBC_uint256& Value)
{
    FString DecValue;
    CTSBC_BaseConverter::Hex2DecConverter().Convert(Value.ToHexString(), DecValue);
    return DecValue;
}

FString UTSBC_uint256FunctionLibrary::ToDecString(
    const FTSBC_uint256& Value,
    const int32 Exponent,
    const int32 MinIntDigits,
    const int32 MinFracDigits,
    const int32 MaxFracDigits)
{
    if(Exponent < 0)
    {
        return "";
    }

    return Value.ToDecString(
        Exponent,
        MinIntDigits,
        MinFracDigits,
        MaxFracDigits);
}

FString UTSBC_uint256FunctionLibrary::ToHexString(const FTSBC_uint256& Value, const bool bZeroPadded)
{
    return Value.ToHexString(bZeroPadded);
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Pow(const int32 Base, const int32 Exponent)
{
    return FTSBC_uint256::Pow(Base, Exponent);
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Add_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B)
{
    return A + B;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Add_Uint256Byte(const FTSBC_uint256& A, const uint8 B)
{
    return A + B;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Add_Uint256Int(const FTSBC_uint256& A, const int32 B)
{
    const uint32 unsignedB = static_cast<uint32>(B);
    return A + unsignedB;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Add_Uint256Int64(const FTSBC_uint256& A, const int64 B)
{
    const uint64 unsignedB = static_cast<uint64>(B);
    return A + unsignedB;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Add_Uint256String(const FTSBC_uint256& A, const FString B)
{
    return A + B;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Subtract_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B)
{
    return A - B;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Subtract_Uint256Byte(const FTSBC_uint256& A, const uint8 B)
{
    return A - B;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Subtract_Uint256Int(const FTSBC_uint256& A, const int32 B)
{
    const uint32 unsignedB = static_cast<uint32>(B);
    return A - unsignedB;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Subtract_Uint256Int64(const FTSBC_uint256& A, const int64 B)
{
    const uint64 unsignedB = static_cast<uint64>(B);
    return A - unsignedB;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Subtract_Uint256String(const FTSBC_uint256& A, const FString B)
{
    return A - B;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Multiply_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B)
{
    return A * B;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Multiply_Uint256Byte(const FTSBC_uint256& A, const uint8 B)
{
    return A * B;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Multiply_Uint256Int(const FTSBC_uint256& A, const int32 B)
{
    const uint32 unsignedB = static_cast<uint32>(B);
    return A * unsignedB;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Multiply_Uint256Int64(const FTSBC_uint256& A, const int64 B)
{
    const uint64 unsignedB = static_cast<uint64>(B);
    return A * unsignedB;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Multiply_Uint256String(const FTSBC_uint256& A, const FString B)
{
    return A * B;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Divide_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B)
{
    return A / B;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Divide_Uint256Byte(const FTSBC_uint256& A, const uint8 B)
{
    return A / B;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Divide_Uint256Int(const FTSBC_uint256& A, const int32 B)
{
    const uint32 unsignedB = static_cast<uint32>(B);
    return A / unsignedB;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Divide_Uint256Int64(const FTSBC_uint256& A, const int64 B)
{
    const uint64 unsignedB = static_cast<uint64>(B);
    return A / unsignedB;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Divide_Uint256String(const FTSBC_uint256& A, const FString B)
{
    return A / B;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Percent_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B)
{
    return A % B;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Percent_Uint256Byte(const FTSBC_uint256& A, const uint8 B)
{
    return A % B;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Percent_Uint256Int(const FTSBC_uint256& A, const int32 B)
{
    const uint32 unsignedB = static_cast<uint32>(B);
    return A % unsignedB;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Percent_Uint256Int64(const FTSBC_uint256& A, const int64 B)
{
    const uint64 unsignedB = static_cast<uint64>(B);
    return A % unsignedB;
}

FTSBC_uint256 UTSBC_uint256FunctionLibrary::Percent_Uint256String(const FTSBC_uint256& A, const FString B)
{
    return A % B;
}

bool UTSBC_uint256FunctionLibrary::EqualEqual_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B)
{
    return A == B;
}

bool UTSBC_uint256FunctionLibrary::EqualEqual_Uint256Byte(const FTSBC_uint256& A, const uint8 B)
{
    return A == B;
}

bool UTSBC_uint256FunctionLibrary::EqualEqual_Uint256Int(const FTSBC_uint256& A, const int32 B)
{
    const uint32 unsignedB = static_cast<uint32>(B);
    return A == unsignedB;
}

bool UTSBC_uint256FunctionLibrary::EqualEqual_Uint256Int64(const FTSBC_uint256& A, const int64 B)
{
    const uint64 unsignedB = static_cast<uint64>(B);
    return A == unsignedB;
}

bool UTSBC_uint256FunctionLibrary::EqualEqual_Uint256String(const FTSBC_uint256& A, const FString B)
{
    return A == B;
}

bool UTSBC_uint256FunctionLibrary::NotEqual_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B)
{
    return A != B;
}

bool UTSBC_uint256FunctionLibrary::NotEqual_Uint256Byte(const FTSBC_uint256& A, const uint8 B)
{
    return A != B;
}

bool UTSBC_uint256FunctionLibrary::NotEqual_Uint256Int(const FTSBC_uint256& A, const int32 B)
{
    const uint32 unsignedB = static_cast<uint32>(B);
    return A != unsignedB;
}

bool UTSBC_uint256FunctionLibrary::NotEqual_Uint256Int64(const FTSBC_uint256& A, const int64 B)
{
    const uint64 unsignedB = static_cast<uint64>(B);
    return A != unsignedB;
}

bool UTSBC_uint256FunctionLibrary::NotEqual_Uint256String(const FTSBC_uint256& A, const FString B)
{
    return A != B;
}

bool UTSBC_uint256FunctionLibrary::Less_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B)
{
    return A < B;
}

bool UTSBC_uint256FunctionLibrary::Less_Uint256Byte(const FTSBC_uint256& A, const uint8 B)
{
    return A < B;
}

bool UTSBC_uint256FunctionLibrary::Less_Uint256Int(const FTSBC_uint256& A, const int32 B)
{
    const uint32 unsignedB = static_cast<uint32>(B);
    return A < unsignedB;
}

bool UTSBC_uint256FunctionLibrary::Less_Uint256Int64(const FTSBC_uint256& A, const int64 B)
{
    const uint64 unsignedB = static_cast<uint64>(B);
    return A < unsignedB;
}

bool UTSBC_uint256FunctionLibrary::Less_Uint256String(const FTSBC_uint256& A, const FString B)
{
    return A < B;
}

bool UTSBC_uint256FunctionLibrary::LessEqual_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B)
{
    return A <= B;
}

bool UTSBC_uint256FunctionLibrary::LessEqual_Uint256Byte(const FTSBC_uint256& A, const uint8 B)
{
    return A <= B;
}

bool UTSBC_uint256FunctionLibrary::LessEqual_Uint256Int(const FTSBC_uint256& A, const int32 B)
{
    const uint32 unsignedB = static_cast<uint32>(B);
    return A <= unsignedB;
}

bool UTSBC_uint256FunctionLibrary::LessEqual_Uint256Int64(const FTSBC_uint256& A, const int64 B)
{
    const uint64 unsignedB = static_cast<uint64>(B);
    return A <= unsignedB;
}

bool UTSBC_uint256FunctionLibrary::LessEqual_Uint256String(const FTSBC_uint256& A, const FString B)
{
    return A <= B;
}

bool UTSBC_uint256FunctionLibrary::Greater_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B)
{
    return A > B;
}

bool UTSBC_uint256FunctionLibrary::Greater_Uint256Byte(const FTSBC_uint256& A, const uint8 B)
{
    return A > B;
}

bool UTSBC_uint256FunctionLibrary::Greater_Uint256Int(const FTSBC_uint256& A, const int32 B)
{
    const uint32 unsignedB = static_cast<uint32>(B);
    return A > unsignedB;
}

bool UTSBC_uint256FunctionLibrary::Greater_Uint256Int64(const FTSBC_uint256& A, const int64 B)
{
    const uint64 unsignedB = static_cast<uint64>(B);
    return A > unsignedB;
}

bool UTSBC_uint256FunctionLibrary::Greater_Uint256String(const FTSBC_uint256& A, const FString B)
{
    return A > B;
}

bool UTSBC_uint256FunctionLibrary::GreaterEqual_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B)
{
    return A >= B;
}

bool UTSBC_uint256FunctionLibrary::GreaterEqual_Uint256Byte(const FTSBC_uint256& A, const uint8 B)
{
    return A >= B;
}

bool UTSBC_uint256FunctionLibrary::GreaterEqual_Uint256Int(const FTSBC_uint256& A, const int32 B)
{
    const uint32 unsignedB = static_cast<uint32>(B);
    return A >= unsignedB;
}

bool UTSBC_uint256FunctionLibrary::GreaterEqual_Uint256Int64(const FTSBC_uint256& A, const int64 B)
{
    const uint64 unsignedB = static_cast<uint64>(B);
    return A >= unsignedB;
}

bool UTSBC_uint256FunctionLibrary::GreaterEqual_Uint256String(const FTSBC_uint256& A, const FString B)
{
    return A >= B;
}
