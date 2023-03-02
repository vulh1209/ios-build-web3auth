// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "Data/TSBC_Types.h"

/**
 * Converts unsigned integral numbers of arbitrary length from a source base to
 * a target base.
 */
class TSBC_PLUGIN_RUNTIME_API CTSBC_BaseConverter
{
public:
    static const TCHAR* CHARSET_BASE_BIN;
    static const TCHAR* CHARSET_BASE_OCT;
    static const TCHAR* CHARSET_BASE_DEC;
    static const TCHAR* CHARSET_BASE_HEX;

private:
    FString _SourceCharset;
    FString _TargetCharset;

public:
    /**
     * Gets a base converter that converts numbers from a source base to a target base.
     *
     * @param SourceCharset Charset for source base
     * @param TargetCharset Charset for target base
     */
    CTSBC_BaseConverter(const FString& SourceCharset, const FString& TargetCharset);

    /**
     * Converts a value of arbitrary length in the source number base to the target number base.
     *
     * @param InValue Value in source number base.
     * @param OutValue Value in target number base.
     * @param SourceBase Source number base of the input value.
     * @param TargetBase Target number base of the output value.
     * @param MinDigits Minimum number of digits for returned value. (Default: 1)
     * @returns True, if conversion was successful.
     */
    static bool ConvertBigInt(
        const FString& InValue,
        FString& OutValue,
        const ETSBC_NumberBase SourceBase,
        const ETSBC_NumberBase TargetBase,
        const int32 MinDigits = 1);

    /**
     * Converts a number of arbitrary length from decimal to hex.
     *
     * @param InValue Value in decimal number base
     * @param OutValue Value in hex number base
     * @param MinDigits Minimum number of digits for returned value. (Default: 1)
     * @returns True, if conversion was successful.
     */
    static bool ConvertBigInt_Dec2Hex(
        const FString& InValue,
        FString& OutValue,
        const int32 MinDigits = 1);

    /**
     * Converts a number of arbitrary length from hex to decimal.
     *
     * @param InValue Value in hex number base
     * @param OutValue Value in decimal number base
     * @param MinDigits Minimum number of digits for returned value. (Default: 1)
     * @returns True, if conversion was successful.
     */
    static bool ConvertBigInt_Hex2Dec(
        const FString& InValue,
        FString& OutValue,
        const int32 MinDigits = 1);

    /**
     * Get a base converter that converts numbers from binary to octal.
     *
     * @returns Instance of a base converter from binary to octal.
     */
    static CTSBC_BaseConverter Bin2OctConverter();

    /**
     * Get a base converter that converts numbers from binary to decimal.
     *
     * @returns Instance of a base converter from binary to decimal.
     */
    static CTSBC_BaseConverter Bin2DecConverter();

    /**
     * Get a base converter that converts numbers from binary to hex.
     *
     * @returns Instance of a base converter from binary to hex.
     */
    static CTSBC_BaseConverter Bin2HexConverter();

    /**
     * Get a base converter that converts numbers from octal to binary.
     *
     * @returns Instance of a base converter from octal to binary.
     */
    static CTSBC_BaseConverter Oct2BinConverter();

    /**
     * Get a base converter that converts numbers from octal to decimal.
     *
     * @returns Instance of a base converter from octal to decimal.
     */
    static CTSBC_BaseConverter Oct2DecConverter();

    /**
     * Get a base converter that converts numbers from octal to hex.
     *
     * @returns Instance of a base converter from octal to hex.
     */
    static CTSBC_BaseConverter Oct2HexConverter();

    /**
     * Get a base converter that converts numbers from decimal to binary.
     *
     * @returns Instance of a base converter from decimal to binary.
     */
    static CTSBC_BaseConverter Dec2BinConverter();

    /**
     * Get a base converter that converts numbers from decimal to octal.
     *
     * @returns Instance of a base converter from decimal to octal.
     */
    static CTSBC_BaseConverter Dec2OctConverter();

