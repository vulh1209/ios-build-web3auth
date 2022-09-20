// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "TSBC_StringUtilsFunctionLibrary.generated.h"

/**
 * String Utils Function Library
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_StringUtilsFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * Converts the text string to a string containing the corresponding
     * character byte values written in two-digit lowercase hex format.
     *
     * @param TextString The text string to be converted to a hex string.
     * @param bWithPrefix If true, the hex code will be prefixed with "0x" (Default: True).
     * @returns Hex string of the input text string.
     */
    UFUNCTION(BlueprintPure, DisplayName = "String to Hex", Category = "3Studio|Utilities|String")
    static FString K2_StringToHex(const FString& TextString, const bool bWithPrefix = true);

    /**
     * Converts a hex string to an UTF-8 string.
     *
     * @param HexString The hex string to be converted to a text string.
     * @returns A text string created from the given hex string.
     */
    UFUNCTION(BlueprintPure, DisplayName = "Hex to String", Category = "3Studio|Utilities|String")
    static FString K2_HexToString(const FString& HexString);

    /**
     * Converts byte Array to a utf8 string.
     *
     * @param Bytes to be converted to a utf8 string.
     * @returns A utf8 string created from the given a bytes array.
     */
    UFUNCTION(BlueprintPure, DisplayName = "Bytes to String", Category = "3Studio|Utilities|String")
    static FString BytesToStringUtf8(const TArray<uint8>& Bytes);

    /**
     * Converts byte Array to a hex string.
     *
     * @param Bytes to be converted to a hex string.
     * @returns A hex string created from the given a bytes array.
     */
    UFUNCTION(BlueprintPure, DisplayName = "Bytes to Hex", Category = "3Studio|Utilities|String")
    static FString BytesToHex(const TArray<uint8>& Bytes);

    /**
     * Converts a hex string to byte Array.
     *
     * @param HexString to be converted to byte Array.
     * @returns byte Array created from the given a Hexstring.
     */
    UFUNCTION(BlueprintPure, DisplayName = "Hex to Bytes", Category = "3Studio|Utilities|String")
    static TArray<uint8> HexStringToBytes(const FString& HexString);

    /**
     * Converts a utf8 string to byte Array.
     *
     * @param String to be converted to byte Array.
     * @returns byte Array created from the given utf8 string.
     */
    UFUNCTION(BlueprintPure, DisplayName = "String to Bytes", Category = "3Studio|Utilities|String")
    static TArray<uint8> StringToBytesUtf8(const FString& String);
};