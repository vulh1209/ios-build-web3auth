// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Util/TSBC_ByteUtilsFunctionLibrary.h"

#include "Util/TSBC_ByteUtils.h"

bool UTSBC_ByteUtilsFunctionLibrary::AreBytesEqual(
    const TArray<uint8>& A,
    const TArray<uint8>& B,
    int32 IndexA,
    int32 LengthA,
    int32 IndexB,
    int32 LengthB)
{
    return TSBC_ByteUtils::AreBytesEqual(A, B, IndexA, LengthA, IndexB, LengthB);
}

void UTSBC_ByteUtilsFunctionLibrary::SplitBytes(
    const TArray<uint8>& ByteArray,
    const int32 LengthA,
    TArray<uint8>& A,
    TArray<uint8>& B)
{
    TSBC_ByteUtils::SplitBytes(ByteArray, LengthA, A, B);
}

TArray<uint8> UTSBC_ByteUtilsFunctionLibrary::MergeBytes(TArray<uint8> A, TArray<uint8> B)
{
    return TSBC_ByteUtils::MergeBytes(A, B);
}

TArray<uint8> UTSBC_ByteUtilsFunctionLibrary::BytesSubArray(
    const TArray<uint8>& ByteArray,
    const int32 Index,
    const int32 Length)
{
    return TSBC_ByteUtils::BytesSubArray(ByteArray, Index, Length);
}