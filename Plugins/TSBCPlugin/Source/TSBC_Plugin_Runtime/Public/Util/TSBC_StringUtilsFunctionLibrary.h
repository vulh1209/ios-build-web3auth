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
    UFUNCTION(BlueprintPure, DisplayName="String to Hex", Category="3Studio|Utilities|String")
    static FString K2_StringToHex(const FString& TextString, const bool bWithPrefix = true);

    /**
     * Converts a hex string to an UTF-8 string.
     *
     * @param HexString The hex string to be converted to a text string.
     * @returns A text string created from the given hex string.
     */
    UFUNCTION(BlueprintPure, DisplayName="Hex to String", Category="3Studio|Utilities|String")
    static FString K2_HexToString(const FString& HexString);

    /**
     * Tries to convert a byte array into string using UTF-8 encoding.
     * 
     * @param Bytes UTF8-encoded data.
     * @returns String from byte array; will be empty if conversion fails.
     */
    UFUNCTION(BlueprintPure, Category="3Studio|Utilities|String")
    static FString BytesToString(const TArray<uint8>& Bytes);

    /**
     * Converts string into a byte array using UTF-8 encoding.
     * 
     * @param String String to convert.
     * @returns Byte array from string.
     */
    UFUNCTION(BlueprintPure, Category="3Studio|Utilities|String")
    static TArray<uint8> StringToBytes(const FString& String);

    /**
     * Converts a hex string into a byte array using UTF-8 encoding.
     * 
     * @param HexString The hex string to be converted to a text string.
     * @returns Byte array from hex string.
     */
    UFUNCTION(BlueprintPure, Category="3Studio|Utilities|String")
    static TArray<uint8> HexToBytes(const FString& HexString);

    /**
     * Converts the byte array to a string containing the corresponding
     * character byte values written in two-digit lowercase hex format.
     * 
     * @param Bytes Byte array.
     * @param bWithPrefix If true, the hex code will be prefixed with "0x" (Default: True).
     * @returns Hex string from byte array; will be empty if conversion fails.
     */
    UFUNCTION(BlueprintPure, Category="3Studio|Utilities|String")
    static FString BytesToHex(const TArray<uint8>& Bytes, const bool bWithPrefix = true);

    /**
     * Converts a string to a bool.
     *
     * @param String The string to be converted to a bool.
     * @returns A bool created from the given string.
     */
    UFUNCTION(BlueprintPure, DisplayName="String to Bool", Category="3Studio|Utilities|String")
    static bool StringToBool(const FString& String);
};