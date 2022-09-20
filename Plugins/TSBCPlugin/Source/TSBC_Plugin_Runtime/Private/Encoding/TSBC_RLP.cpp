// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Encoding/TSBC_RLP.h"
#include "Util/TSBC_StringUtils.h"

TArray<uint8> UTSBC_RLP::Encode(const FString& InHex)
{
    TArray<uint8> Result;

    const TArray<uint8> DataAsBytes = TSBC_StringUtils::HexStringToBytes(InHex);
    const int32 NumDataBytesToEncode = DataAsBytes.Num();

    // Encode 1 byte of data
    if(NumDataBytesToEncode == 1)
    {
        const uint8 Value = DataAsBytes[0];
        if(Value >= 0x80)
        {
            // This byte describes the length of encoded data
            Result.Add(NumDataBytesToEncode + 0x80);
        }

        // Append original byte of the data to encode
        Result.Add(Value);

        return Result;
    }

    // Encode 0 or between 1 and up to 55 bytes of data
    if(NumDataBytesToEncode == 0 || NumDataBytesToEncode <= 55)
    {
        // This byte describes the length of encoded data
        Result.Add(NumDataBytesToEncode + 0x80);

        // Append original bytes of the data to encode
        for(int8 i = 0; i < NumDataBytesToEncode; i++)
        {
            Result.Add(DataAsBytes[i]);
        }

        return Result;
    }

    // Encode more than 55 bytes of data

    // This byte describes how many bytes make up the length value for the encoded data
    const uint8 NumBytesForLength = BytesNeeded(NumDataBytesToEncode);
    Result.Add(NumBytesForLength + 0xB7);

    // These bytes describe the length of the encoded data
    const uint8* NumDataBytesToEncodeAsUInt8Ptr = reinterpret_cast<uint8*>(const_cast<int32*>(&NumDataBytesToEncode));
    for(int32 i = 0; i < NumBytesForLength; i++)
    {
        const uint8 Byte = NumDataBytesToEncodeAsUInt8Ptr[i];
        Result.Add(Byte);
    }

    // Append original bytes of the data to encode
    for(int32 i = 0; i < NumDataBytesToEncode; i++)
    {
        Result.Add(DataAsBytes[i]);
    }

    return Result;
}

TArray<uint8> UTSBC_RLP::Encode(const TArray<FString>& InHexArray)
{
    TArray<uint8> Result;

    // Encode all elements in hex array
    for(const FString& InHex : InHexArray)
    {
        Result.Append(Encode(InHex));
    }

    const uint8 NumDataBytesToEncode = Result.Num();

    // Encode between 0 and up to 55 bytes of data
    if(NumDataBytesToEncode <= 55)
    {
        Result.Insert(NumDataBytesToEncode + 0xC0, 0);
        return Result;
    }

    // Encode more than 55 bytes of data
    TArray<uint8> Tmp;

    // This byte describes how many bytes make up the length value for the encoded data
    constexpr uint8 NumBytesForLength = sizeof NumDataBytesToEncode;
    Tmp.Add(NumBytesForLength + 0xF7);

    // These bytes describe describe the length of the encoded data
    TArray<uint8> LengthBytes;
    LengthBytes.Init(0, sizeof NumDataBytesToEncode);
    FMemory::Memcpy(LengthBytes.GetData(), &NumDataBytesToEncode, sizeof NumDataBytesToEncode);
    for(uint8 Byte : LengthBytes)
    {
        Tmp.Add(Byte);
    }

    Result.Insert(Tmp, 0);

    return Result;
}

uint32 UTSBC_RLP::BytesNeeded(int32 Number)
{
    uint8 ByteCount = 0;
    while(Number > 0)
    {
        Number >>= 8;
        ByteCount++;
    }

    return ByteCount;
}