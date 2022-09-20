// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once

/**
 * This class implements the Recursive Length Prefix (RLP) algorithm to encode arbitrary data.
 *
 * Supported features:
 * - Encode an item (string)
 * - Encode a list of items (array)
 * 
 * Decoding of RLP-encoded data is not supported for now.
 */
class TSBC_PLUGIN_RUNTIME_API UTSBC_RLP
{
public:
    /**
     * RLP-encodes a hex input string.
     *
     * @param InHex Input string in hex representation.
     * @returns An array of bytes containing the RLP-encoded data.
     */
    static TArray<uint8> Encode(const FString& InHex);

    /**
     * RLP-encodes an array of hex input strings.
     *
     * @param InHexArray Input array of strings in hex representation.
     * @returns An array of bytes containing the RLP-encoded data.
     */
    static TArray<uint8> Encode(const TArray<FString>& InHexArray);

private:
    static uint32 BytesNeeded(int32 Number);
};