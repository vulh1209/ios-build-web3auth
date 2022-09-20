// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Math/TSBC_uint256.h"

#include "Crypto/Encryption/TSBC_EcdsaSecp256k1.h"
#include "Crypto/Random/TSBC_SecureRandom.h"
#include "Util/TSBC_StringUtils.h"

constexpr uint256_t FTSBC_uint256::MIN_VALUE = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

constexpr uint256_t FTSBC_uint256::MAX_VALUE = {
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
};

FTSBC_uint256::FTSBC_uint256()
{
#if WITH_EDITOR
    UpdateDebugValues();
#endif
}

FTSBC_uint256::FTSBC_uint256(const uint64& Value)
{
    SetValue(Value);

#if WITH_EDITOR
    UpdateDebugValues();
#endif
}

FTSBC_uint256::FTSBC_uint256(const uint256_t& Value)
{
    SetValue(Value);

#if WITH_EDITOR
    UpdateDebugValues();
#endif
}

FTSBC_uint256::FTSBC_uint256(const FTSBC_uint256& Value)
{
    SetValue(Value.CurrentValue);

#if WITH_EDITOR
    UpdateDebugValues();
#endif
}

FTSBC_uint256::FTSBC_uint256(const FString& Value)
{
    FTSBC_uint256 Tmp;
    if(Tmp.ParseFromString(Value))
    {
        SetValue(Tmp.CurrentValue);
    }

#if WITH_EDITOR
    UpdateDebugValues();
#endif
}

