// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "TSBC_BaseConverter.h"
#include "Module/TSBC_PluginUserSettings.h"

#include "TSBC_uint256.generated.h"

using uint256_t = uint32[8];

/**
 * Implements a new uint256 data type; Unsigned Integer with 256 bit precision.
 *
 * Min value:
 * - Dec: 0
 * - Hex: 0x0
 * 
 * Max value (2^256 - 1):
 * - Dec: 115792089237316195423570985008687907853269984665640564039457584007913129639935
 * - Hex: 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
 *
 * Supports:
 * - Parse value from string:
 *   - As decimal, if input consists of valid decimal digits (0 to 9)
 *   - As hexadecimal, if input is prefixed with "0x" and the remaining part consists of valid hex digits (0 to 9, A to F).
 * - Convert uint256 value to string:
 *   - In decimal notation
 *   - In hexadecimal notation
 * - Handle value overflow and underflow
 * - Math Operations:
 *   - Addition          (+)
 *   - Subtraction       (-)
 *   - Multiplication    (*)
 *   - Division          (/)
 *   - Modulo            (%)
 * - Math Comparision:
 *   - Equal             (==)
 *   - Unequal           (!=)
 *   - Greater Than      (> )
 *   - Greater or Equal  (>=)
 *   - Less Than         (< )
 *   - Less or Equal     (<=)
 * - Debugging
 *   - If enabled via project settings, human-readable debug values can be calculated for easier debugging purposes. 
 */
USTRUCT(BlueprintType, DisplayName="uint256")
struct TSBC_PLUGIN_RUNTIME_API FTSBC_uint256
{
    GENERATED_BODY()

public:
    static constexpr uint32 NUM_BYTES = sizeof(uint256_t);
    static constexpr uint32 NUM_WORDS = NUM_BYTES / sizeof(uint32);
    static const uint256_t MIN_VALUE;
    static const uint256_t MAX_VALUE;

#if WITH_EDITORONLY_DATA
    UPROPERTY(VisibleAnywhere, DisplayName="Hex", Category="3Studio|Math|BigInteger")
    FString DebugValueHex;

    UPROPERTY(VisibleAnywhere, DisplayName="Dec", Category="3Studio|Math|BigInteger")
    FString DebugValueDec;
#endif

private:
    uint256_t CurrentValue = {0};

public:
    FTSBC_uint256();
    FTSBC_uint256(const uint64& Value);
    FTSBC_uint256(const uint256_t& Value);
    FTSBC_uint256(const FTSBC_uint256& Value);
    explicit FTSBC_uint256(const FString& Value);

    FTSBC_uint256& operator=(const uint64_t& Other);
    FTSBC_uint256& operator=(const uint256_t& Other);
    FTSBC_uint256& operator=(const FTSBC_uint256& Other);
    FTSBC_uint256& operator=(const FString& Other);

    FTSBC_uint256 operator+(const uint64_t& Other) const;
    FTSBC_uint256 operator+(const uint256_t& Other) const;
    FTSBC_uint256 operator+(const FTSBC_uint256& Other) const;
    FTSBC_uint256 operator+(const FString Other) const;

    FTSBC_uint256& operator+=(const uint64_t& Other);
    FTSBC_uint256& operator+=(const uint256_t& Other);
    FTSBC_uint256& operator+=(const FTSBC_uint256& Other);
    FTSBC_uint256& operator+=(const FString& Other);

    FTSBC_uint256 operator-(const uint64_t& Other) const;
    FTSBC_uint256 operator-(const uint256_t& Other) const;
    FTSBC_uint256 operator-(const FTSBC_uint256& Other) const;
    FTSBC_uint256 operator-(const FString& Other) const;

    FTSBC_uint256& operator-=(const uint64_t& Other);
    FTSBC_uint256& operator-=(const uint256_t& Other);
    FTSBC_uint256& operator-=(const FTSBC_uint256& Other);
    FTSBC_uint256& operator-=(const FString& Other);

    FTSBC_uint256 operator*(const uint64_t& Other) const;
    FTSBC_uint256 operator*(const uint256_t& Other) const;
    FTSBC_uint256 operator*(const FTSBC_uint256& Other) const;
    FTSBC_uint256 operator*(const FString& Other) const;

    FTSBC_uint256& operator*=(const uint64_t& Other);
    FTSBC_uint256& operator*=(const uint256_t& Other);
    FTSBC_uint256& operator*=(const FTSBC_uint256& Other);
    FTSBC_uint256& operator*=(const FString& Other);

    FTSBC_uint256 operator/(const uint64_t& Other) const;
    FTSBC_uint256 operator/(const uint256_t& Other) const;
    FTSBC_uint256 operator/(const FTSBC_uint256& Other) const;
    FTSBC_uint256 operator/(const FString& Other) const;

