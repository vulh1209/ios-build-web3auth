// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Crypto/Hash/TSBC_Ripemd160.h"

constexpr uint32 CTSBC_Ripemd160::KL[5] = {0x00000000, 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xA953FD4E};
constexpr uint32 CTSBC_Ripemd160::KR[5] = {0x50A28BE6, 0x5C4DD124, 0x6D703EF3, 0x7A6D76E9, 0x00000000};

// @formatter:off
constexpr int32 CTSBC_Ripemd160::RL[80] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
     7,  4, 13,  1, 10,  6, 15,  3, 12,  0,  9,  5,  2, 14, 11,  8,
     3, 10, 14,  4,  9, 15,  8,  1,  2,  7,  0,  6, 13, 11,  5, 12,
     1,  9, 11, 10,  0,  8, 12,  4, 13,  3,  7, 15, 14,  5,  6,  2,
     4,  0,  5,  9,  7, 12,  2, 10, 14,  1,  3,  8, 11,  6, 15, 13
};

constexpr int32 CTSBC_Ripemd160::RR[80] = {
     5, 14,  7,  0,  9,  2, 11,  4, 13,  6, 15,  8,  1, 10,  3, 12,
     6, 11,  3,  7,  0, 13,  5, 10, 14, 15,  8, 12,  4,  9,  1,  2,
    15,  5,  1,  3,  7, 14,  6,  9, 11,  8, 12,  2, 10,  0,  4, 13,
     8,  6,  4,  1,  3, 11, 15,  0,  5, 12,  2, 13,  9,  7, 10, 14,
    12, 15, 10,  4,  1,  5,  8,  7,  6,  2, 13, 14,  0,  3,  9, 11
};

constexpr int32 CTSBC_Ripemd160::SL[80] = {
    11, 14, 15, 12,  5,  8,  7,  9, 11, 13, 14, 15,  6,  7,  9,  8,
     7,  6,  8, 13, 11,  9,  7, 15,  7, 12, 15,  9, 11,  7, 13, 12,
    11, 13,  6,  7, 14,  9, 13, 15, 14,  8, 13,  6,  5, 12,  7,  5,
    11, 12, 14, 15, 14, 15,  9,  8,  9, 14,  5,  6,  8,  6,  5, 12,
     9, 15,  5, 11,  6,  8, 13, 12,  5, 12, 13, 14, 11,  8,  5,  6
};

constexpr int32 CTSBC_Ripemd160::SR[80] = {
     8,  9,  9, 11, 13, 15, 15,  5,  7,  7,  8, 11, 14, 14, 12,  6,
     9, 13, 15,  7, 12,  8,  9, 11,  7,  7, 12,  7,  6, 15, 13, 11,
     9,  7, 15, 11,  8,  6,  6, 14, 12, 13,  5, 14, 13, 13,  7,  5,
    15,  5,  8, 11, 14, 14,  6, 14,  6,  9, 12,  9, 12,  5, 15,  8,
     8,  5, 12,  9, 12,  5, 14,  6,  8, 13,  6,  5, 15, 13, 11, 11
};
// @formatter:on

CTSBC_Ripemd160::CTSBC_Ripemd160()
{
}

int32 CTSBC_Ripemd160::f(const int32 i, const uint32 x, const uint32 y, const uint32 z)
{
    switch(i >> 4)
    {
    case 0:
        return x ^ y ^ z;
    case 1:
        return (x & y) | (~x & z);
    case 2:
        return (x | ~y) ^ z;
    case 3:
        return (x & z) | (y & ~z);
    case 4:
        return x ^ (y | ~z);
    default:
        return 0;
    }
}

int32 CTSBC_Ripemd160::rotl32(const uint32 x, const int32 i)
{
    return ((0U + x) << i) | (x >> (32 - i));
}

void CTSBC_Ripemd160::c(TArray<uint32>& state, const TArray<uint8>& blocks, const uint32 len)
{
    //assert(len % BLOCK_LEN == 0);
    uint32 schedule[16];
    for(uint32 i = 0; i < len;)
    {
        for(int32 j = 0; j < 16; j++, i += 4)
        {
            schedule[j] = static_cast<uint32>(blocks[i + 0]) << 0
                          | static_cast<uint32>(blocks[i + 1]) << 8
                          | static_cast<uint32>(blocks[i + 2]) << 16
                          | static_cast<uint32>(blocks[i + 3]) << 24;
        }

        uint32 al = state[0], ar = state[0];
        uint32 bl = state[1], br = state[1];
        uint32 cl = state[2], cr = state[2];
        uint32 dl = state[3], dr = state[3];
        uint32 el = state[4], er = state[4];
        for(int32 j = 0; j < NUM_ROUNDS; j++)
        {
            uint32 temp;

            temp = 0U + rotl32(0U + al + f(j, bl, cl, dl) + schedule[RL[j]] + KL[j >> 4], SL[j]) + el;
            al = el;
            el = dl;
            dl = rotl32(cl, 10);
            cl = bl;
            bl = temp;

            temp = 0U + rotl32(0U + ar + f(NUM_ROUNDS - 1 - j, br, cr, dr) + schedule[RR[j]] + KR[j >> 4], SR[j]) + er;
            ar = er;
            er = dr;
            dr = rotl32(cr, 10);
            cr = br;
            br = temp;
        }

        const uint32 temp = 0U + state[1] + cl + dr;
        state[1] = 0U + state[2] + dl + er;
        state[2] = 0U + state[3] + el + ar;
        state[3] = 0U + state[4] + al + br;
        state[4] = 0U + state[0] + bl + cr;
        state[0] = temp;
    }
}

TArray<uint8> CTSBC_Ripemd160::Hash(const TArray<uint8>& ByteArray)
{
    //assert((msg != nullptr || len == 0) && hashResult != nullptr);
    TArray<uint32> state;
    state.SetNum(5);
    state[0] = 0x67452301;
    state[1] = 0xEFCDAB89;
    state[2] = 0x98BADCFE;
    state[3] = 0x10325476;
    state[4] = 0xC3D2E1F0;
    uint32 len = ByteArray.Num();
    uint32 off = len & ~static_cast<uint32>(BLOCK_LEN - 1);
    c(state, ByteArray, off);

    TArray<uint8> block;
    block.SetNum(BLOCK_LEN);
    for(uint32 i = 0; i < len - off; i++)
    {
        block[i] = ByteArray[off + i];
    }

    off = len & (BLOCK_LEN - 1);
    block[off] = 0x80;
    off++;

    if(off + 8 > BLOCK_LEN)
    {
        c(state, block, BLOCK_LEN);
        block = TArray<uint8>();
        block.SetNum(BLOCK_LEN);
    }

    block[BLOCK_LEN - 8] = static_cast<uint8>((len & 0x1FU) << 3);
    len >>= 5;
    for(uint32 i = 1; i < 8; i++, len >>= 8)
    {
        block[BLOCK_LEN - 8 + i] = static_cast<uint8>(len);
    }
    c(state, block, BLOCK_LEN);

    TArray<uint8> hashResult;
    hashResult.SetNum(HASH_LEN);
    for(uint32 i = 0; i < HASH_LEN; i++)
    {
        hashResult[i] = static_cast<uint8>(state[i >> 2] >> ((i & 3) << 3));
    }

    return hashResult;
}