FTSBC_uint256& FTSBC_uint256::operator=(const uint64_t& Other)
{
    SetValue(Other);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256& FTSBC_uint256::operator=(const uint256_t& Other)
{
    SetValue(Other);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256& FTSBC_uint256::operator=(const FTSBC_uint256& Other)
{
    if(this == &Other)
    {
        return *this;
    }

    SetValueFromBytes(CurrentValue, Other.CurrentValue, NUM_WORDS);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256& FTSBC_uint256::operator=(const FString& Other)
{
    SetValue(FTSBC_uint256(Other));

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256 FTSBC_uint256::operator+(const uint64_t& Other) const
{
    uint256_t result = {0};
    Add(result, CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
    return result;
}

FTSBC_uint256 FTSBC_uint256::operator+(const uint256_t& Other) const
{
    uint256_t result = {0};
    Add(result, CurrentValue, Other, NUM_WORDS);
    return result;
}

FTSBC_uint256 FTSBC_uint256::operator+(const FTSBC_uint256& Other) const
{
    uint256_t result = {0};
    Add(result, CurrentValue, Other.CurrentValue, NUM_WORDS);
    return result;
}

FTSBC_uint256 FTSBC_uint256::operator+(const FString Other) const
{
    uint256_t result = {0};
    FTSBC_uint256 tmp;
    if(tmp.ParseFromString(Other))
    {
        Add(result, CurrentValue, tmp.CurrentValue, NUM_WORDS);
        return result;
    }

    return CurrentValue;
}

FTSBC_uint256& FTSBC_uint256::operator+=(const uint64_t& Other)
{
    uint256_t result = {0};
    Add(result, CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
    SetValue(result);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256& FTSBC_uint256::operator+=(const uint256_t& Other)
{
    uint256_t result = {0};
    Add(result, CurrentValue, Other, NUM_WORDS);
    SetValue(result);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256& FTSBC_uint256::operator+=(const FTSBC_uint256& Other)
{
    uint256_t result = {0};
    Add(result, CurrentValue, Other.CurrentValue, NUM_WORDS);
    SetValue(result);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256& FTSBC_uint256::operator+=(const FString& Other)
{
    uint256_t result = {0};
    Add(result, CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
    SetValue(result);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256 FTSBC_uint256::operator-(const uint64_t& Other) const
{
    uint256_t result = {0};
    Subtract(result, CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
    return result;
}

FTSBC_uint256 FTSBC_uint256::operator-(const uint256_t& Other) const
{
    uint256_t result = {0};
    Subtract(result, CurrentValue, Other, NUM_WORDS);
    return result;
}

FTSBC_uint256 FTSBC_uint256::operator-(const FTSBC_uint256& Other) const
{
    uint256_t result = {0};
    Subtract(result, CurrentValue, Other.CurrentValue, NUM_WORDS);
    return result;
}

FTSBC_uint256 FTSBC_uint256::operator-(const FString& Other) const
{
    uint256_t result = {0};
    Subtract(result, CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
    return result;
}

FTSBC_uint256& FTSBC_uint256::operator-=(const uint64_t& Other)
{
    uint256_t result = {0};
    Subtract(result, CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
    SetValue(result);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256& FTSBC_uint256::operator-=(const uint256_t& Other)
{
    uint256_t result = {0};
    Subtract(result, CurrentValue, Other, NUM_WORDS);
    SetValue(result);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256& FTSBC_uint256::operator-=(const FTSBC_uint256& Other)
{
    uint256_t result = {0};
    Subtract(result, CurrentValue, Other.CurrentValue, NUM_WORDS);
    SetValue(result);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256& FTSBC_uint256::operator-=(const FString& Other)
{
    uint256_t result = {0};
    Subtract(result, CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
    SetValue(result);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256 FTSBC_uint256::operator*(const uint64_t& Other) const
{
    uint256_t result = {0};
    ModMultiply(result, CurrentValue, FTSBC_uint256(Other).CurrentValue, MAX_VALUE, NUM_WORDS);
    return result;
}

FTSBC_uint256 FTSBC_uint256::operator*(const uint256_t& Other) const
{
    uint256_t result = {0};
    ModMultiply(result, CurrentValue, Other, MAX_VALUE, NUM_WORDS);
    return result;
}

FTSBC_uint256 FTSBC_uint256::operator*(const FTSBC_uint256& Other) const
{
    uint256_t result = {0};
    ModMultiply(result, CurrentValue, Other.CurrentValue, MAX_VALUE, NUM_WORDS);
    return result;
}

FTSBC_uint256 FTSBC_uint256::operator*(const FString& Other) const
{
    uint256_t result = {0};
    ModMultiply(result, CurrentValue, FTSBC_uint256(Other).CurrentValue, MAX_VALUE, NUM_WORDS);
    return result;
}

FTSBC_uint256& FTSBC_uint256::operator*=(const uint64_t& Other)
{
    uint256_t result = {0};
    ModMultiply(result, CurrentValue, FTSBC_uint256(Other).CurrentValue, MAX_VALUE, NUM_WORDS);
    SetValue(result);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256& FTSBC_uint256::operator*=(const uint256_t& Other)
{
    uint256_t result = {0};
    ModMultiply(result, CurrentValue, Other, MAX_VALUE, NUM_WORDS);
    SetValue(result);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256& FTSBC_uint256::operator*=(const FTSBC_uint256& Other)
{
    uint256_t result = {0};
    ModMultiply(result, CurrentValue, Other.CurrentValue, MAX_VALUE, NUM_WORDS);
    SetValue(result);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256& FTSBC_uint256::operator*=(const FString& Other)
{
    uint256_t result = {0};
    ModMultiply(result, CurrentValue, FTSBC_uint256(Other).CurrentValue, MAX_VALUE, NUM_WORDS);
    SetValue(result);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256 FTSBC_uint256::operator/(const uint64_t& Other) const
{
    uint256_t quotient = {0};
    Divide(quotient, nullptr, CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
    return quotient;
}

FTSBC_uint256 FTSBC_uint256::operator/(const uint256_t& Other) const
{
    uint256_t quotient = {0};
    Divide(quotient, nullptr, CurrentValue, Other, NUM_WORDS);
    return quotient;
}

FTSBC_uint256 FTSBC_uint256::operator/(const FTSBC_uint256& Other) const
{
    uint256_t quotient = {0};
    Divide(quotient, nullptr, CurrentValue, Other.CurrentValue, NUM_WORDS);
    return quotient;
}

FTSBC_uint256 FTSBC_uint256::operator/(const FString& Other) const
{
    uint256_t quotient = {0};
    Divide(quotient, nullptr, CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
    return quotient;
}

FTSBC_uint256& FTSBC_uint256::operator/=(const uint64_t& Other)
{
    uint256_t quotient = {0};
    Divide(quotient, nullptr, CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
    SetValue(quotient);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256& FTSBC_uint256::operator/=(const uint256_t& Other)
{
    uint256_t quotient = {0};
    Divide(quotient, nullptr, CurrentValue, Other, NUM_WORDS);
    SetValue(quotient);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256& FTSBC_uint256::operator/=(const FTSBC_uint256& Other)
{
    uint256_t quotient = {0};
    Divide(quotient, nullptr, CurrentValue, Other.CurrentValue, NUM_WORDS);
    SetValue(quotient);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256& FTSBC_uint256::operator/=(const FString& Other)
{
    uint256_t quotient = {0};
    Divide(quotient, nullptr, CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
    SetValue(quotient);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256 FTSBC_uint256::operator%(const uint64_t& Other) const
{
    uint256_t quotient = {0};
    uint256_t remainder = {0};
    Divide(quotient, remainder, CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
    return remainder;
}

FTSBC_uint256 FTSBC_uint256::operator%(const uint256_t& Other) const
{
    uint256_t quotient = {0};
    uint256_t remainder = {0};
    Divide(quotient, remainder, CurrentValue, Other, NUM_WORDS);
    return remainder;
}

FTSBC_uint256 FTSBC_uint256::operator%(const FTSBC_uint256& Other) const
{
    uint256_t quotient = {0};
    uint256_t remainder = {0};
    Divide(quotient, remainder, CurrentValue, Other.CurrentValue, NUM_WORDS);
    return remainder;
}

FTSBC_uint256 FTSBC_uint256::operator%(const FString& Other) const
{
    uint256_t quotient = {0};
    uint256_t remainder = {0};
    Divide(quotient, remainder, CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
    return remainder;
}

FTSBC_uint256& FTSBC_uint256::operator%=(const uint64_t& Other)
{
    uint256_t quotient = {0};
    uint256_t remainder = {0};
    Divide(quotient, remainder, CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
    SetValue(remainder);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256& FTSBC_uint256::operator%=(const uint256_t& Other)
{
    uint256_t quotient = {0};
    uint256_t remainder = {0};
    Divide(quotient, remainder, CurrentValue, Other, NUM_WORDS);
    SetValue(remainder);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256& FTSBC_uint256::operator%=(const FTSBC_uint256& Other)
{
    uint256_t quotient = {0};
    uint256_t remainder = {0};
    Divide(quotient, remainder, CurrentValue, Other.CurrentValue, NUM_WORDS);
    SetValue(remainder);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

FTSBC_uint256& FTSBC_uint256::operator%=(const FString& Other)
{
    uint256_t quotient = {0};
    uint256_t remainder = {0};
    Divide(quotient, remainder, CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
    SetValue(remainder);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return *this;
}

bool FTSBC_uint256::operator==(const uint64_t& Other) const
{
    return IsEqual(CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
}

bool FTSBC_uint256::operator==(const uint256_t& Other) const
{
    return IsEqual(CurrentValue, Other, NUM_WORDS);
}

bool FTSBC_uint256::operator==(const FTSBC_uint256& Other) const
{
    return IsEqual(CurrentValue, Other.CurrentValue, NUM_WORDS);
}

bool FTSBC_uint256::operator==(const FString& Other) const
{
    return IsEqual(CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
}

bool FTSBC_uint256::operator!=(const uint64_t& Other) const
{
    return !IsEqual(CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
}

bool FTSBC_uint256::operator!=(const uint256_t& Other) const
{
    return !IsEqual(CurrentValue, Other, NUM_WORDS);
}

bool FTSBC_uint256::operator!=(const FTSBC_uint256& Other) const
{
    return !IsEqual(CurrentValue, Other.CurrentValue, NUM_WORDS);
}

bool FTSBC_uint256::operator!=(const FString& Other) const
{
    return !IsEqual(CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
}

bool FTSBC_uint256::operator>(const uint64_t& Other) const
{
    return GreaterThan(CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
}

bool FTSBC_uint256::operator>(const uint256_t& Other) const
{
    return GreaterThan(CurrentValue, Other, NUM_WORDS);
}

bool FTSBC_uint256::operator>(const FTSBC_uint256& Other) const
{
    return GreaterThan(CurrentValue, Other.CurrentValue, NUM_WORDS);
}

bool FTSBC_uint256::operator>(const FString& Other) const
{
    return GreaterThan(CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
}

bool FTSBC_uint256::operator>=(const uint64_t& Other) const
{
    return GreaterOrEqual(CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
}

bool FTSBC_uint256::operator>=(const uint256_t& Other) const
{
    return GreaterOrEqual(CurrentValue, Other, NUM_WORDS);
}

bool FTSBC_uint256::operator>=(const FTSBC_uint256& Other) const
{
    return GreaterOrEqual(CurrentValue, Other.CurrentValue, NUM_WORDS);
}

bool FTSBC_uint256::operator>=(const FString& Other) const
{
    return GreaterOrEqual(CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
}

bool FTSBC_uint256::operator<(const uint64_t& Other) const
{
    return LessThan(CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
}

bool FTSBC_uint256::operator<(const uint256_t& Other) const
{
    return LessThan(CurrentValue, Other, NUM_WORDS);
}

bool FTSBC_uint256::operator<(const FTSBC_uint256& Other) const
{
    return LessThan(CurrentValue, Other.CurrentValue, NUM_WORDS);
}

bool FTSBC_uint256::operator<(const FString& Other) const
{
    return LessThan(CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
}

bool FTSBC_uint256::operator<=(const uint64_t& Other) const
{
    return LessOrEqual(CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
}

bool FTSBC_uint256::operator<=(const uint256_t& Other) const
{
    return LessOrEqual(CurrentValue, Other, NUM_WORDS);
}

bool FTSBC_uint256::operator<=(const FTSBC_uint256& Other) const
{
    return LessOrEqual(CurrentValue, Other.CurrentValue, NUM_WORDS);
}

bool FTSBC_uint256::operator<=(const FString& Other) const
{
    return LessOrEqual(CurrentValue, FTSBC_uint256(Other).CurrentValue, NUM_WORDS);
}

FString FTSBC_uint256::ToHexString(const bool bZeroPadded) const
{
    uint8 Bytes[NUM_BYTES] = {0};
    NativeToBytes(Bytes, NUM_BYTES, CurrentValue);

    FString Hex;
    for(int i = 0; i < NUM_BYTES; i++)
    {
        const uint8 Num = Bytes[i];
        const FString Tmp = FString::Printf(TEXT("%02x"), Num);
        if(Tmp == "00" && Hex.IsEmpty() && i < NUM_BYTES - 1)
        {
            continue;
        }
        Hex += Tmp;
    }

    if(!bZeroPadded)
    {
        if(Hex.GetCharArray()[0] == '0')
        {
            Hex.MidInline(1);
        }
    }

    return FString("0x") + Hex;
}

FString FTSBC_uint256::ToDecString(
    const uint32 Exponent,
    const int32 MinIntDigits,
    const int32 MinFracDigits,
    const int32 MaxFracDigits) const
{
    FString DecValueIntegral;
    FString DecValueFractional;

    // If there is no unit conversion required, we can use a shortcut and save CPU cycles.
    if(Exponent <= 0)
    {
        CTSBC_BaseConverter::Hex2DecConverter().Convert(ToHexString(), DecValueIntegral);
    }
    else
    {
        FTSBC_uint256 Quotient;
        FTSBC_uint256 Remainder;

        const FTSBC_uint256 Divisor = Pow(10, Exponent);
        if(!DivideGetQuotientRemainder(CurrentValue, Divisor, Quotient, Remainder))
        {
            return "";
        }

        CTSBC_BaseConverter::Hex2DecConverter().Convert(Quotient.ToHexString(), DecValueIntegral);
        CTSBC_BaseConverter::Hex2DecConverter().Convert(Remainder.ToHexString(), DecValueFractional);

        // Strip unnecessary zeroes from end
        if(Remainder == 0)
        {
            DecValueFractional = "";
        }
        else
        {
            TSBC_StringUtils::StripTrailingZeroes(DecValueFractional);
        }
    }

    if(DecValueIntegral.Len() < MinIntDigits)
    {
        DecValueIntegral = FString::ChrN(MinIntDigits - DecValueIntegral.Len(), '0') + DecValueIntegral;
    }

    if(MaxFracDigits <= 0)
    {
        DecValueFractional.Empty();
    }
    else
    {
        if(DecValueFractional.Len() < MinFracDigits)
        {
            DecValueFractional = DecValueFractional + FString::ChrN(MinFracDigits - DecValueFractional.Len(), '0');
        }
        else if(DecValueFractional.Len() > MaxFracDigits)
        {
            DecValueFractional.MidInline(0, MaxFracDigits);
        }
    }

    const FString DecValue = DecValueFractional.IsEmpty()
                             ? DecValueIntegral
                             : FString::Printf(TEXT("%s.%s"), *DecValueIntegral, *DecValueFractional);
    return DecValue;
}

FTSBC_uint256 FTSBC_uint256::Pow(const uint32 Base, const uint32 Exponent)
{
    if(Exponent == 0)
    {
        return 1;
    }

    if(Exponent == 1)
    {
        return Base;
    }

    // TODO Use LUT for commonly used base/exponent combinations (b: 2 and 10; e: 1 to 32)
    // TODO Or, even better: Use fast exponentiation algorithm

    FTSBC_uint256 Value = Base;
    for(uint32 i = 1; i < Exponent; i++)
    {
        Value = Value * Base;
    }

    return Value;
}

bool FTSBC_uint256::ParseFromString(const FString& Value)
{
    FString ValueSanitized = Value.TrimStartAndEnd().ToLower();
    if(ValueSanitized.IsEmpty())
    {
        return false;
    }

    FTSBC_uint256 ValueParsed;
    if(ValueSanitized.StartsWith("0x"))
    {
        ValueSanitized.MidInline(2);

        if(ValueSanitized.IsEmpty())
        {
            SetValue(0);
            return true;
        }

        const bool bSuccess = ParseFromHexString(ValueSanitized, ValueParsed);
        SetValue(ValueParsed);

        return bSuccess;
    }

    const bool bSuccess = ParseFromDecString(ValueSanitized, ValueParsed);
    SetValue(ValueParsed);

#if WITH_EDITOR
    UpdateDebugValues();
#endif

    return bSuccess;
}

bool FTSBC_uint256::ParseFromStringWithFractionalValue(const FString& Value, const int32 Exponent)
{
    FString ValueSanitized = Value.TrimStartAndEnd();

    // Try to find a dot and only one dot in the input string.
    // Also ensure that the input string only contains numeric characters besides the dot.
    bool bDotFound = false;
    int32 DotIndex = INDEX_NONE;
    for(int32 i = 0; i < ValueSanitized.Len(); i++)
    {
        if(ValueSanitized[i] == '.')
        {
            if(bDotFound)
            {
                // Error, more than one dot found!
                return false;
            }

            bDotFound = true;
            DotIndex = i;

            continue;
        }

        if(ValueSanitized[i] < '0' || ValueSanitized[i] > '9')
        {
            // Error, invalid characters!
            return false;
        }
    }

    if(!bDotFound)
    {
        // return ParseFromString(ValueSanitized);
        DotIndex = ValueSanitized.Len();
    }

    const FString ValueInt = TSBC_StringUtils::StripLeadingZeroes(ValueSanitized.Mid(0, DotIndex));
    const FString ValueFrac = TSBC_StringUtils::StripTrailingZeroes(
        ValueSanitized.Mid(DotIndex + 1, ValueSanitized.Len() - 1 - DotIndex));

    FString FinalValue;
    if(Exponent == 0)
    {
        // Truncate fractional part
        FinalValue = ValueInt;
    }
    else if(Exponent > 0)
    {
        // Shift dot to the right
        const int32 NumMissingDigits = Exponent - ValueFrac.Len();
        if(NumMissingDigits > 0)
        {
            // Add missing zeroes in fractional part and prepend integral part to it.
            FinalValue = ValueInt + ValueFrac + FString::ChrN(NumMissingDigits, '0');
        }
        else
        {
            // Concatenate integral part and fractional part (fully or partially).
            FinalValue = ValueInt + ValueFrac.Mid(0, Exponent);
        }
    }
    else
    {
        // Shift dot to the left
        const uint32 NumberOfDigitsToMove = Exponent + ValueInt.Len();
        const bool bOutOfIntegralDigits = NumberOfDigitsToMove < 0;
        if(bOutOfIntegralDigits)
        {
            // uint256 can only store integral values so we can set the value to zero.
            FinalValue = "0";
        }
        else
        {
            // Keep integral value (fully or partially).
            FinalValue = ValueInt.Mid(0, NumberOfDigitsToMove);
        }
    }

    return ParseFromString(FinalValue);
}

bool FTSBC_uint256::ParseFromHexString(const FString& HexAsString, FTSBC_uint256& DecAsUint256)
{
    FString ValueSanitized = HexAsString.TrimStartAndEnd();
    if(HexAsString.IsEmpty())
    {
        return false;
    }

    ValueSanitized.ToLowerInline();
    if(ValueSanitized.StartsWith("0x"))
    {
        ValueSanitized.MidInline(2);
    }

    if(!TSBC_StringUtils::IsHexString(ValueSanitized, false))
    {
        return false;
    }

    DecAsUint256 = 0;
    for(int32 i = 0; i < ValueSanitized.Len(); i++)
    {
        const char Hex = ValueSanitized[ValueSanitized.Len() - i - 1];
        // This statement expects that Hex contains lowercase hex digits only!
        const uint32 Value = Hex > 57
                             ? Hex - ('a' - 10)
                             : Hex - '0';
        FTSBC_uint256 Power = Pow(16, i);

        DecAsUint256 = DecAsUint256 + Power * Value;
    }

    return true;
}

bool FTSBC_uint256::ParseFromDecString(const FString& DecAsString, FTSBC_uint256& DecAsUint256)
{
    const CTSBC_BaseConverter BaseConverter = CTSBC_BaseConverter::Dec2HexConverter();

    FString HexAsString;
    if(!BaseConverter.Convert(DecAsString, HexAsString))
    {
        return false;
    }

    if(!ParseFromHexString(HexAsString, DecAsUint256))
    {
        return false;
    }

    return true;
}

bool FTSBC_uint256::DivideGetQuotientRemainder(
    const FTSBC_uint256& Dividend,
    const FTSBC_uint256& Divisor,
    FTSBC_uint256& Quotient,
    FTSBC_uint256& Remainder)
{
    uint256_t quotient = {0};
    uint256_t remainder = {0};
    if(!Divide(quotient, remainder, Dividend.CurrentValue, Divisor.CurrentValue, NUM_WORDS))
    {
        return false;
    }

    Quotient.SetValue(quotient);
    Remainder.SetValue(remainder);
    return true;
}

void FTSBC_uint256::SetValue(const uint64_t& NewValue)
{
    const int num_bits = 64;

    const int num_bytes = num_bits / 8;
    const uint8* otherPtr = (uint8*)&NewValue;
    uint8 bytes[num_bytes] = {0};
    for(int i = 0; i < num_bytes; i++)
    {
        bytes[num_bytes - i - 1] = otherPtr[i];
    }

    uint32 native[NUM_WORDS] = {0};
    BytesToNative(native, bytes, num_bytes);
    SetValueFromBytes(CurrentValue, native, NUM_WORDS);
}

void FTSBC_uint256::SetValue(const uint256_t& NewValue)
{
    SetValueFromBytes(CurrentValue, NewValue, NUM_WORDS);
}

void FTSBC_uint256::SetValue(const FTSBC_uint256& NewValue)
{
    SetValueFromBytes(CurrentValue, NewValue.CurrentValue, NUM_WORDS);
}

uint32* FTSBC_uint256::GetRawValue()
{
    return CurrentValue;
}

void FTSBC_uint256::SetValueFromBytes(uint32* Dest, const uint32* Src, const uint32 NumWords)
{
    for(uint32 i = 0; i < NumWords; ++i)
    {
        Dest[i] = Src[i];
    }
}

void FTSBC_uint256::ResetToZero(uint32* Vli, const uint32 NumWords)
{
    for(uint32 i = 0; i < NumWords; ++i)
    {
        Vli[i] = 0;
    }
}

bool FTSBC_uint256::IsZero(const uint32* Vli, const uint32 NumWords)
{
    uint32 bits = 0;
    for(uint32 i = 0; i < NumWords; ++i)
    {
        bits |= Vli[i];
    }
    return bits == 0;
}

bool FTSBC_uint256::IsBitSet(const uint32* Vli, uint8 Bit)
{
    return (Vli[Bit / 32] & 1u << (Bit & 31)) != 0;
}

uint32 FTSBC_uint256::GetNumDigits(const uint32* Vli, const int32 MaxWords)
{
    int32 i;
    for(i = MaxWords - 1; i >= 0 && Vli[i] == 0; --i)
    {
    }

    return i + 1;
}

uint32 FTSBC_uint256::GetNumBits(const uint32* Vli, const int32 MaxWords)
{
    uint32 i;

    const uint32 num_digits = GetNumDigits(Vli, MaxWords);
    if(num_digits == 0)
    {
        return 0;
    }

    uint32 digit = Vli[num_digits - 1];
    for(i = 0; digit; ++i)
    {
        digit /= 2;
    }

    return (num_digits - 1) * 32 + i;
}

int32 FTSBC_uint256::CompareUnsafe(const uint32* Left, const uint32* Right, const uint32 NumWords)
{
    for(int32 i = NumWords - 1; i >= 0; --i)
    {
        if(Left[i] > Right[i])
        {
            return 1;
        }

        if(Left[i] < Right[i])
        {
            return -1;
        }
    }

    return 0;
}

int32 FTSBC_uint256::Compare(const uint32* Left, const uint32* Right, const uint32 NumWords)
{
    uint32 tmp[8];
    const bool neg = Subtract(tmp, Left, Right, NumWords);
    const bool equal = IsZero(tmp, NumWords);

    return !equal - 2 * neg;
}

void FTSBC_uint256::RightShiftOne(uint32* Vli, const uint32 NumWords)
{
    uint32* end = Vli;
    uint32 carry = 0;

    Vli += NumWords;
    while(Vli-- > end)
    {
        const uint32 temp = *Vli;
        *Vli = (temp >> 1) | carry;
        carry = temp << (32 - 1);
    }
}

bool FTSBC_uint256::Add(
    uint32* Result,
    const uint32* Left,
    const uint32* Right,
    const uint32 NumWords)
{
    uint32 carry = 0;
    for(uint32 i = 0; i < NumWords; ++i)
    {
        const uint32 sum = Left[i] + Right[i] + carry;
        if(sum != Left[i])
        {
            carry = (sum < Left[i]) ? 1 : 0;
        }
        Result[i] = sum;
    }

    return carry != 0;
}

bool FTSBC_uint256::Subtract(
    uint32* Result,
    const uint32* Left,
    const uint32* Right,
    const uint32 NumWords)
{
    uint32 borrow = 0;
    for(uint32 i = 0; i < NumWords; ++i)
    {
        const uint32 diff = Left[i] - Right[i] - borrow;
        if(diff != Left[i])
        {
            borrow = (diff > Left[i]) ? 1 : 0;
        }
        Result[i] = diff;
    }

    return borrow != 0;
}

void FTSBC_uint256::Multiply(
    uint32* Result,
    const uint32* Left,
    const uint32* Right,
    const uint32 NumWords)
{
    uint64 k, t;
    uint32 i, j;

    for(i = 0; i < NumWords * 2; i++)
    {
        Result[i] = 0;
    }

    for(j = 0; j < NumWords; j++)
    {
        k = 0;
        for(i = 0; i < NumWords; i++)
        {
            t = static_cast<uint64>(Left[i]) * Right[j] + Result[i + j] + k;
            Result[i + j] = t;
            k = t >> 32;
        }
        Result[j + NumWords] = k;
    }
}

bool FTSBC_uint256::Divide(
    uint32* Quotient,
    uint32* Remainder,
    const uint32* Dividend,
    const uint32* Divisor,
    const uint32 NumWords)
{
    uint32 m = NumWords;
    uint32 n = NumWords;

    if(NumWords <= 0 || Divisor[NumWords - 1] == 0)
    {
        if(IsZero(Divisor, NumWords))
        {
            return false;
        }

        for(int32 i = NumWords - 1; i >= 0; --i)
        {
            if(Dividend[i] != 0)
            {
                m = i + 1;
                break;
            }
        }

        for(int32 i = NumWords - 1; i >= 0; --i)
        {
            if(Divisor[i] != 0)
            {
                n = i + 1;
                break;
            }
        }
    }

    const uint64 b = 4294967296LL;
    uint32* un,* vn;
    uint64 qhat;
    uint64 rhat;
    uint64 p;
    int64 t, k;
    uint32 s, i;
    int32 j;

    if(n == 1)
    {
        k = 0;
        for(int32 index = m - 1; index >= 0; index--)
        {
            Quotient[index] = (k * b + Dividend[index]) / Divisor[0];
            k = (k * b + Dividend[index]) - Quotient[index] * Divisor[0];
        }

        if(Remainder != nullptr)
        {
            Remainder[0] = k;
        }

        return true;
    }

    s = Normalize(Divisor[n - 1]);
    vn = (uint32*)alloca(4 * n);
    for(i = n - 1; i > 0; i--)
    {
        vn[i] = (Divisor[i] << s) | ((uint64)Divisor[i - 1] >> (32 - s));
    }
    vn[0] = Divisor[0] << s;

    un = (uint32*)alloca(4 * (m + 1));
    un[m] = (uint64)Dividend[m - 1] >> (32 - s);
    for(i = m - 1; i > 0; i--)
    {
        un[i] = (Dividend[i] << s) | ((uint64)Dividend[i - 1] >> (32 - s));
    }
    un[0] = Dividend[0] << s;

    for(j = m - n; j >= 0; j--)
    {
        qhat = (un[j + n] * b + un[j + n - 1]) / vn[n - 1];
        rhat = (un[j + n] * b + un[j + n - 1]) - qhat * vn[n - 1];
    again:
        if(qhat >= b || qhat * vn[n - 2] > b * rhat + un[j + n - 2])
        {
            qhat = qhat - 1;
            rhat = rhat + vn[n - 1];
            if(rhat < b)
            {
                goto again;
            }
        }
        k = 0;
        for(i = 0; i < n; i++)
        {
            p = qhat * vn[i];
            t = un[i + j] - k - (p & 0xFFFFFFFFLL);
            un[i + j] = t;
            k = (p >> 32) - (t >> 32);
        }
        t = un[j + n] - k;
        un[j + n] = t;

        Quotient[j] = qhat;
        if(t < 0)
        {
            Quotient[j] = Quotient[j] - 1;
            k = 0;
            for(i = 0; i < n; i++)
            {
                t = (uint64)un[i + j] + vn[i] + k;
                un[i + j] = t;
                k = t >> 32;
            }
            un[j + n] = un[j + n] + k;
        }
    }

    if(Remainder != nullptr)
    {
        for(i = 0; i < n - 1; i++)
        {
            Remainder[i] = (un[i] >> s) | ((uint64)un[i + 1] << (32 - s));
        }
        Remainder[n - 1] = un[n - 1] >> s;
    }

    return true;
}

void FTSBC_uint256::ModAdd(
    uint32* Result,
    const uint32* Left,
    const uint32* Right,
    const uint32* Mod,
    const uint32 NumWords)
{
    const bool carry = Add(Result, Left, Right, NumWords);
    if(carry || CompareUnsafe(Mod, Result, NumWords) != 1)
    {
        Subtract(Result, Result, Mod, NumWords);
    }
}

void FTSBC_uint256::ModSubtract(
    uint32* Result,
    const uint32* Left,
    const uint32* Right,
    const uint32* Mod,
    const uint32 NumWords)
{
    if(Subtract(Result, Left, Right, NumWords))
    {
        Add(Result, Result, Mod, NumWords);
    }
}

void FTSBC_uint256::MMod(uint32* Result, uint32* Product, const uint32* Mod, const uint32 NumWords)
{
    uint32 mod_multiple[2 * 8];
    uint32 tmp[2 * 8];
    uint32* v[2] = {tmp, Product};
    uint32 index;

    int32 shift = (NumWords * 2 * 32) - GetNumBits(Mod, NumWords);
    const int32 word_shift = shift / 32;
    const int32 bit_shift = shift % 32;
    ResetToZero(mod_multiple, word_shift);

    if(bit_shift > 0)
    {
        uint32 carry = 0;
        for(index = 0; index < static_cast<uint32>(NumWords); ++index)
        {
            mod_multiple[word_shift + index] = (Mod[index] << bit_shift) | carry;
            carry = Mod[index] >> (32 - bit_shift);
        }
    }
    else
    {
        SetValueFromBytes(mod_multiple + word_shift, Mod, NumWords);
    }

    for(index = 1; shift >= 0; --shift)
    {
        uint32 borrow = 0;
        for(uint32 i = 0; i < NumWords * 2; ++i)
        {
            const uint32 diff = v[index][i] - mod_multiple[i] - borrow;
            if(diff != v[index][i])
            {
                borrow = (diff > v[index][i]) ? 1 : 0;
            }

            v[1 - index][i] = diff;
        }

        index = ((index ^ borrow) == 0) ? 1 : 0;
        RightShiftOne(mod_multiple, NumWords);
        mod_multiple[NumWords - 1] |= mod_multiple[NumWords] << (32 - 1);
        RightShiftOne(mod_multiple + NumWords, NumWords);
    }

    SetValueFromBytes(Result, v[index], NumWords);
}

void FTSBC_uint256::ModMultiply(
    uint32* Result,
    const uint32* Left,
    const uint32* Right,
    const uint32* Mod,
    const uint32 NumWords)
{
    uint32 product[2 * 8];
    Multiply(product, Left, Right, NumWords);
    MMod(Result, product, Mod, NumWords);
}

void FTSBC_uint256::ModInverseUpdate(uint32* Uv, const uint32* Mod, const uint32 NumWords)
{
    bool carry = false;
    if(!(!(Uv[0] & 1)))
    {
        carry = Add(Uv, Uv, Mod, NumWords);
    }

    RightShiftOne(Uv, NumWords);
    if(carry)
    {
        Uv[NumWords - 1] |= 0x80000000;
    }
}

void FTSBC_uint256::ModInverse(
    uint32* Result,
    const uint32* Input,
    const uint32* Mod,
    const uint32 NumWords)
{
    uint32 a[8], b[8], u[8], v[8];
    int32 cmpResult;

    if(IsZero(Input, NumWords))
    {
        ResetToZero(Result, NumWords);
        return;
    }

    SetValueFromBytes(a, Input, NumWords);
    SetValueFromBytes(b, Mod, NumWords);
    ResetToZero(u, NumWords);
    u[0] = 1;
    ResetToZero(v, NumWords);

    while((cmpResult = CompareUnsafe(a, b, NumWords)) != 0)
    {
        if((!(a[0] & 1)))
        {
            RightShiftOne(a, NumWords);
            ModInverseUpdate(u, Mod, NumWords);
        }
        else if((!(b[0] & 1)))
        {
            RightShiftOne(b, NumWords);
            ModInverseUpdate(v, Mod, NumWords);
        }
        else if(cmpResult > 0)
        {
            Subtract(a, a, b, NumWords);
            RightShiftOne(a, NumWords);
            if(CompareUnsafe(u, v, NumWords) < 0)
            {
                Add(u, u, Mod, NumWords);
            }
            Subtract(u, u, v, NumWords);
            ModInverseUpdate(u, Mod, NumWords);
        }
        else
        {
            Subtract(b, b, a, NumWords);
            RightShiftOne(b, NumWords);
            if(CompareUnsafe(v, u, NumWords) < 0)
            {
                Add(v, v, Mod, NumWords);
            }
            Subtract(v, v, u, NumWords);
            ModInverseUpdate(v, Mod, NumWords);
        }
    }

    SetValueFromBytes(Result, u, NumWords);
}

void FTSBC_uint256::NativeToBytes(uint8* Bytes, const int32 NumBytes, const uint32* Native)
{
    for(int32 i = 0; i < NumBytes; ++i)
    {
        const uint32 b = NumBytes - 1 - i;
        Bytes[i] = Native[b / 4] >> (8 * (b % 4));
    }
}

void FTSBC_uint256::BytesToNative(uint32* Native, const uint8* Bytes, const int32 NumBytes)
{
    ResetToZero(Native, (NumBytes + (4 - 1)) / 4);
    for(int32 i = 0; i < NumBytes; ++i)
    {
        const uint32 b = NumBytes - 1 - i;
        Native[b / 4] |= static_cast<uint32>(Bytes[i]) << (8 * (b % 4));
    }
}

bool FTSBC_uint256::GetRandom(uint32* Random, const uint32* Top, const uint32 NumWords)
{
    constexpr uint32 mask = static_cast<uint32>(-1);
    const int32 num_bits = GetNumBits(Top, NumWords);

    for(uint32 tries = 0; tries < CTSBC_EcdsaSecp256k1::uECC_RNG_MAX_TRIES; ++tries)
    {
        CTSBC_SecureRandom::randombytes_sysrandom_buf((uint8*)Random, NumWords * 4);
        Random[NumWords - 1] &= mask >> (NumWords * 4 * 8 - num_bits);
        if(!IsZero(Random, NumWords) && Compare(Top, Random, NumWords) == 1)
        {
            return true;
        }
    }

    return false;
}

bool FTSBC_uint256::IsEqual(const uint32* Left, const uint32* Right, const uint32 NumWords)
{
    uint32 diff = 0;
    for(int32 i = NumWords - 1; i >= 0; --i)
    {
        diff |= Left[i] ^ Right[i];
    }

    return diff == 0;
}

bool FTSBC_uint256::GreaterOrEqual(const uint32* Left, const uint32* Right, const uint32 NumWords)
{
    return Compare(Left, Right, NumWords) >= 0;
}

bool FTSBC_uint256::LessOrEqual(const uint32* Left, const uint32* Right, const uint32 NumWords)
{
    return Compare(Left, Right, NumWords) <= 0;
}

bool FTSBC_uint256::GreaterThan(const uint32* Left, const uint32* Right, const uint32 NumWords)
{
    return Compare(Left, Right, NumWords) > 0;
}

bool FTSBC_uint256::LessThan(const uint32* Left, const uint32* Right, const uint32 NumWords)
{
    return Compare(Left, Right, NumWords) < 0;
}

int32 FTSBC_uint256::Normalize(uint32 Value)
{
    if(Value == 0)
    {
        return 32;
    }

    int32 n = 0;
    if(Value <= 0x0000FFFF)
    {
        n = n + 16;
        Value = Value << 16;
    }
    if(Value <= 0x00FFFFFF)
    {
        n = n + 8;
        Value = Value << 8;
    }
    if(Value <= 0x0FFFFFFF)
    {
        n = n + 4;
        Value = Value << 4;
    }
    if(Value <= 0x3FFFFFFF)
    {
        n = n + 2;
        Value = Value << 2;
    }
    if(Value <= 0x7FFFFFFF)
    {
        n = n + 1;
    }

    return n;
}