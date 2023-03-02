// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "TSBC_ByteUtilsFunctionLibrary.generated.h"

/**
 * Byte Utils Function Library
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_ByteUtilsFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * @returns True if the two arrays contain the same data.
     */
    UFUNCTION(
        BlueprintPure,
        DisplayName="Are Bytes Equal",
        Category="3Studio|Utilities|Bytes",
        Meta=(Keywords="same compare array ==", AdvancedDisplay="2"))
    static bool AreBytesEqual(
        const TArray<uint8>& A,
        const TArray<uint8>& B,
        int32 IndexA = 0,
        int32 LengthA = 0x7FFFFFFF,
        int32 IndexB = 0,
        int32 LengthB = 0x7FFFFFFF);

    /**
     * Splits the given Byte Array into two Byte Arrays.
     */
    UFUNCTION(
        BlueprintPure,
        DisplayName="Split Bytes",
        Category="3Studio|Utilities|Bytes",
        Meta=(Keywords="byte array"))
    static void SplitBytes(
        const TArray<uint8>& ByteArray,
        const int32 LengthA,
        TArray<uint8>& A,
        TArray<uint8>& B);

    /**
     * Merges multiple Byte Arrays into one.
     */
    UFUNCTION(
        BlueprintPure,
        DisplayName="Merge Bytes",
        Category="3Studio|Utilities|Bytes",
        Meta=(Keywords="byte array"))
    static TArray<uint8> MergeBytes(TArray<uint8> A, TArray<uint8> B);

    /**
     * Returns the values of the given bytes of the given index and length.
     */
    UFUNCTION(
        BlueprintPure,
        DisplayName="Get Bytes Sub Array",
        Category="3Studio|Utilities|Bytes",
        Meta=(Keywords="get byte array"))
    static TArray<uint8> BytesSubArray(
        const TArray<uint8>& ByteArray,
        const int32 Index,
        const int32 Length = 0x7FFFFFFF);
};