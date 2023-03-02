// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once

#ifndef TSBC_USE_CPP_REGEX_IMPL
#define TSBC_USE_CPP_REGEX_IMPL false
#endif

#if TSBC_USE_CPP_REGEX_IMPL
#include <regex>
#include <string>
#else
#include "Internationalization/Regex.h"
#endif

namespace TSBC_StringUtils
{
    const FString RE_HexString = "^[a-zA-Z0-9]+$";

    static bool RegexMatch(const FString& Input, const FString& Pattern, const bool bCaseSensitive = true)
    {
        bool bMatches;

#if TSBC_USE_CPP_REGEX_IMPL
            const std::string Str = TCHAR_TO_UTF8(*Input);
            const std::regex Pat = bCaseSensitive
                                   ? std::regex(TCHAR_TO_UTF8(*Pattern))
                                   : std::regex(TCHAR_TO_UTF8(*Pattern), std::regex_constants::icase);

            bMatches = std::regex_match(Str, Pat);
#else
        bMatches = FRegexMatcher(FRegexPattern(Pattern), Input).FindNext();
#endif

        return bMatches;
    }

    /**
     * Converts the hex string containing hexadecimal characters to a byte array.
     * The input string can be prefixed with "0x" which will be ignored during the conversion.
     *
     * @param HexString A string containing hex digits. A "0x" prefix is allowed.
     * @returns Byte array of the hex string. Will be empty if the input hex string is empty or the conversion failed.
     */
    FORCEINLINE static TArray<uint8> HexToBytes(const FString& HexString)
    {
        TArray<uint8> Buffer;

        // Ensure that the hex string:
        // 1. Does not contain whitespace characters.
        // 2. Does no longer contain an optional "0x" prefix.
        // 3. Every hex value is written in two digit form.
        // 4. Contains valid hexadecimal characters only.

        FString Tmp = HexString.TrimStartAndEnd();
        if(Tmp.IsEmpty())
        {
            return Buffer;
        }

        Tmp.RemoveFromStart("0x");
        if(Tmp.Len() % 2 != 0)
        {
            // Length is uneven so we assume that a leading zero is missing.
            Tmp = "0" + Tmp;
        }

        if(!RegexMatch(Tmp, RE_HexString))
        {
            // Input hex string contains invalid characters, abort.
            return Buffer;
        }

        const FString HexStringSanitized = Tmp;

        // Convert hex string to uint8 array
        for(int32 i = 0; i < HexStringSanitized.Len(); i += 2)
        {
            const FString HexDigits = HexStringSanitized.Mid(i, 2);
            const uint32 HexNumber = FParse::HexNumber(*HexDigits);
            const uint8 ByteValue = static_cast<uint8>(HexNumber);
            Buffer.Add(ByteValue);
        }

        return Buffer;
    }

    /**
     * Converts the hex string containing hexadecimal characters to an int32.
     * The input string can be prefixed with "0x" which will be ignored during the conversion.
     *
     * @param HexString A string containing hex digits. A "0x" prefix is allowed.
     * @returns -1 if the input hex string is empty or the conversion failed. Otherwise, int32 value >= 0.
     */
    FORCEINLINE static int32 HexStringToInt32(const FString& HexString)
    {
        FString Tmp = HexString.TrimStartAndEnd();
        if(Tmp.IsEmpty())
        {
            return -1;
        }

        Tmp.RemoveFromStart("0x");
        if(Tmp.Len() % 2 != 0)
        {
            // Length is uneven so we assume that a leading zero is missing.
            Tmp = "0" + Tmp;
        }

        if(!RegexMatch(Tmp, RE_HexString))
        {
            // Input hex string contains invalid characters, abort.
            return -1;
        }

        const FString HexStringSanitized = Tmp;

        const uint32 Value = FParse::HexNumber(ToCStr(HexStringSanitized));
        return Value;
    }

    /**
     * Converts an array of bytes to an UTF-8 string.
     *
     * @param Bytes The byte array to be converted to a string.
     * @returns A string created from the given bytes array..
     */
    FORCEINLINE static FString BytesToStringUtf8(const TArray<uint8>& Bytes)
    {
        int32 Index = 0;
        int32 Length = 0x7FFFFFFF;

        if(Bytes.Num() <= 0)
        {
            return TEXT("");
        }

        if(Index < 0)
        {
            Length += Index;
            Index = 0;
        }
        if(Length > Bytes.Num() - Index)
        {
            Length = Bytes.Num() - Index;
        }
        if(Length <= 0)
        {
            return TEXT("");
        }

        FString String = TEXT("");
        const FUTF8ToTCHAR Src = FUTF8ToTCHAR(reinterpret_cast<const ANSICHAR*>(Bytes.GetData() + Index), Length);
        String.AppendChars(Src.Get(), Src.Length());
        return String;
    }

    /**
     * Converts the text string to a string containing the corresponding
     * character byte values written in two-digit lowercase hex format.
     * 
     * @param TextString The text string to be converted to a hex string.
     * @param bWithPrefix If true, the hex code will be prefixed with "0x" (Default: True).
     * @returns Hex string of the input text string. 
     */
    FORCEINLINE static FString StringToHex(const FString& TextString, const bool& bWithPrefix = true)
    {
        FString Buffer = bWithPrefix ? "0x" : "";
        for(int32 i = 0; i < TextString.Len(); i++)
        {
            Buffer.Appendf(TEXT("%02x"), TextString[i]);
        }

        return Buffer;
    }

