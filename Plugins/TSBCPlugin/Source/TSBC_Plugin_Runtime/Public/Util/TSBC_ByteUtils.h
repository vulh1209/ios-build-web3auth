// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once

namespace TSBC_ByteUtils
{
    /**
     * @returns True if the two arrays contain the same data.
     */
    static bool AreBytesEqual(
        const TArray<uint8>& A,
        const TArray<uint8>& B,
        int32 IndexA = 0,
        int32 LengthA = 0x7FFFFFFF,
        int32 IndexB = 0,
        int32 LengthB = 0x7FFFFFFF)
    {
        if(IndexA < 0)
        {
            LengthA += IndexA;
            IndexA = 0;
        }

        if(LengthA > A.Num() - IndexA)
        {
            LengthA = A.Num() - IndexA;
        }

        if(LengthA < 0)
        {
            LengthA = 0;
        }

        if(IndexB < 0)
        {
            LengthB += IndexB;
            IndexB = 0;
        }

        if(LengthB > B.Num() - IndexB)
        {
            LengthB = B.Num() - IndexB;
        }

        if(LengthB < 0)
        {
            LengthB = 0;
        }

        if(LengthA != LengthB)
        {
            return false;
        }

        for(int32 i = 0; i < LengthA; i++)
        {
            if(A[IndexA + i] != B[IndexB + i])
            {
                return false;
            }
        }

        return true;
    }

    /**
     * Splits the given Byte Array into two Byte Arrays.
     */
    static void SplitBytes(const TArray<uint8>& ByteArray, const int32 LengthA, TArray<uint8>& A, TArray<uint8>& B)
    {
        if(LengthA <= 0)
        {
            A = TArray<uint8>();
            B = ByteArray;
            return;
        }

        if(LengthA >= ByteArray.Num())
        {
            A = ByteArray;
            B = TArray<uint8>();
            return;
        }

        A = TArray<uint8>();
        A.Append(ByteArray.GetData(), LengthA);

        B = TArray<uint8>();
        B.Append(ByteArray.GetData() + LengthA, ByteArray.Num() - LengthA);
    }

    /**
     * Merges multiple Byte Arrays into one.
     */
    static TArray<uint8> MergeBytes(TArray<uint8> A, TArray<uint8> B)
    {
        if(A.Num() <= 0)
        {
            return B;
        }

        if(B.Num() <= 0)
        {
            return A;
        }

        TArray<uint8> ReturnArray = TArray<uint8>();
        ReturnArray.Reserve(A.Num() + B.Num());
        ReturnArray.Append(A.GetData(), A.Num());
        ReturnArray.Append(B.GetData(), B.Num());

        return ReturnArray;
    }

    /**
     * Returns the values of the given bytes of the given index and length.
     */
    static TArray<uint8> BytesSubArray(const TArray<uint8>& ByteArray, int32 Index, int32 Length = 0x7FFFFFFF)
    {
        if(ByteArray.Num() <= 0)
        {
            return TArray<uint8>();
        }

        if(Index < 0)
        {
            Length += Index;
            Index = 0;
        }

        if(Length > ByteArray.Num() - Index)
        {
            Length = ByteArray.Num() - Index;
        }

        if(Length <= 0)
        {
            return TArray<uint8>();
        }

        if((Index == 0) && (Length == ByteArray.Num()))
        {
            return ByteArray;
        }

        TArray<uint8> ReturnArray = TArray<uint8>();
        ReturnArray.Append(ByteArray.GetData() + Index, Length);

        return ReturnArray;
    }
}