// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once

/**
 * This class implements the SHA-512 hash function.
 */
class TSBC_PLUGIN_RUNTIME_API CTSBC_Sha512
{
public:
    const static int32 BLOCK_SIZE;
    const static int64 k[80];

private:
    TArray<int64> w;

    int64 h0 = 0x6a09e667f3bcc908L;
    int64 h1 = 0xbb67ae8584caa73bL;
    int64 h2 = 0x3c6ef372fe94f82bL;
    int64 h3 = 0xa54ff53a5f1d36f1L;
    int64 h4 = 0x510e527fade682d1L;
    int64 h5 = 0x9b05688c2b3e6c1fL;
    int64 h6 = 0x1f83d9abfb41bd6bL;
    int64 h7 = 0x5be0cd19137e2179L;

    int32 count = 0;
    TArray<uint8> buffer;

public:
    /**
     * Generates a SHA-512 hash, always returns 64 bytes.
     *
     * @param Bytes The bytes to hash.
     * @returns Hash (64 bytes)
     */
    static TArray<uint8> Hash(const TArray<uint8>& Bytes);

    /**
     * Generates a SHA-512 hash, always returns 64 bytes.
     *
     * @param Bytes The bytes to hash.
     * @param Index Starting index.
     * @param Length Number of bytes.
     * @returns Hash (64 bytes)
     */
    static TArray<uint8> Hash(const TArray<uint8>& Bytes, int32 Index, int32 Length);

private:
    void Initialize();
    void Update(const TArray<uint8>& Bytes, const int32 Offset, const int32 Len);
    TArray<uint8> Digest();
    TArray<uint8> PadBuffer() const;
    TArray<uint8> GetResult() const;
    void Sha(const TArray<uint8>& Input, int32 Offset);
    FORCEINLINE static int64 ShiftRight(const int64 A, const int32 B);
};