    /**
     * Converts a hex string to an UTF-8 string.
     *
     * @param HexString The hex string to be converted to a text string.
     * @returns A text string created from the given hex string.
     */
    FORCEINLINE static FString HexToString(const FString& HexString)
    {
        return BytesToStringUtf8(HexToBytes(HexString));
    }

    /**
     * Converts the byte array to a string containing the corresponding
     * character byte values written in two-digit lowercase hex format.
     * 
     * @param Bytes The bytes to be converted to a hex string.
     * @param bWithPrefix If true, the hex code will be prefixed with "0x" (Default: True).
     * @returns Hex string of the input byte array.
     */
    FORCEINLINE static FString BytesToHex(const TArray<uint8>& Bytes, const bool& bWithPrefix = true)
    {
        const FString Hex = ::BytesToHex(Bytes.GetData(), Bytes.Num()).ToLower();
        return bWithPrefix ? "0x" + Hex : Hex;
    }

    /**
     * Converts an UTF-8 string to an array of bytes.
     *
     * @param String The string to be converted to an array of bytes.
     * @returns An array of bytes of the input string.
     */
    FORCEINLINE static TArray<uint8> StringToBytesUtf8(const FString& String)
    {
        if(String.Len() <= 0)
        {
            return TArray<uint8>();
        }

        TArray<uint8> ByteArray;
        const FTCHARToUTF8 Src = FTCHARToUTF8(String.GetCharArray().GetData());
        ByteArray.Append((uint8*)Src.Get(), Src.Length());
        return ByteArray;
    }

    /**
     * Removes leading zeroes ("0") from the given string and returns the result as a new string.
     * If no leading zeroes are found, the original string will be returned.
     *
     * @param Input String where leading zeroes should be removed from.
     * @returns New string without leading zeroes.
     */
    FORCEINLINE static FString StripLeadingZeroes(const FString& Input)
    {
        if(!Input.StartsWith("0"))
        {
            return Input;
        }

        // Find number of zeroes that need to be stripped
        int32 Index = 0;
        while(Index < Input.Len() && Input[Index] == '0')
        {
            Index++;
        }

        // Strip all zeroes in one operation
        return Input.Mid(Index, Input.Len() - Index - 1);
    }

    /**
     * Removes trailing zeroes ("0") from then given string and returns the result as a new string.
     * If no trailing zeroes are found, the original string will be returned.
     *
     * @param Input String where trailing zeroes should be removed from.
     * @returns New string without trailing zeroes.
     */
    FORCEINLINE static FString StripTrailingZeroes(const FString& Input)
    {
        if(!Input.EndsWith("0"))
        {
            return Input;
        }

        // Find number of zeroes that need to be stripped
        int32 Index = Input.Len() - 1;
        while(Index >= 0 && Input[Index] == '0')
        {
            Index--;
        }

        // Strip all zeroes in one operation
        return Input.Mid(0, Index + 1);
    }

    /**
     * Checks if the HexString contains valid hex digits only.
     *
     * Make sure the input string does not contain any whitespaces.
     *
     * @param HexString The string to check.
     * @param bContains0xPrefix If true, the input string is expected to be prefixed with "0x".
     * @returns True, if no invalid characters have been found in the input string.
     */
    FORCEINLINE static bool IsHexString(const FString& HexString, const bool bContains0xPrefix)
    {
        return bContains0xPrefix
               ? RegexMatch(HexString, "^0x[a-fA-F0-9]+$")
               : RegexMatch(HexString, "^[a-fA-F0-9]+$");
    }

    /**
     * Add zeroes ("0") to the left of the given string and returns the result as a new string.
     *
     * @param Input String where zeroes should be added to.
     * @param NumberOfDigits Number of zero digits to add.
     * @returns New zero-padded string.
     */
    FORCEINLINE static FString ZeroPadLeft(const FString& Input, const uint32& NumberOfDigits)
    {
        const int32 Len = NumberOfDigits - Input.Len();
        if(Len <= 0)
        {
            return Input;
        }
        return FString::ChrN(Len,TEXT('0')).Append(Input);
    }

    /**
     * Add zeroes ("0") to the right of the given string and returns the result as a new string.
     *
     * @param Input String where zeroes should be added to.
     * @param NumberOfDigits Number of zero digits to add.
     * @returns New zero-padded string.
     */
    FORCEINLINE static FString ZeroPadRight(FString Input, const uint32& NumberOfDigits)
    {
        const int32 Len = NumberOfDigits - Input.Len();
        if(Len <= 0)
        {
            return Input;
        }
        return Input.Append(FString::ChrN(Len,TEXT('0')));
    }

    /**
     * Tries to convert a byte array into string using UTF-8 encoding.
     * 
     * @param Bytes UTF8-encoded data.
     * @returns String from byte array; will be empty if conversion fails.
     */
    FORCEINLINE static FString BytesToString(const TArray<uint8>& Bytes)
    {
        if(Bytes.Num() <= 0)
        {
            return "";
        }

        const FUTF8ToTCHAR TCHARData(reinterpret_cast<const ANSICHAR*>(Bytes.GetData()), Bytes.Num());
        return FString(TCHARData.Length(), TCHARData.Get());
    }

    /**
     * Converts string into a byte array using UTF-8 encoding.
     * 
     * @param String String to convert.
     * @returns Byte array from string.
     */
    FORCEINLINE static TArray<uint8> StringToBytes(const FString& String)
    {
        if(String.Len() <= 0)
        {
            return TArray<uint8>();
        }

        TArray<uint8> ByteArray;
        const FTCHARToUTF8 Src = FTCHARToUTF8(String.GetCharArray().GetData());
        ByteArray.Append((uint8*)Src.Get(), Src.Length());
        return ByteArray;
    }
}