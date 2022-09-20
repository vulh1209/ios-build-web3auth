// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Math/TSBC_BaseConverter.h"

#include "Module/TSBC_RuntimeLogCategories.h"

const TCHAR* CTSBC_BaseConverter::CHARSET_BASE_BIN = TEXT("01");
const TCHAR* CTSBC_BaseConverter::CHARSET_BASE_OCT = TEXT("01234567");
const TCHAR* CTSBC_BaseConverter::CHARSET_BASE_DEC = TEXT("0123456789");
const TCHAR* CTSBC_BaseConverter::CHARSET_BASE_HEX = TEXT("0123456789ABCDEF");

CTSBC_BaseConverter::CTSBC_BaseConverter(const FString& SourceCharset, const FString& TargetCharset)
{
    _SourceCharset = SourceCharset.TrimStartAndEnd().ToUpper();
    _TargetCharset = TargetCharset.TrimStartAndEnd().ToUpper();

    checkf(
        !_SourceCharset.IsEmpty() && !_TargetCharset.IsEmpty(),
        TEXT("Invalid source and/or target base charset"));
}

bool CTSBC_BaseConverter::ConvertBigInt(
    const FString& InValue,
    FString& OutValue,
    const ETSBC_NumberBase SourceBase,
    const ETSBC_NumberBase TargetBase,
    const int32 MinDigits)
{
    const CTSBC_BaseConverter BaseConverter = CTSBC_BaseConverter(
        CharsetBaseFromEnum(SourceBase),
        CharsetBaseFromEnum(TargetBase));

    return BaseConverter.Convert(
        InValue,
        OutValue,
        MinDigits);
}

bool CTSBC_BaseConverter::ConvertBigInt_Dec2Hex(
    const FString& InValue,
    FString& OutValue,
    const int32 MinDigits)
{
    return CTSBC_BaseConverter::Dec2HexConverter().Convert(
        InValue,
        OutValue,
        MinDigits);
}

bool CTSBC_BaseConverter::ConvertBigInt_Hex2Dec(
    const FString& InValue,
    FString& OutValue,
    const int32 MinDigits)
{
    return CTSBC_BaseConverter::Hex2DecConverter().Convert(
        InValue,
        OutValue,
        MinDigits);
}

CTSBC_BaseConverter CTSBC_BaseConverter::Bin2OctConverter()
{
    return CTSBC_BaseConverter(CHARSET_BASE_BIN, CHARSET_BASE_OCT);
}

CTSBC_BaseConverter CTSBC_BaseConverter::Bin2DecConverter()
{
    return CTSBC_BaseConverter(CHARSET_BASE_BIN, CHARSET_BASE_DEC);
}

CTSBC_BaseConverter CTSBC_BaseConverter::Bin2HexConverter()
{
    return CTSBC_BaseConverter(CHARSET_BASE_BIN, CHARSET_BASE_HEX);
}

CTSBC_BaseConverter CTSBC_BaseConverter::Oct2BinConverter()
{
    return CTSBC_BaseConverter(CHARSET_BASE_OCT, CHARSET_BASE_BIN);
}

CTSBC_BaseConverter CTSBC_BaseConverter::Oct2DecConverter()
{
    return CTSBC_BaseConverter(CHARSET_BASE_OCT, CHARSET_BASE_DEC);
}

CTSBC_BaseConverter CTSBC_BaseConverter::Oct2HexConverter()
{
    return CTSBC_BaseConverter(CHARSET_BASE_OCT, CHARSET_BASE_HEX);
}

CTSBC_BaseConverter CTSBC_BaseConverter::Dec2BinConverter()
{
    return CTSBC_BaseConverter(CHARSET_BASE_DEC, CHARSET_BASE_BIN);
}

CTSBC_BaseConverter CTSBC_BaseConverter::Dec2OctConverter()
{
    return CTSBC_BaseConverter(CHARSET_BASE_DEC, CHARSET_BASE_OCT);
}

CTSBC_BaseConverter CTSBC_BaseConverter::Dec2HexConverter()
{
    return CTSBC_BaseConverter(CHARSET_BASE_DEC, CHARSET_BASE_HEX);
}

CTSBC_BaseConverter CTSBC_BaseConverter::Hex2BinConverter()
{
    return CTSBC_BaseConverter(CHARSET_BASE_HEX, CHARSET_BASE_BIN);
}

CTSBC_BaseConverter CTSBC_BaseConverter::Hex2OctConverter()
{
    return CTSBC_BaseConverter(CHARSET_BASE_HEX, CHARSET_BASE_OCT);
}

