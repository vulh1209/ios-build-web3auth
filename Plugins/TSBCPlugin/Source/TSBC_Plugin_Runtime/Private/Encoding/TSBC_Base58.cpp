// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Encoding/TSBC_Base58.h"

// @formatter:off
constexpr TCHAR CTSBC_Base58::ALPHABET[] = TEXT("123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz");
constexpr TCHAR CTSBC_Base58::ENCODED_ZERO = ALPHABET[0];
constexpr int32 CTSBC_Base58::INDEXES[128] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8, -1, -1, -1, -1, -1, -1,
    -1,  9, 10, 11, 12, 13, 14, 15, 16, -1, 17, 18, 19, 20, 21, -1,
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, -1, -1, -1, -1, -1,
    -1, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, -1, 44, 45, 46,
    47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, -1, -1, -1, -1, -1
};
// @formatter:on

CTSBC_Base58::CTSBC_Base58()
{
}

uint8 CTSBC_Base58::DivMod(
    TArray<uint8>& Number,
    const int32 FirstDigit,
    const int32 Base,
    const int32 Divisor)
{
    int32 Remainder = 0;
    for(int32 i = FirstDigit; i < Number.Num(); i++)
    {
        const int32 Digit = static_cast<int>(Number[i]) & 0xFF;
        const int32 Temp = Remainder * Base + Digit;
        Number[i] = static_cast<uint8>(Temp / Divisor);
        Remainder = Temp % Divisor;
    }

    return static_cast<uint8>(Remainder);
}

FString CTSBC_Base58::Encode(const TArray<uint8>& ByteArray)
{
    if(ByteArray.Num() == 0)
    {
        return TEXT("");
    }

    int32 Zeros = 0;
    while(Zeros < ByteArray.Num() && ByteArray[Zeros] == 0)
    {
        ++Zeros;
    }

    TArray<uint8> Input = ByteArray;

    TArray<TCHAR> Encoded;
    Encoded.SetNum(Input.Num() * 2);

    int32 OutputStart = Encoded.Num();
    for(int32 InputStart = Zeros; InputStart < Input.Num();)
    {
        --OutputStart;
        Encoded[OutputStart] = ALPHABET[DivMod(Input, InputStart, 256, 58)];
        if(Input[InputStart] == 0)
        {
            ++InputStart;
        }
    }

    while(OutputStart < Encoded.Num() && Encoded[OutputStart] == ENCODED_ZERO)
    {
        ++OutputStart;
    }

    --Zeros;
    while(Zeros >= 0)
    {
        Encoded[--OutputStart] = ENCODED_ZERO;
        --Zeros;
    }

    return FString(Encoded.Num() - OutputStart, Encoded.GetData() + OutputStart);
}

TArray<uint8> CTSBC_Base58::Decode(const FString& Base58)
{
    if(Base58.Len() == 0)
    {
        return TArray<uint8>();
    }

    TArray<uint8> Input58;
    Input58.SetNum(Base58.Len());

    for(int32 i = 0; i < Base58.Len(); ++i)
    {
        const TCHAR c = Base58[i];
        const int32 Digit = c < 128 ? INDEXES[c] : -1;
        if(Digit < 0)
        {
            //throw new AddressFormatException.InvalidCharacter(c, i);
            return TArray<uint8>();
        }
        Input58[i] = static_cast<uint8>(Digit);
    }

    int32 zeros = 0;
    while(zeros < Input58.Num() && Input58[zeros] == 0)
    {
        ++zeros;
    }

    TArray<uint8> Decoded;
    Decoded.SetNum(Base58.Len());

    int32 OutputStart = Decoded.Num();
    for(int32 inputStart = zeros; inputStart < Input58.Num();)
    {
        --OutputStart;
        Decoded[OutputStart] = DivMod(Input58, inputStart, 58, 256);
        if(Input58[inputStart] == 0)
        {
            ++inputStart;
        }
    }

    while(OutputStart < Decoded.Num() && Decoded[OutputStart] == 0)
    {
        ++OutputStart;
    }

    TArray<uint8> Result;
    Result.Append(Decoded.GetData() + (OutputStart - zeros), Decoded.Num() - (OutputStart - zeros));

    return Result;
}