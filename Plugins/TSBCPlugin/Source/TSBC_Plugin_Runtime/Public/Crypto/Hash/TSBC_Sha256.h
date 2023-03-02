// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once

/**
 * This class implements the SHA-256 hash function.
 */
class TSBC_PLUGIN_RUNTIME_API CTSBC_Sha256
{
public:
    const static int32 BLOCK_SIZE;
    const static int32 k[64];

private:
    TArray<int32> w;

    int32 h0 = 0x6a09e667;
    int32 h1 = 0xbb67ae85;
    int32 h2 = 0x3c6ef372;
    int32 h3 = 0xa54ff53a;
    int32 h4 = 0x510e527f;
    int32 h5 = 0x9b05688c;
    int32 h6 = 0x1f83d9ab;
    int32 h7 = 0x5be0cd19;

    int32 count = 0;
    TArray<uint8> buffer;

public:
    /**
     * Generates a SHA-256 hash, always returns 32 bytes.
     *
     * @param Bytes The bytes to hash.
     * @returns Hash (32 bytes)
     */
    static TArray<uint8> Hash(const TArray<uint8>& Bytes);

    /**
     * Generates a SHA-256 hash, always returns 32 bytes.
     *
     * @param Bytes The bytes to hash.
     * @param Index Starting index.
     * @param Length Number of bytes.
     * @returns Hash (32 bytes)
     */
    static TArray<uint8> Hash(const TArray<uint8>& Bytes, int32 Index, int32 Length);

private:
    void Initialize();
    void Update(const TArray<uint8>& Bytes, const int32 Offset, const int32 Len);
    TArray<uint8> Digest();
    TArray<uint8> PadBuffer() const;
    TArray<uint8> GetResult() const;
    void Sha(const TArray<uint8>& Input, int32 Offset);
    FORCEINLINE static int32 ShiftRight(const int32 A, const int32 B);
};