CTSBC_BaseConverter CTSBC_BaseConverter::Hex2DecConverter()
{
    return CTSBC_BaseConverter(CHARSET_BASE_HEX, CHARSET_BASE_DEC);
}

bool CTSBC_BaseConverter::Convert(
    const FString& InValue,
    FString& OutValue,
    const int32 MinDigits) const
{
    const uint32 TargetBase = _TargetCharset.Len();
    FString Value = InValue.TrimStartAndEnd().ToUpper();

    if(_SourceCharset == CHARSET_BASE_HEX)
    {
        if(Value.StartsWith("0x"))
        {
            Value.MidInline(2);
        }
    }

    FString Result;
    do
    {
        uint32 Remainder;
        FString Quotient;
        if(!Divide(
            _SourceCharset,
            TargetBase,
            Value,
            Quotient,
            Remainder))
        {
            return false;
        }

        Value = Quotient;
        Result.AppendChar(_TargetCharset[Remainder]);
    } while(!Value.IsEmpty() && !(Value.Len() == 1 && Value[0] == _SourceCharset[0]));

    OutValue = Result.Reverse();

    OutValue = OutValue.Len() < MinDigits
               ? FString::ChrN(MinDigits - OutValue.Len(), _TargetCharset[0]) + OutValue
               : OutValue;

    return true;
}

FString CTSBC_BaseConverter::FromDecimal(const uint32 Value) const
{
    return Dec2Base(_TargetCharset, Value);
}

FString CTSBC_BaseConverter::FromDecimal(const uint32 Value, const int32 MinDigits) const
{
    FString Result = FromDecimal(Value);

    return Result.Len() < MinDigits
           ? FString::ChrN(MinDigits - Result.Len(), _TargetCharset[0]) + Result
           : Result;
}

bool CTSBC_BaseConverter::ToDecimal(const FString& Value, uint32& OutDecimal) const
{
    uint32 Decimal;
    return Base2Dec(_SourceCharset, Value, Decimal);
}

bool CTSBC_BaseConverter::Divide(
    const FString& BaseCharset,
    const uint32 TargetNumberBase,
    FString Value,
    FString& OutQuotient,
    uint32& OutRemainder)
{
    FString Quotient;
    const int32 Length = Value.Len();
    for(int32 Index = 0; Index < Length; ++Index)
    {
        const int32 j = Index + 1 + Value.Len() - Length;
        if(Value.Len() < j)
        {
            break;
        }

        uint32 Decimal;
        if(!Base2Dec(BaseCharset, Value.Mid(0, j), Decimal))
        {
            return false;
        }

        Quotient.AppendChar(BaseCharset[Decimal / TargetNumberBase]);
        Value = Dec2Base(BaseCharset, Decimal % TargetNumberBase) + Value.Mid(j);
    }

    uint32 Remainder;
    if(!Base2Dec(BaseCharset, Value, Remainder))
    {
        return false;
    }

    // Remove all leading zeros from Quotient and put remaining string in OutRemainder
    int32 n = INDEX_NONE;
    for(int32 i = 0; i < Quotient.Len(); ++i)
    {
        if(Quotient[i] != BaseCharset[0])
        {
            n = i;
            break;
        }
    }
    if(n != INDEX_NONE)
    {
        OutQuotient = Quotient.Mid(n);
    }
    else
    {
        OutQuotient.Reset();
    }

    OutRemainder = Remainder;
    return true;
}

FString CTSBC_BaseConverter::Dec2Base(const FString& TargetBaseCharset, uint32 Value)
{
    const int32 NumberBase = TargetBaseCharset.Len();
    FString Result;
    do
    {
        Result.AppendChar(TargetBaseCharset[Value % NumberBase]);
        Value /= NumberBase;
    } while(Value > 0);

    return Result.Reverse();
}

bool CTSBC_BaseConverter::Base2Dec(const FString& SourceBaseCharset, const FString& Value, uint32& OutDecimal)
{
    const int32 NumberBase = SourceBaseCharset.Len();

    uint32 Result = 0;
    for(int32 i = 0; i < Value.Len(); ++i)
    {
        Result *= NumberBase;

        int32 CharAtIndex;
        if(!SourceBaseCharset.FindChar(Value[i], CharAtIndex))
        {
            TSBC_LOG(
                Error,
                TEXT(
                    "Invalid character '%c' in input value: "
                    "Cannot convert this value using source base charset '%s'."
                ),
                Value[i],
                *SourceBaseCharset);
            return false;
        }

        Result += CharAtIndex;
    }

    OutDecimal = Result;
    return true;
}