    FTSBC_uint256& operator/=(const uint64_t& Other);
    FTSBC_uint256& operator/=(const uint256_t& Other);
    FTSBC_uint256& operator/=(const FTSBC_uint256& Other);
    FTSBC_uint256& operator/=(const FString& Other);

    FTSBC_uint256 operator%(const uint64_t& Other) const;
    FTSBC_uint256 operator%(const uint256_t& Other) const;
    FTSBC_uint256 operator%(const FTSBC_uint256& Other) const;
    FTSBC_uint256 operator%(const FString& Other) const;

    FTSBC_uint256& operator%=(const uint64_t& Other);
    FTSBC_uint256& operator%=(const uint256_t& Other);
    FTSBC_uint256& operator%=(const FTSBC_uint256& Other);
    FTSBC_uint256& operator%=(const FString& Other);

    bool operator==(const uint64_t& Other) const;
    bool operator==(const uint256_t& Other) const;
    bool operator==(const FTSBC_uint256& Other) const;
    bool operator==(const FString& Other) const;

    bool operator!=(const uint64_t& Other) const;
    bool operator!=(const uint256_t& Other) const;
    bool operator!=(const FTSBC_uint256& Other) const;
    bool operator!=(const FString& Other) const;

    bool operator>(const uint64_t& Other) const;
    bool operator>(const uint256_t& Other) const;
    bool operator>(const FTSBC_uint256& Other) const;
    bool operator>(const FString& Other) const;

    bool operator>=(const uint64_t& Other) const;
    bool operator>=(const uint256_t& Other) const;
    bool operator>=(const FTSBC_uint256& Other) const;
    bool operator>=(const FString& Other) const;

    bool operator<(const uint64_t& Other) const;
    bool operator<(const uint256_t& Other) const;
    bool operator<(const FTSBC_uint256& Other) const;
    bool operator<(const FString& Other) const;

    bool operator<=(const uint64_t& Other) const;
    bool operator<=(const uint256_t& Other) const;
    bool operator<=(const FTSBC_uint256& Other) const;
    bool operator<=(const FString& Other) const;

    /**
     * Converts uint256 to String in hexadecimal notation.
     *
     * @param bZeroPadded When true, a leading zero may be added to ensure two digit hex codes per byte.
     * @returns Value in hexadecimal notation.
     */
    FString ToHexString(const bool bZeroPadded = true) const;

    /**
     * Converts uint256 to String in decimal notation.
     *
     * @param Exponent The exponent describes the number of digits to move the decimal point to the left or right.
     *                 A positive exponent moves the decimal point to the right while a negative exponent moves it to
     *                 the left.
     * @param MinIntDigits Minimum number of integral digits for the returned value.
     *                     Digits will be zero-padded if value uses less digits than the minimum specified. (Default: 1)
     * @param MinFracDigits Minimum number of fractional digits for the returned value.
     *                      Digits will be zero-padded if value uses less digits than the minimum specified. (Default: 0)
     * @param MaxFracDigits Maximum number of fractional digits for the returned value.
     *                      Truncates fractional part to specified number of digits. (Default: 30)
     * @returns Value in decimal notation.
     */
    FString ToDecString(
        const uint32 Exponent = 0,
        const int32 MinIntDigits = 1,
        const int32 MinFracDigits = 0,
        const int32 MaxFracDigits = 30) const;

    /**
     * @returns The result of the Base argument raised to the power of the Exponent argument.
     */
    static FTSBC_uint256 Pow(const uint32 Base, const uint32 Exponent);

    /**
     * Tries to parse the input string value.
     *
     * If the string value is prefixed with "0x" it will be interpreted as a hex value, otherwise as a decimal value.
     * 
     * @param Value The input value as string. Can be either in hex (prefixed with "0x") or decimal representation.
     * @returns True, if the input string value could be parsed.
     */
    bool ParseFromString(const FString& Value);

    /**
     * Tries to parse the input string while also expecting a fractional value.
     *
     * The input string must be a decimal value.
     * 
     * @param Value The input value as string. Can be either in hex (prefixed with "0x") or decimal representation.
     * @param Exponent The exponent describes the number of digits to move the decimal point to the left or right.
     *                 A positive exponent moves the decimal point to the right while a negative exponent moves it to
     *                 the left.
     * @returns True, if the input string value could be parsed.
     */
    bool ParseFromStringWithFractionalValue(const FString& Value, const int32 Exponent);

    /**
     * Tries to parse the input string value. The string value can be optionally prefixed with "0x".
     * 
     * @param HexAsString The input value as string in hex representation.
     * @param DecAsUint256 The parsed input value as big integer. 
     * @returns True, if the input string value could be parsed.
     */
    static bool ParseFromHexString(const FString& HexAsString, FTSBC_uint256& DecAsUint256);