    /**
     * Get a base converter that converts numbers from decimal to hex.
     *
     * @returns Instance of a base converter from decimal to hex.
     */
    static CTSBC_BaseConverter Dec2HexConverter();

    /**
     * Get a base converter that converts numbers from hex to binary.
     *
     * @returns Instance of a base converter from hex to binary.
     */
    static CTSBC_BaseConverter Hex2BinConverter();

    /**
     * Get a base converter that converts numbers from hex to octal.
     *
     * @returns Instance of a base converter from hex to octal.
     */
    static CTSBC_BaseConverter Hex2OctConverter();

    /**
     * Get a base converter that converts numbers from hex to decimal.
     *
     * @returns Instance of a base converter from hex to decimal.
     */
    static CTSBC_BaseConverter Hex2DecConverter();

    /**
     * Converts a value in the source number base to the target number base.
     *
     * @param InValue Value in source number base.
     * @param OutValue Value in target number base.
     * @param MinDigits Minimum number of digits for the returned value. (Default: 1)
     * @returns True, if conversion was successful.
     */
    bool Convert(
        const FString& InValue,
        FString& OutValue,
        const int32 MinDigits = 1) const;

    /**
     * Converts a decimal value to the target base.
     *
     * @param Value Decimal value.
     * @returns Result in target base.
     */
    FString FromDecimal(const uint32 Value) const;

    /**
     * Converts a decimal value to the target base.
     *
     * @param Value Decimal value.
     * @param MinDigits Minimum number of digits for returned value.
     * @returns Result in target base.
     */
    FString FromDecimal(const uint32 Value, const int32 MinDigits) const;

    /**
     * Converts a value in source base to a decimal.
     *
     * @param Value Value in source base.
     * @param OutDecimal The decimal value.
     * @returns True, if conversion was successful.
     */
    bool ToDecimal(const FString& Value, uint32& OutDecimal) const;

private:
    /**
     * Divides Value by TargetNumberBase and returns the quotient and remainder.
     *
     * @param BaseCharset The base charset to use for OutQuotient and OutRemainder.
     * @param TargetNumberBase Target number base that will be used as denominator for the division.
     * @param Value The value that will be used as numerator for the division.
     * @param OutQuotient The quotient in base digits.
     * @param OutRemainder The remainder of Value divided by TargetNumberBase.
     * @returns True, if division was successful.
     */
    static bool Divide(
        const FString& BaseCharset,
        uint32 TargetNumberBase,
        FString Value,
        FString& OutQuotient,
        uint32& OutRemainder);

    /**
     * Tries to convert the given string Value to a decimal value using the
     * provided base charset.
     *
     * @param SourceBaseCharset The base charset to use for the conversion.
     * @param Value The value to convert from base to decimal.
     * @param OutDecimal The converted decimal value.
     * @Returns True, if conversion was successful.
     */
    static bool Base2Dec(const FString& SourceBaseCharset, const FString& Value, uint32& OutDecimal);

    /**
     * Converts the decimal Value to another base as string using the given
     * TargetBaseCharset.
     *
     * @param TargetBaseCharset The base charset to use for the conversion.
     * @param Value The value to convert from decimal to target base.
     * @Returns Value in target base as string.
     */
    static FString Dec2Base(const FString& TargetBaseCharset, uint32 Value);

    FORCEINLINE static FString CharsetBaseFromEnum(const ETSBC_NumberBase NumberBase)
    {
        switch(NumberBase)
        {
        case ETSBC_NumberBase::Bin:
            {
                return CTSBC_BaseConverter::CHARSET_BASE_BIN;
            }

        case ETSBC_NumberBase::Oct:
            {
                return CTSBC_BaseConverter::CHARSET_BASE_OCT;
            }

        case ETSBC_NumberBase::Dec:
            {
                return CTSBC_BaseConverter::CHARSET_BASE_DEC;
            }

        case ETSBC_NumberBase::Hex:
            {
                return CTSBC_BaseConverter::CHARSET_BASE_HEX;
            }

        default:
            {
                checkNoEntry();
                return "";
            }
        }
    }
};