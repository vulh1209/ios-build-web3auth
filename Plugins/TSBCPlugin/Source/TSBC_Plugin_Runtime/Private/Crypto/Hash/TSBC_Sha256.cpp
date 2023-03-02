// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Crypto/Hash/TSBC_Sha256.h"

const int32 CTSBC_Sha256::BLOCK_SIZE = 64;

// ReSharper disable CppRedundantCastExpression
// ReSharper disable CppCStyleCast
// @formatter:off
const int32 CTSBC_Sha256::k[64] = {
    ((int32) 0x428a2f98), ((int32) 0x71374491),	((int32) 0xb5c0fbcf), ((int32) 0xe9b5dba5),
    ((int32) 0x3956c25b), ((int32) 0x59f111f1),	((int32) 0x923f82a4), ((int32) 0xab1c5ed5),
    ((int32) 0xd807aa98), ((int32) 0x12835b01),	((int32) 0x243185be), ((int32) 0x550c7dc3),
    ((int32) 0x72be5d74), ((int32) 0x80deb1fe),	((int32) 0x9bdc06a7), ((int32) 0xc19bf174),
    ((int32) 0xe49b69c1), ((int32) 0xefbe4786), ((int32) 0x0fc19dc6), ((int32) 0x240ca1cc),
    ((int32) 0x2de92c6f), ((int32) 0x4a7484aa), ((int32) 0x5cb0a9dc), ((int32) 0x76f988da),
    ((int32) 0x983e5152), ((int32) 0xa831c66d), ((int32) 0xb00327c8), ((int32) 0xbf597fc7),
    ((int32) 0xc6e00bf3), ((int32) 0xd5a79147), ((int32) 0x06ca6351), ((int32) 0x14292967),
    ((int32) 0x27b70a85), ((int32) 0x2e1b2138), ((int32) 0x4d2c6dfc), ((int32) 0x53380d13),
    ((int32) 0x650a7354), ((int32) 0x766a0abb), ((int32) 0x81c2c92e), ((int32) 0x92722c85),
    ((int32) 0xa2bfe8a1), ((int32) 0xa81a664b), ((int32) 0xc24b8b70), ((int32) 0xc76c51a3),
    ((int32) 0xd192e819), ((int32) 0xd6990624), ((int32) 0xf40e3585), ((int32) 0x106aa070),
    ((int32) 0x19a4c116), ((int32) 0x1e376c08), ((int32) 0x2748774c), ((int32) 0x34b0bcb5),
    ((int32) 0x391c0cb3), ((int32) 0x4ed8aa4a), ((int32) 0x5b9cca4f), ((int32) 0x682e6ff3),
    ((int32) 0x748f82ee), ((int32) 0x78a5636f), ((int32) 0x84c87814), ((int32) 0x8cc70208),
    ((int32) 0x90befffa), ((int32) 0xa4506ceb), ((int32) 0xbef9a3f7), ((int32) 0xc67178f2)
};
// @formatter:on

void CTSBC_Sha256::Initialize()
{
    w.SetNum(64);
    buffer.SetNum(64);
}

void CTSBC_Sha256::Update(const TArray<uint8>& Bytes, const int32 Offset, const int32 Len)
{
    int32 n = count % BLOCK_SIZE;
    count += Len;
    const int32 PartLen = BLOCK_SIZE - n;
    int32 i = 0;

    if(Len >= PartLen)
    {
        for(int32 Index = 0; Index < PartLen; Index++)
        {
            buffer[n + Index] = Bytes[Offset + Index];
        }

        Sha(buffer, 0);

        for(i = PartLen; ((i + BLOCK_SIZE) - 1) < Len; i += BLOCK_SIZE)
        {
            Sha(Bytes, Offset + i);
        }

        n = 0;
    }

    if(i < Len)
    {
        for(int32 Index = 0; Index < Len - i; Index++)
        {
            buffer[n + Index] = Bytes[Offset + i + Index];
        }
    }
}

TArray<uint8> CTSBC_Sha256::Digest()
{
    const TArray<uint8> Tail = PadBuffer();
    Update(Tail, 0, Tail.Num());

    return GetResult();
}

TArray<uint8> CTSBC_Sha256::PadBuffer() const
{
    const int32 n = count % BLOCK_SIZE;
    const int32 Padding = n < 56 ? 56 - n : 120 - n;
    TArray<uint8> Result;
    Result.SetNum(Padding + 8);
    Result[0] = static_cast<uint8>(0x80);

    const int64 Bits = count << 3;
    Result[Padding + 0] = static_cast<uint8>(Bits >> 56);
    Result[Padding + 1] = static_cast<uint8>(Bits >> 48);
    Result[Padding + 2] = static_cast<uint8>(Bits >> 40);
    Result[Padding + 3] = static_cast<uint8>(Bits >> 32);
    Result[Padding + 4] = static_cast<uint8>(Bits >> 24);
    Result[Padding + 5] = static_cast<uint8>(Bits >> 16);
    Result[Padding + 6] = static_cast<uint8>(Bits >> 8);
    Result[Padding + 7] = static_cast<uint8>(Bits);

    return Result;
}