    /**
     * Tries to parse the input string value.
     * 
     * @param DecAsString The input value as string in decimal representation.
     * @param DecAsUint256 The parsed input value as big integer. 
     * @returns True, if the input string value could be parsed.
     */
    static bool ParseFromDecString(const FString& DecAsString, FTSBC_uint256& DecAsUint256);

    /**
     * Executes a division operation that returns the quotient as well as the remainder.
     *
     * @param Dividend The value to be divided by the divisor.
     * @param Divisor The value to divide the dividend by.
     * @param Quotient The integral result after the division operation.
     * @param Remainder The fractional result after the division operation.
     */
    static bool DivideGetQuotientRemainder(
        const FTSBC_uint256& Dividend,
        const FTSBC_uint256& Divisor,
        FTSBC_uint256& Quotient,
        FTSBC_uint256& Remainder);

    /**
     * Sets a new value.
     *
     * @param NewValue The new value to set.
     */
    void SetValue(const uint64_t& NewValue);

    /**
     * Sets a new value.
     *
     * @param NewValue The new value to set.
     */
    void SetValue(const uint256_t& NewValue);

    /**
     * Sets a new value.
     *
     * @param NewValue The new value to set.
     */
    void SetValue(const FTSBC_uint256& NewValue);

    /**
     * Get access to the underlying data storing the current value.
     *
     * @returns Pointer to where the current value is stored.
     */
    uint32* GetRawValue();

private:
    static void SetValueFromBytes(uint32* Dest, const uint32* Src, const uint32 NumWords);

    static void ResetToZero(uint32* Vli, const uint32 NumWords);

    static bool IsZero(const uint32* Vli, const uint32 NumWords);

    static bool IsBitSet(const uint32* Vli, uint8 Bit);

    static uint32 GetNumDigits(const uint32* Vli, const int32 MaxWords);

    static uint32 GetNumBits(const uint32* Vli, const int32 MaxWords);

    static int32 CompareUnsafe(const uint32* Left, const uint32* Right, const uint32 NumWords);

    static int32 Compare(const uint32* Left, const uint32* Right, const uint32 NumWords);

    static void RightShiftOne(uint32* Vli, const uint32 NumWords);

    static bool Add(uint32* Result, const uint32* Left, const uint32* Right, const uint32 NumWords);

    static bool Subtract(uint32* Result, const uint32* Left, const uint32* Right, const uint32 NumWords);

    static void Multiply(uint32* Result, const uint32* Left, const uint32* Right, const uint32 NumWords);

    static bool Divide(
        uint32* Quotient,
        uint32* Remainder,
        const uint32* Dividend,
        const uint32* Divisor,
        const uint32 NumWords);

    static void ModAdd(
        uint32* Result,
        const uint32* Left,
        const uint32* Right,
        const uint32* Mod,
        const uint32 NumWords);

    static void ModSubtract(
        uint32* Result,
        const uint32* Left,
        const uint32* Right,
        const uint32* Mod,
        const uint32 NumWords);

    static void MMod(uint32* Result, uint32* Product, const uint32* Mod, const uint32 NumWords);

    static void ModMultiply(
        uint32* Result,
        const uint32* Left,
        const uint32* Right,
        const uint32* Mod,
        const uint32 NumWords);

    static void ModInverseUpdate(uint32* Uv, const uint32* Mod, const uint32 NumWords);

    static void ModInverse(uint32* Result, const uint32* Input, const uint32* Mod, const uint32 NumWords);

    static void NativeToBytes(uint8* Bytes, const int32 NumBytes, const uint32* Native);

    static void BytesToNative(uint32* Native, const uint8* Bytes, const int32 NumBytes);

    static bool GetRandom(uint32* Random, const uint32* Top, const uint32 NumWords);

    static bool IsEqual(const uint32* Left, const uint32* Right, const uint32 NumWords);

    static bool GreaterOrEqual(const uint32* Left, const uint32* Right, const uint32 NumWords);

    static bool LessOrEqual(const uint32* Left, const uint32* Right, const uint32 NumWords);

    static bool GreaterThan(const uint32* Left, const uint32* Right, const uint32 NumWords);

    static bool LessThan(const uint32* Left, const uint32* Right, const uint32 NumWords);

    static int32 Normalize(uint32 Value);

#if WITH_EDITOR
    void UpdateDebugValues()
    {
        const auto* Settings = UTSBC_PluginUserSettings::Get();
        if(!Settings || !Settings->bDebugUint256Values)
        {
            return;
        }

        DebugValueHex = ToHexString();
        CTSBC_BaseConverter::Hex2DecConverter().Convert(DebugValueHex, DebugValueDec);
    }
#endif
};