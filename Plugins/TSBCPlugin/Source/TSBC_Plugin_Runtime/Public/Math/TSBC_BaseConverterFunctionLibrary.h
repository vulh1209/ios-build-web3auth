// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Data/TSBC_Types.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "TSBC_BaseConverterFunctionLibrary.generated.h"

/**
 * This Blueprint Function Library implements wrapper functions that allow to
 * convert numbers of arbitrary length from a source number base to a target
 * number base.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_BaseConverterFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * Converts a value of arbitrary length in the source number base to the target number base.
     *
     * @param InValue Value in source number base.
     * @param bSuccess True, if conversion was successful.
     * @param OutValue Value in target number base.
     * @param SourceBase Source number base of the input value.
     * @param TargetBase Target number base of the output value.
     * @param MinDigits Minimum number of digits for returned value. (Default: 1)
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Convert BigInt",
        Category="3Studio|Math|Big Integer",
        Meta=(AdvancedDisplay="MinDigits"))
    static void K2_ConvertBigInt(
        const FString& InValue,
        bool& bSuccess,
        FString& OutValue,
        const ETSBC_NumberBase SourceBase = ETSBC_NumberBase::Hex,
        const ETSBC_NumberBase TargetBase = ETSBC_NumberBase::Dec,
        const int32 MinDigits = 1);

    /**
     * Converts a number of arbitrary length from hex to decimal.
     *
     * @param InValue Value in hex number base
     * @param bSuccess True, if conversion was successful.
     * @param OutValue Value in decimal number base
     * @param MinDigits Minimum number of digits for returned value. (Default: 1)
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Convert BigInt from Hex to Decimal",
        Category="3Studio|Math|Big Integer",
        Meta=(AdvancedDisplay="MinDigits"))
    static void K2_ConvertBigInt_Hex2Dec(
        const FString& InValue,
        bool& bSuccess,
        FString& OutValue,
        const int32 MinDigits = 1);

    /**
     * Converts a number of arbitrary length from decimal to hex.
     *
     * @param InValue Value in decimal number base
     * @param bSuccess True, if conversion was successful.
     * @param OutValue Value in hex number base
     * @param MinDigits Minimum number of digits for returned value. (Default: 1)
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Convert BigInt from Decimal to Hex",
        Category="3Studio|Math|Big Integer",
        Meta=(AdvancedDisplay="MinDigits"))
    static void K2_ConvertBigInt_Dec2Hex(
        const FString& InValue,
        bool& bSuccess,
        FString& OutValue,
        const int32 MinDigits = 1);
};