TArray<uint8> CTSBC_Sha256::GetResult() const
{
    TArray<uint8> Result;
    Result.SetNum(32);

    Result[0] = static_cast<uint8>(h0 >> 24);
    Result[1] = static_cast<uint8>(h0 >> 16);
    Result[2] = static_cast<uint8>(h0 >> 8);
    Result[3] = static_cast<uint8>(h0);

    Result[4] = static_cast<uint8>(h1 >> 24);
    Result[5] = static_cast<uint8>(h1 >> 16);
    Result[6] = static_cast<uint8>(h1 >> 8);
    Result[7] = static_cast<uint8>(h1);

    Result[8] = static_cast<uint8>(h2 >> 24);
    Result[9] = static_cast<uint8>(h2 >> 16);
    Result[10] = static_cast<uint8>(h2 >> 8);
    Result[11] = static_cast<uint8>(h2);

    Result[12] = static_cast<uint8>(h3 >> 24);
    Result[13] = static_cast<uint8>(h3 >> 16);
    Result[14] = static_cast<uint8>(h3 >> 8);
    Result[15] = static_cast<uint8>(h3);

    Result[16] = static_cast<uint8>(h4 >> 24);
    Result[17] = static_cast<uint8>(h4 >> 16);
    Result[18] = static_cast<uint8>(h4 >> 8);
    Result[19] = static_cast<uint8>(h4);

    Result[20] = static_cast<uint8>(h5 >> 24);
    Result[21] = static_cast<uint8>(h5 >> 16);
    Result[22] = static_cast<uint8>(h5 >> 8);
    Result[23] = static_cast<uint8>(h5);

    Result[24] = static_cast<uint8>(h6 >> 24);
    Result[25] = static_cast<uint8>(h6 >> 16);
    Result[26] = static_cast<uint8>(h6 >> 8);
    Result[27] = static_cast<uint8>(h6);

    Result[28] = static_cast<uint8>(h7 >> 24);
    Result[29] = static_cast<uint8>(h7 >> 16);
    Result[30] = static_cast<uint8>(h7 >> 8);
    Result[31] = static_cast<uint8>(h7);

    return Result;
}

void CTSBC_Sha256::Sha(const TArray<uint8>& Input, int32 Offset)
{
    int32 A = h0;
    int32 B = h1;
    int32 C = h2;
    int32 D = h3;
    int32 E = h4;
    int32 F = h5;
    int32 G = h6;
    int32 H = h7;
    int32 T;
    int32 T2;
    int32 r;

    for(r = 0; r < 16; r++)
    {
		// @formatter:off
		w[r] = (Input[Offset + 0]         << 24)
			| ((Input[Offset + 1] & 0xFF) << 16)
			| ((Input[Offset + 2] & 0xFF) << 8)
			| ( Input[Offset + 3] & 0xFF);
		Offset += 4;
        // @formatter:on
    }

    for(r = 16; r < 64; r++)
    {
        T = w[r - 2];
        T2 = w[r - 15];

        w[r] = ((ShiftRight(T, 17) | (T << 15)) ^ (ShiftRight(T, 19) | (T << 13)) ^ ShiftRight(T, 10))
               + w[r - 7]
               + ((ShiftRight(T2, 7) | (T2 << 25)) ^ (ShiftRight(T2, 18) | (T2 << 14)) ^ ShiftRight(T2, 3))
               + w[r - 16];
    }

    for(r = 0; r < 64; r++)
    {
        T = H +
            ((ShiftRight(E, 6) | (E << 26)) ^ (ShiftRight(E, 11) | (E << 21)) ^ (ShiftRight(E, 25) | (E << 7)))
            + ((E & F) ^ (~E & G))
            + k[r]
            + w[r];

        T2 = ((ShiftRight(A, 2) | (A << 30)) ^ (ShiftRight(A, 13) | (A << 19)) ^ (ShiftRight(A, 22) | (A << 10)))
             + ((A & B) ^ (A & C) ^ (B & C));

        H = G;
        G = F;
        F = E;
        E = D + T;
        D = C;
        C = B;
        B = A;
        A = T + T2;
    }

    h0 += A;
    h1 += B;
    h2 += C;
    h3 += D;
    h4 += E;
    h5 += F;
    h6 += G;
    h7 += H;
}

int32 CTSBC_Sha256::ShiftRight(const int32 A, const int32 B)
{
    const uint32 a = A;
    const uint32 b = B;

    return a >> b;
}

TArray<uint8> CTSBC_Sha256::Hash(const TArray<uint8>& Bytes)
{
    CTSBC_Sha256 Instance = CTSBC_Sha256();
    Instance.Initialize();
    Instance.Update(Bytes, 0, Bytes.Num());

    return Instance.Digest();
}

TArray<uint8> CTSBC_Sha256::Hash(const TArray<uint8>& Bytes, int32 Index, int32 Length)
{
    CTSBC_Sha256 Instance = CTSBC_Sha256();
    Instance.Initialize();

    if(Bytes.Num() <= 0)
    {
        return Instance.Digest();
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
        return Instance.Digest();
    }

    Instance.Update(Bytes, Index, Length);

    return Instance.Digest();
}