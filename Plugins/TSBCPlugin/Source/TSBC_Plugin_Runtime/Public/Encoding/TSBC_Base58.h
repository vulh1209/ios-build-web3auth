// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once

/**
 * This class implements Base58 encoding and decoding functions. 
 */
class TSBC_PLUGIN_RUNTIME_API CTSBC_Base58
{
private:
    const static TCHAR ALPHABET[];
    const static TCHAR ENCODED_ZERO;
    const static int32 INDEXES[128];

public:
    CTSBC_Base58();

    /**
     * Converts a byte array into a Base58 string.
     * 
     * @param ByteArray The byte array to convert.
     * @returns A converted byte array into a Base58 string.
     */
    static FString Encode(const TArray<uint8>& ByteArray);

    /**
     * Tries to convert a Base58 string into a byte array.
     *
     * @param Base58 The Base58-encoded string to convert.
     * @returns An empty array on failure.
     */
    static TArray<uint8> Decode(const FString& Base58);

private:
    static uint8 DivMod(TArray<uint8>& Number, const int32 FirstDigit, const int32 Base, const int32 Divisor);
};