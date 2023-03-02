// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Crypto/Hash/TSBC_Sha512.h"

const int32 CTSBC_Sha512::BLOCK_SIZE = 128;

// ReSharper disable CppRedundantCastExpression
// ReSharper disable CppCStyleCast
// @formatter:off
const int64 CTSBC_Sha512::k[80] = {
    ((int64) 0x428a2f98d728ae22), ((int64) 0x7137449123ef65cd), ((int64) 0xb5c0fbcfec4d3b2f), ((int64) 0xe9b5dba58189dbbc),
    ((int64) 0x3956c25bf348b538), ((int64) 0x59f111f1b605d019), ((int64) 0x923f82a4af194f9b), ((int64) 0xab1c5ed5da6d8118),
    ((int64) 0xd807aa98a3030242), ((int64) 0x12835b0145706fbe), ((int64) 0x243185be4ee4b28c), ((int64) 0x550c7dc3d5ffb4e2),
    ((int64) 0x72be5d74f27b896f), ((int64) 0x80deb1fe3b1696b1), ((int64) 0x9bdc06a725c71235), ((int64) 0xc19bf174cf692694),
    ((int64) 0xe49b69c19ef14ad2), ((int64) 0xefbe4786384f25e3), ((int64) 0x0fc19dc68b8cd5b5), ((int64) 0x240ca1cc77ac9c65),
    ((int64) 0x2de92c6f592b0275), ((int64) 0x4a7484aa6ea6e483), ((int64) 0x5cb0a9dcbd41fbd4), ((int64) 0x76f988da831153b5),
    ((int64) 0x983e5152ee66dfab), ((int64) 0xa831c66d2db43210), ((int64) 0xb00327c898fb213f), ((int64) 0xbf597fc7beef0ee4),
    ((int64) 0xc6e00bf33da88fc2), ((int64) 0xd5a79147930aa725), ((int64) 0x06ca6351e003826f), ((int64) 0x142929670a0e6e70),
    ((int64) 0x27b70a8546d22ffc), ((int64) 0x2e1b21385c26c926), ((int64) 0x4d2c6dfc5ac42aed), ((int64) 0x53380d139d95b3df),
    ((int64) 0x650a73548baf63de), ((int64) 0x766a0abb3c77b2a8), ((int64) 0x81c2c92e47edaee6), ((int64) 0x92722c851482353b),
    ((int64) 0xa2bfe8a14cf10364), ((int64) 0xa81a664bbc423001), ((int64) 0xc24b8b70d0f89791), ((int64) 0xc76c51a30654be30),
    ((int64) 0xd192e819d6ef5218), ((int64) 0xd69906245565a910), ((int64) 0xf40e35855771202a), ((int64) 0x106aa07032bbd1b8),
    ((int64) 0x19a4c116b8d2d0c8), ((int64) 0x1e376c085141ab53), ((int64) 0x2748774cdf8eeb99), ((int64) 0x34b0bcb5e19b48a8),
    ((int64) 0x391c0cb3c5c95a63), ((int64) 0x4ed8aa4ae3418acb), ((int64) 0x5b9cca4f7763e373), ((int64) 0x682e6ff3d6b2b8a3),
    ((int64) 0x748f82ee5defb2fc), ((int64) 0x78a5636f43172f60), ((int64) 0x84c87814a1f0ab72), ((int64) 0x8cc702081a6439ec),
    ((int64) 0x90befffa23631e28), ((int64) 0xa4506cebde82bde9), ((int64) 0xbef9a3f7b2c67915), ((int64) 0xc67178f2e372532b),
    ((int64) 0xca273eceea26619c), ((int64) 0xd186b8c721c0c207), ((int64) 0xeada7dd6cde0eb1e), ((int64) 0xf57d4f7fee6ed178),
    ((int64) 0x06f067aa72176fba), ((int64) 0x0a637dc5a2c898a6), ((int64) 0x113f9804bef90dae), ((int64) 0x1b710b35131c471b),
    ((int64) 0x28db77f523047d84), ((int64) 0x32caab7b40c72493), ((int64) 0x3c9ebe0a15c9bebc), ((int64) 0x431d67c49c100d4c),
    ((int64) 0x4cc5d4becb3e42b6), ((int64) 0x597f299cfc657e2a), ((int64) 0x5fcb6fab3ad6faec), ((int64) 0x6c44198c4a475817)
};
// @formatter:on

