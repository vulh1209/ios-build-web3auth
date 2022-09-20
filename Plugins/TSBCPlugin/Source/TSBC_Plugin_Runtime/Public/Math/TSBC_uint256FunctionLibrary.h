// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Math/TSBC_uint256.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "TSBC_uint256FunctionLibrary.generated.h"

/**
 * This Blueprint Function Library exposes the uint256 data type to Blueprints including functions to create, parse,
 * and convert uint256 values. Additionally, all available math operations and comparison functions are exposed as well. 
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_uint256FunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * Tries to parse the input string value.
     *
     * If the string value is prefixed with "0x" it will be interpreted as a hex value, otherwise as a decimal value.
     *
     * @param InValue The input value as string. Can be either in hex (prefixed with "0x") or decimal representation.
     * @param bSuccess True, if the input string value could be parsed.
     * @param OutValue The parsed uint256 value.
     */
    UFUNCTION(
        BlueprintPure,
        Category="3Studio|Math|uint256",
        DisplayName="Make Literal uint256 from String",
        Meta=(Keywords="parse"))
    static void MakeLiteralUint256FromString(
        bool& bSuccess,
        UPARAM(DisplayName="Value") FTSBC_uint256& OutValue,
        UPARAM(DisplayName="Value") const FString& InValue = "0");

    /**
     * Tries to parse the input string while also expecting a fractional value.
     * 
     * The input string must be a decimal value.
     * 
     * @param InValue The input value as string. Can be either in hex (prefixed with "0x") or decimal representation.
     * @param bSuccess True, if the input string value could be parsed.
     * @param Exponent The exponent describes the number of digits to move the decimal point to the left or right.
     *                 A positive exponent moves the decimal point to the right while a negative exponent moves it to
     *                 the left.
     * @param OutValue The parsed uint256 value.
     */
    UFUNCTION(
        BlueprintPure,
        Category="3Studio|Math|uint256",
        DisplayName="Make Literal uint256 from String with Fractional Value",
        Meta=(Keywords="parse"))
    static void MakeLiteralUint256FromStringWithFractionalValue(
        bool& bSuccess,
        UPARAM(DisplayName="Value") FTSBC_uint256& OutValue,
        UPARAM(DisplayName="Value") const FString& InValue = "0",
        const int32 Exponent = 0);

    /**
     * Converts uint256 to String in decimal notation.
     */
    UFUNCTION(
        BlueprintPure,
        Category="Utilities|String",
        Meta=(DisplayName = "uint256 to String", CompactNodeTitle = "->", BlueprintAutocast))
    static FString Conv_Uint256ToString(const FTSBC_uint256& Value);

    /**
     * Converts uint256 to String in decimal notation.
     *
     * @param Value The value to be converted.
     * @param Exponent The exponent describes the number of digits to move the decimal point to the left or right.
     *                 A positive exponent moves the decimal point to the right while a negative exponent moves it to
     *                 the left.
     * @param MinIntDigits Minimum number of integral digits for the returned value.
     *                     Digits will be zero-padded if value uses less digits than the minimum specified. (Default: 1)
     * @param MinFracDigits Minimum number of fractional digits for the returned value.
     *                      Digits will be zero-padded if value uses less digits than the minimum specified. (Default: 0)
     * @param MaxFracDigits Maximum number of fractional digits for the returned value.
     *                      Truncates fractional part to specified number of digits. (Default: 18)
     * @returns Value in decimal notation.
     */
    UFUNCTION(
        BlueprintPure,
        Category="3Studio|Math|uint256",
        Meta=(AdvancedDisplay="Exponent,MinIntDigits,MinFracDigits,MaxFracDigits"))
    static FString ToDecString(
        UPARAM(Ref) const FTSBC_uint256& Value,
        const int32 Exponent = 0,
        const int32 MinIntDigits = 1,
        const int32 MinFracDigits = 0,
        const int32 MaxFracDigits = 18);

    /**
     * Converts uint256 to String in hexadecimal notation.
     *
     * @param Value The value to be converted.
     * @param bZeroPadded When true, a leading zero may be added to ensure two digit hex codes per byte.
     * @returns Value in hexadecimal notation.
     */
    UFUNCTION(BlueprintPure, Category="3Studio|Math|uint256")
    static FString ToHexString(
        UPARAM(Ref) const FTSBC_uint256& Value,
        const bool bZeroPadded = true);

    /**
     * @returns The result of the Base argument raised to the power of the Exponent argument.
     */
    UFUNCTION(BlueprintPure, Category="3Studio|Math|uint256")
    static FTSBC_uint256 Pow(const int32 Base, const int32 Exponent);


    // =================================================================================================================
    // == Add
    // =================================================================================================================

    /**
     * Addition (A + B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 + uint256",
            CompactNodeTitle = "+",
            Keywords = "+ add plus",
            CommutativeAssociativeBinaryOperator = "true"))
    static FTSBC_uint256 Add_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B);

    /**
     * Addition (A + B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 + Byte",
            CompactNodeTitle = "+",
            Keywords = "+ add plus" ))
    static FTSBC_uint256 Add_Uint256Byte(const FTSBC_uint256& A, const uint8 B);

    /**
     * Addition (A + B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 + integer",
            CompactNodeTitle = "+",
            Keywords = "+ add plus" ))
    static FTSBC_uint256 Add_Uint256Int(const FTSBC_uint256& A, const int32 B);

    /**
     * Addition (A + B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 + integer64",
            CompactNodeTitle = "+",
            Keywords = "+ add plus" ))
    static FTSBC_uint256 Add_Uint256Int64(const FTSBC_uint256& A, const int64 B);

    /**
     * Addition (A + B), where B is a String
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 + String",
            CompactNodeTitle = "+",
            Keywords = "+ add plus" ))
    static FTSBC_uint256 Add_Uint256String(const FTSBC_uint256& A, const FString B);


    // =================================================================================================================
    // == Subtract
    // =================================================================================================================

    /**
     * Subtraction (A - B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 - uint256",
            CompactNodeTitle = "-",
            Keywords = "- subtract minus"))
    static FTSBC_uint256 Subtract_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B);

    /**
     * Subtraction (A - B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 - Byte",
            CompactNodeTitle = "-",
            Keywords = "- subtract minus" ))
    static FTSBC_uint256 Subtract_Uint256Byte(const FTSBC_uint256& A, const uint8 B);

    /**
     * Subtraction (A - B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 - integer",
            CompactNodeTitle = "-",
            Keywords = "- subtract minus" ))
    static FTSBC_uint256 Subtract_Uint256Int(const FTSBC_uint256& A, const int32 B);

    /**
     * Subtraction (A - B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 - integer64",
            CompactNodeTitle = "-",
            Keywords = "- subtract minus" ))
    static FTSBC_uint256 Subtract_Uint256Int64(const FTSBC_uint256& A, const int64 B);

    /**
     * Subtraction (A - B), where B is a String
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 - String",
            CompactNodeTitle = "-",
            Keywords = "- subtract minus" ))
    static FTSBC_uint256 Subtract_Uint256String(const FTSBC_uint256& A, const FString B);


    // =================================================================================================================
    // == Multiply
    // =================================================================================================================

    /**
     * Multiplication (A * B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 * uint256",
            CompactNodeTitle = "*",
            Keywords = "* multiply",
            CommutativeAssociativeBinaryOperator = "true"))
    static FTSBC_uint256 Multiply_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B);

    /**
     * Multiplication (A * B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 * Byte",
            CompactNodeTitle = "*",
            Keywords = "* multiply" ))
    static FTSBC_uint256 Multiply_Uint256Byte(const FTSBC_uint256& A, const uint8 B);

    /**
     * Multiplication (A * B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 * integer",
            CompactNodeTitle = "*",
            Keywords = "* multiply" ))
    static FTSBC_uint256 Multiply_Uint256Int(const FTSBC_uint256& A, const int32 B);

    /**
     * Multiplication (A * B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 * integer64",
            CompactNodeTitle = "*",
            Keywords = "* multiply" ))
    static FTSBC_uint256 Multiply_Uint256Int64(const FTSBC_uint256& A, const int64 B);

    /**
     * Multiplication (A * B), where B is a String
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 * String",
            CompactNodeTitle = "*",
            Keywords = "* multiply" ))
    static FTSBC_uint256 Multiply_Uint256String(const FTSBC_uint256& A, const FString B);


    // =================================================================================================================
    // == Divide
    // =================================================================================================================

    /**
     * Division (A / B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 / uint256",
            CompactNodeTitle = "/",
            Keywords = "/ divide division"))
    static FTSBC_uint256 Divide_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B);

    /**
     * Division (A / B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 / Byte",
            CompactNodeTitle = "/",
            Keywords = "/ divide division"))
    static FTSBC_uint256 Divide_Uint256Byte(const FTSBC_uint256& A, const uint8 B);

    /**
     * Division (A / B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 / integer",
            CompactNodeTitle = "/",
            Keywords = "/ divide division"))
    static FTSBC_uint256 Divide_Uint256Int(const FTSBC_uint256& A, const int32 B);

    /**
     * Division (A / B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 / integer64",
            CompactNodeTitle = "/",
            Keywords = "/ divide division"))
    static FTSBC_uint256 Divide_Uint256Int64(const FTSBC_uint256& A, const int64 B);

    /**
     * Division (A / B), where B is a String
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 / String",
            CompactNodeTitle = "/",
            Keywords = "/ divide division"))
    static FTSBC_uint256 Divide_Uint256String(const FTSBC_uint256& A, const FString B);


    // =================================================================================================================
    // == Modulo
    // =================================================================================================================

    /**
     * Modulo (A % B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 % uint256",
            CompactNodeTitle = "%",
            Keywords = "% modulus"))
    static FTSBC_uint256 Percent_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B);

    /**
     * Modulo (A % B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 % Byte",
            CompactNodeTitle = "%",
            Keywords = "% modulus"))
    static FTSBC_uint256 Percent_Uint256Byte(const FTSBC_uint256& A, const uint8 B);

    /**
     * Modulo (A % B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 % integer",
            CompactNodeTitle = "%",
            Keywords = "% modulus"))
    static FTSBC_uint256 Percent_Uint256Int(const FTSBC_uint256& A, const int32 B);

    /**
     * Modulo (A % B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 % integer64",
            CompactNodeTitle = "%",
            Keywords = "% modulus"))
    static FTSBC_uint256 Percent_Uint256Int64(const FTSBC_uint256& A, const int64 B);

    /**
     * Modulo (A % B), where B is a String
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 % String",
            CompactNodeTitle = "%",
            Keywords = "% modulus"))
    static FTSBC_uint256 Percent_Uint256String(const FTSBC_uint256& A, const FString B);


    // =================================================================================================================
    // == EqualEqual
    // =================================================================================================================

    /**
     * Returns true if A is equal to B (A == B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 == uint256",
            CompactNodeTitle = "==",
            Keywords = "== equal"))
    static bool EqualEqual_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B);

    /**
     * Returns true if A is equal to B (A == B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 == Byte",
            CompactNodeTitle = "==",
            Keywords = "== equal"))
    static bool EqualEqual_Uint256Byte(const FTSBC_uint256& A, const uint8 B);

    /**
     * Returns true if A is equal to B (A == B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 == integer",
            CompactNodeTitle = "==",
            Keywords = "== equal"))
    static bool EqualEqual_Uint256Int(const FTSBC_uint256& A, const int32 B);

    /**
     * Returns true if A is equal to B (A == B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 == integer64",
            CompactNodeTitle = "==",
            Keywords = "== equal"))
    static bool EqualEqual_Uint256Int64(const FTSBC_uint256& A, const int64 B);

    /**
     * Returns true if A is equal to B (A == B), where B is a String
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 == String",
            CompactNodeTitle = "==",
            Keywords = "== equal"))
    static bool EqualEqual_Uint256String(const FTSBC_uint256& A, const FString B);


    // =================================================================================================================
    // == NotEqual
    // =================================================================================================================

    /**
     * Returns true if A is not equal to B (A != B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 != uint256",
            CompactNodeTitle = "!=",
            Keywords = "!= not equal"))
    static bool NotEqual_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B);

    /**
     * Returns true if A is not equal to B (A != B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 != Byte",
            CompactNodeTitle = "!=",
            Keywords = "!= not equal"))
    static bool NotEqual_Uint256Byte(const FTSBC_uint256& A, const uint8 B);

    /**
     * Returns true if A is not equal to B (A != B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 != integer",
            CompactNodeTitle = "!=",
            Keywords = "!= not equal"))
    static bool NotEqual_Uint256Int(const FTSBC_uint256& A, const int32 B);

    /**
     * Returns true if A is not equal to B (A != B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 != integer64",
            CompactNodeTitle = "!=",
            Keywords = "!= not equal"))
    static bool NotEqual_Uint256Int64(const FTSBC_uint256& A, const int64 B);

    /**
     * Returns true if A is not equal to B (A != B), where B is a String
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 != String",
            CompactNodeTitle = "!=",
            Keywords = "!= not equal"))
    static bool NotEqual_Uint256String(const FTSBC_uint256& A, const FString B);


    // =================================================================================================================
    // == Less
    // =================================================================================================================

    /**
     * Returns true if A is less than B (A < B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 < uint256",
            CompactNodeTitle = "<",
            Keywords = "< less"))
    static bool Less_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B);

    /**
     * Returns true if A is less than B (A < B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 < Byte",
            CompactNodeTitle = "<",
            Keywords = "< less"))
    static bool Less_Uint256Byte(const FTSBC_uint256& A, const uint8 B);

    /**
     * Returns true if A is less than B (A < B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 < integer",
            CompactNodeTitle = "<",
            Keywords = "< less"))
    static bool Less_Uint256Int(const FTSBC_uint256& A, const int32 B);

    /**
     * Returns true if A is less than B (A < B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 < integer64",
            CompactNodeTitle = "<",
            Keywords = "< less"))
    static bool Less_Uint256Int64(const FTSBC_uint256& A, const int64 B);

    /**
     * Returns true if A is less than B (A < B), where B is a String
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 < String",
            CompactNodeTitle = "<",
            Keywords = "< less"))
    static bool Less_Uint256String(const FTSBC_uint256& A, const FString B);


    // =================================================================================================================
    // == LessEqual
    // =================================================================================================================

    /**
     * Returns true if A is less than or equal to B (A <= B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 <= uint256",
            CompactNodeTitle = "<=",
            Keywords = "<= less"))
    static bool LessEqual_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B);

    /**
     * Returns true if A is less than or equal to B (A <= B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 <= Byte",
            CompactNodeTitle = "<=",
            Keywords = "<= less"))
    static bool LessEqual_Uint256Byte(const FTSBC_uint256& A, const uint8 B);

    /**
     * Returns true if A is less than or equal to B (A <= B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 <= integer",
            CompactNodeTitle = "<=",
            Keywords = "<= less"))
    static bool LessEqual_Uint256Int(const FTSBC_uint256& A, const int32 B);

    /**
     * Returns true if A is less than or equal to B (A <= B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 <= integer64",
            CompactNodeTitle = "<=",
            Keywords = "<= less"))
    static bool LessEqual_Uint256Int64(const FTSBC_uint256& A, const int64 B);

    /**
     * Returns true if A is less than or equal to B (A <= B), where B is a String
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 <= String",
            CompactNodeTitle = "<=",
            Keywords = "<= less"))
    static bool LessEqual_Uint256String(const FTSBC_uint256& A, const FString B);


    // =================================================================================================================
    // == Greater
    // =================================================================================================================

    /**
     * Returns true if A is greater than B (A > B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 > uint256",
            CompactNodeTitle = ">",
            Keywords = "> greater"))
    static bool Greater_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B);

    /**
     * Returns true if A is greater than B (A > B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 > Byte",
            CompactNodeTitle = ">",
            Keywords = "> greater"))
    static bool Greater_Uint256Byte(const FTSBC_uint256& A, const uint8 B);

    /**
     * Returns true if A is greater than B (A > B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 > integer",
            CompactNodeTitle = ">",
            Keywords = "> greater"))
    static bool Greater_Uint256Int(const FTSBC_uint256& A, const int32 B);

    /**
     * Returns true if A is greater than B (A > B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 > integer64",
            CompactNodeTitle = ">",
            Keywords = "> greater"))
    static bool Greater_Uint256Int64(const FTSBC_uint256& A, const int64 B);

    /**
     * Returns true if A is greater than B (A > B), where B is a String
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 > String",
            CompactNodeTitle = ">",
            Keywords = "> greater"))
    static bool Greater_Uint256String(const FTSBC_uint256& A, const FString B);


    // =================================================================================================================
    // == GreaterEqual
    // =================================================================================================================

    /**
     * Returns true if A is greater than or equal to B (A >= B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 >= uint256",
            CompactNodeTitle = ">=",
            Keywords = ">= greater"))
    static bool GreaterEqual_Uint256Uint256(const FTSBC_uint256& A, const FTSBC_uint256& B);

    /**
     * Returns true if A is greater than or equal to B (A >= B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 >= Byte",
            CompactNodeTitle = ">=",
            Keywords = ">= greater"))
    static bool GreaterEqual_Uint256Byte(const FTSBC_uint256& A, const uint8 B);

    /**
     * Returns true if A is greater than or equal to B (A >= B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 >= integer",
            CompactNodeTitle = ">=",
            Keywords = ">= greater"))
    static bool GreaterEqual_Uint256Int(const FTSBC_uint256& A, const int32 B);

    /**
     * Returns true if A is greater than or equal to B (A >= B)
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 >= integer64",
            CompactNodeTitle = ">=",
            Keywords = ">= greater"))
    static bool GreaterEqual_Uint256Int64(const FTSBC_uint256& A, const int64 B);

    /**
     * Returns true if A is greater than or equal to B (A >= B), where B is a String
     */
    UFUNCTION(
        BlueprintPure,
        Category="Math|BigInteger",
        Meta=(
            DisplayName = "uint256 >= String",
            CompactNodeTitle = ">=",
            Keywords = ">= greater"))
    static bool GreaterEqual_Uint256String(const FTSBC_uint256& A, const FString B);
};