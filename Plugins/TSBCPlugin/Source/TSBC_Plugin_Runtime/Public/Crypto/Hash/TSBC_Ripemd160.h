// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once

/**
 * This class implements the RIPEMD-160 hash function.
 */
class TSBC_PLUGIN_RUNTIME_API CTSBC_Ripemd160
{
public:
    constexpr static int32 HASH_LEN = 20;
    constexpr static int32 BLOCK_LEN = 64;
    constexpr static int32 NUM_ROUNDS = 80;

    const static uint32 KL[5];
    const static uint32 KR[5];
    const static int32 RL[80];
    const static int32 RR[80];
    const static int32 SL[80];
    const static int32 SR[80];

public:
    CTSBC_Ripemd160();

    /**
     * Hashes the given bytes with Ripemd160.
     *
     * The hash is always 20 bytes long.
     *
     * @param ByteArray The bytes to hash
     * @returns Hash (20 bytes)
     */
    static TArray<uint8> Hash(const TArray<uint8>& ByteArray);

private:
    static int32 f(const int32 i, const uint32 x, const uint32 y, const uint32 z);
    static int32 rotl32(const uint32 x, const int32 i);
    static void c(TArray<uint32>& state, const TArray<uint8>& blocks, const uint32 len);
};