void CTSBC_Sha512::Initialize()
{
    w.SetNum(80);
    buffer.SetNum(128);
}

void CTSBC_Sha512::Update(const TArray<uint8>& Bytes, const int32 Offset, const int32 Len)
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

TArray<uint8> CTSBC_Sha512::Digest()
{
    const TArray<uint8> Tail = PadBuffer();
    Update(Tail, 0, Tail.Num());

    return GetResult();
}

TArray<uint8> CTSBC_Sha512::PadBuffer() const
{
    const int32 n = count % BLOCK_SIZE;
    int32 Padding = n < 112 ? 112 - n : 240 - n;
    TArray<uint8> Result;
    Result.SetNum(Padding + 16);
    Result[0] = static_cast<uint8>(0x80);

    const uint64 Bits = count << 3;
    Padding += 8;
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

TArray<uint8> CTSBC_Sha512::GetResult() const
{
    TArray<uint8> Result;
    Result.SetNum(64);

    Result[0] = static_cast<uint8>(h0 >> 56);
    Result[1] = static_cast<uint8>(h0 >> 48);
    Result[2] = static_cast<uint8>(h0 >> 40);
    Result[3] = static_cast<uint8>(h0 >> 32);
    Result[4] = static_cast<uint8>(h0 >> 24);
    Result[5] = static_cast<uint8>(h0 >> 16);
    Result[6] = static_cast<uint8>(h0 >> 8);
    Result[7] = static_cast<uint8>(h0);

    Result[8] = static_cast<uint8>(h1 >> 56);
    Result[9] = static_cast<uint8>(h1 >> 48);
    Result[10] = static_cast<uint8>(h1 >> 40);
    Result[11] = static_cast<uint8>(h1 >> 32);
    Result[12] = static_cast<uint8>(h1 >> 24);
    Result[13] = static_cast<uint8>(h1 >> 16);
    Result[14] = static_cast<uint8>(h1 >> 8);
    Result[15] = static_cast<uint8>(h1);

    Result[16] = static_cast<uint8>(h2 >> 56);
    Result[17] = static_cast<uint8>(h2 >> 48);
    Result[18] = static_cast<uint8>(h2 >> 40);
    Result[19] = static_cast<uint8>(h2 >> 32);
    Result[20] = static_cast<uint8>(h2 >> 24);
    Result[21] = static_cast<uint8>(h2 >> 16);
    Result[22] = static_cast<uint8>(h2 >> 8);
    Result[23] = static_cast<uint8>(h2);

    Result[24] = static_cast<uint8>(h3 >> 56);
    Result[25] = static_cast<uint8>(h3 >> 48);
    Result[26] = static_cast<uint8>(h3 >> 40);
    Result[27] = static_cast<uint8>(h3 >> 32);
    Result[28] = static_cast<uint8>(h3 >> 24);
    Result[29] = static_cast<uint8>(h3 >> 16);
    Result[30] = static_cast<uint8>(h3 >> 8);
    Result[31] = static_cast<uint8>(h3);

    Result[32] = static_cast<uint8>(h4 >> 56);
    Result[33] = static_cast<uint8>(h4 >> 48);
    Result[34] = static_cast<uint8>(h4 >> 40);
    Result[35] = static_cast<uint8>(h4 >> 32);
    Result[36] = static_cast<uint8>(h4 >> 24);
    Result[37] = static_cast<uint8>(h4 >> 16);
    Result[38] = static_cast<uint8>(h4 >> 8);
    Result[39] = static_cast<uint8>(h4);

    Result[40] = static_cast<uint8>(h5 >> 56);
    Result[41] = static_cast<uint8>(h5 >> 48);
    Result[42] = static_cast<uint8>(h5 >> 40);
    Result[43] = static_cast<uint8>(h5 >> 32);
    Result[44] = static_cast<uint8>(h5 >> 24);
    Result[45] = static_cast<uint8>(h5 >> 16);
    Result[46] = static_cast<uint8>(h5 >> 8);
    Result[47] = static_cast<uint8>(h5);

    Result[48] = static_cast<uint8>(h6 >> 56);
    Result[49] = static_cast<uint8>(h6 >> 48);
    Result[50] = static_cast<uint8>(h6 >> 40);
    Result[51] = static_cast<uint8>(h6 >> 32);
    Result[52] = static_cast<uint8>(h6 >> 24);
    Result[53] = static_cast<uint8>(h6 >> 16);
    Result[54] = static_cast<uint8>(h6 >> 8);
    Result[55] = static_cast<uint8>(h6);

    Result[56] = static_cast<uint8>(h7 >> 56);
    Result[57] = static_cast<uint8>(h7 >> 48);
    Result[58] = static_cast<uint8>(h7 >> 40);
    Result[59] = static_cast<uint8>(h7 >> 32);
    Result[60] = static_cast<uint8>(h7 >> 24);
    Result[61] = static_cast<uint8>(h7 >> 16);
    Result[62] = static_cast<uint8>(h7 >> 8);
    Result[63] = static_cast<uint8>(h7);

    return Result;
}

void CTSBC_Sha512::Sha(const TArray<uint8>& Input, int32 Offset)
{
    int64 A = h0;
    int64 B = h1;
    int64 C = h2;
    int64 D = h3;
    int64 E = h4;
    int64 F = h5;
    int64 G = h6;
    int64 H = h7;
    uint64 T;
    uint64 T2;
    int32 r;

    for(r = 0; r < 16; r++)
    {
        w[r] = (static_cast<int64>(Input[Offset + 0]) << 56)
               | ((static_cast<int64>(Input[Offset + 1]) & 0xFF) << 48)
               | ((static_cast<int64>(Input[Offset + 2]) & 0xFF) << 40)
               | ((static_cast<int64>(Input[Offset + 3]) & 0xFF) << 32)
               | ((static_cast<int64>(Input[Offset + 4]) & 0xFF) << 24)
               | ((static_cast<int64>(Input[Offset + 5]) & 0xFF) << 16)
               | ((static_cast<int64>(Input[Offset + 6]) & 0xFF) << 8)
               | (static_cast<int64>(Input[Offset + 7]) & 0xFF);
        Offset += 8;
    }

    for(r = 16; r < 80; r++)
    {
        T = w[r - 2];
        T2 = w[r - 15];

        w[r] = ((ShiftRight(T, 19) | (T << 45)) ^ (ShiftRight(T, 61) | (T << 3)) ^ ShiftRight(T, 6))
               + w[r - 7]
               + ((ShiftRight(T2, 1) | (T2 << 63)) ^ (ShiftRight(T2, 8) | (T2 << 56)) ^ ShiftRight(T2, 7))
               + w[r - 16];
    }

    for(r = 0; r < 80; r++)
    {
        T = H + ((ShiftRight(E, 14) | (E << 50)) ^ (ShiftRight(E, 18) | (E << 46)) ^ (ShiftRight(E, 41) | (E << 23)))
            + ((E & F) ^ ((~E) & G))
            + k[r]
            + w[r];

        T2 = ((ShiftRight(A, 28) | (A << 36)) ^ (ShiftRight(A, 34) | (A << 30)) ^ (ShiftRight(A, 39) | (A << 25)))
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

int64 CTSBC_Sha512::ShiftRight(const int64 A, const int32 B)
{
    const uint64 a = A;
    const uint32 b = B;

    return a >> b;
}

TArray<uint8> CTSBC_Sha512::Hash(const TArray<uint8>& Bytes)
{
    CTSBC_Sha512 Instance = CTSBC_Sha512();
    Instance.Initialize();
    Instance.Update(Bytes, 0, Bytes.Num());

    return Instance.Digest();
}

TArray<uint8> CTSBC_Sha512::Hash(const TArray<uint8>& Bytes, int32 Index, int32 Length)
{
    CTSBC_Sha512 Instance = CTSBC_Sha512();
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