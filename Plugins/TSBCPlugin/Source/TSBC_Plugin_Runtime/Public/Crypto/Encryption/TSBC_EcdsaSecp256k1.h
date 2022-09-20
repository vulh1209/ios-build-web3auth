// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once

/**
 * This class implements ECDSA using curve secp256k1.
 */
class TSBC_PLUGIN_RUNTIME_API CTSBC_EcdsaSecp256k1
{
public:
    /**
     * Number of retries for generating a random value.
     */
    constexpr static int32 uECC_RNG_MAX_TRIES = 64;

private:
    struct uECC_Curve
    {
        int32 num_words;
        int32 num_bytes;
        int32 num_n_bits;
        uint32 p[8];
        uint32 n[8];
        uint32 nhalf[8];
        uint32 G[8 * 2];
        uint32 b[8];
        void (*double_jacobian)(uint32* X1, uint32* Y1, uint32* Z1, const uECC_Curve* curve);
        void (*mod_sqrt)(uint32* a, const uECC_Curve* curve);
        void (*x_side)(uint32* result, const uint32* x, const uECC_Curve* curve);
        void (*mmod_fast)(uint32* result, uint32* product);
    };

    struct uECC_HashContext
    {
        void (*init_hash)(const uECC_HashContext* context);
        void (*update_hash)(const uECC_HashContext* context, const uint8* message, unsigned message_size);
        void (*finish_hash)(const uECC_HashContext* context, uint8* hash_result);
        unsigned block_size;  /* Hash function block size in bytes, eg 64 for SHA-256. */
        unsigned result_size; /* Hash function result size in bytes, eg 32 for SHA-256. */
        uint8* tmp;           /* Must point to a buffer of at least (2 * result_size + block_size) bytes. */
    };

    const static uECC_Curve curve_secp256k1;

public:
    /**
     * Generates a private key for secp256k1.
     *
     * @param PrivateKey The generated private key.
     * @returns False on failure.
     */
    static bool Secp256k1_GeneratePrivateKey(TArray<uint8>& PrivateKey);

    /**
     * Generates a public key for secp256k1.
     *
     * @param PrivateKey The private key to use for generating the public key.
     * @param PublicKey The generated public key.
     * @returns False on failure.
     */
    static bool Secp256k1_GeneratePublicKey(
        const TArray<uint8>& PrivateKey,
        TArray<uint8>& PublicKey);

    /**
     * Checks if the given public key is valid for secp256k1.
     *
     * @param PublicKey The public key to validate.
     * @returns True if the given public key is valid for secp256k1.
     */
    static bool Secp256k1_IsPublicKeyValid(const TArray<uint8>& PublicKey);

    /**
     * Checks if the given private key is valid for secp256k1.
     * 
     * @param PrivateKey The private key to validate.
     * @returns True if the given private key is valid for secp256k1.
     */
    static bool Secp256k1_IsPrivateKeyValid(const TArray<uint8>& PrivateKey);

    /**
     * Generates shared secret for secp256k1.
     *
     * @param PublicKey The public key to use for generating the secret.
     * @param PrivateKey The private key to use for generating the secret.
     * @param Secret The generated secret.
     * @returns False on failure.
     */
    static bool Secp256k1_GenerateSharedSecret(
        const TArray<uint8>& PublicKey,
        const TArray<uint8>& PrivateKey,
        TArray<uint8>& Secret);

    /**
     * Signs the given hash with the private key using secp256k1, returns a signature.
     *
     * Requires the given hash to be at least 16 bytes in length. Too short and it will fail. Too long and the
     * overflowing bytes will be cut off.
     *
     * @param PrivateKey The private key to use for signing.
     * @param Hash The hash sign.
     * @param Signature The created signature.
     * @returns False on failure.
     */
    static bool Secp256k1_CreateSignature(
        const TArray<uint8>& PrivateKey,
        const TArray<uint8>& Hash,
        TArray<uint8>& Signature);

    /**
     * Signs the given hash deterministically with the private key using secp256k1, returns a signature.
     *
     * Requires the given hash to be at least 16 bytes in length. Too short and it will fail. Too long and the
     * overflowing bytes will be cut off.
     *
     * @param PrivateKey The private key to use for signing.
     * @param Hash The hash to sign.
     * @param Signature The created signature.
     * @returns False on failure.
     */
    static bool Secp256k1_CreateSignatureDeterministic(
        const TArray<uint8>& PrivateKey,
        const TArray<uint8>& Hash,
        TArray<uint8>& Signature);

    /**
     * Checks if the signature of a signed hash with secp256k1 is valid.
     * 
     * @param PublicKey The public key to verify.
     * @param Hash The signed hash.
     * @param Signature The signature.
     * @returns True if the signature of a signed hash with secp256k1 is valid.
     */
    static bool Secp256k1_VerifySignature(
        const TArray<uint8>& PublicKey,
        const TArray<uint8>& Hash,
        const TArray<uint8>& Signature);

    /**
     * Compresses the public key.
     *
     * @param PublicKey The uncompressed public key.
     * @param CompressedPublicKey The compressed public key.
     * @returns False on failure.
     */
    static bool Secp256k1_CompressPublicKey(
        const TArray<uint8>& PublicKey,
        TArray<uint8>& CompressedPublicKey);

    /**
     * Decompresses the public key.
     *
     * @param CompressedPublicKey The compressed public key.
     * @param PublicKey The uncompressed public key.
     * @returns False on failure. Also returns false when the decompressed public key isn't valid.
     */
    static bool Secp256k1_DecompressPublicKey(
        const TArray<uint8>& CompressedPublicKey,
        TArray<uint8>& PublicKey);

    /**
     * Gets the V-value from the signature according to the given Blockchain ID.
     *
     * @param Signature The signature.
     * @param ChainId The Blockchain ID.
     * @returns The V-value from the Signature.
     */
    static FString GetFromSignatureValueV(const TArray<uint8>& Signature, const uint32 ChainId);

    /**
     * Gets the R-value from the signature.
     * 
     * @param Signature The signature.
     * @returns The R-value on the curve from the Signature.
     */
    static FString GetFromSignatureValueR(const TArray<uint8>& Signature);

    /**
     * Gets the S-value from the signature.
     * 
     * @param Signature The signature.
     * @returns The S-value on the curve from the Signature.
     */
    static FString GetFromSignatureValueS(const TArray<uint8>& Signature);

    /**
     * Gets the Y-parity from a given point on the elliptic curve.
     *
     * @param Point Point on the elliptic curve.
     * @returns The Y-Parity value.
     */
    static uint32 GetYParity(const uint32* Point);

private:
    static bool IsPublicKeyValid(const TArray<uint8>& PublicKey, const uECC_Curve* curve);
    static bool IsPrivateKeyValid(const TArray<uint8>& PrivateKey, const uECC_Curve* curve);
    static bool IsOddParity(const uint32* Bytes);

    static const uECC_Curve* uECC_secp256k1();
    static int32 BITS_TO_WORDS(const int32 num_bits);
    static int32 BITS_TO_BYTES(const int32 num_bits);
    static void mod_sqrt_default(uint32* a, const uECC_Curve* curve);
    static void double_jacobian_secp256k1(uint32* X1, uint32* Y1, uint32* Z1, const uECC_Curve* curve);
    static void x_side_secp256k1(uint32* result, const uint32* x, const uECC_Curve* curve);
    static void vli_mmod_fast_secp256k1(uint32* result, uint32* product);
    static void omega_mult_secp256k1(uint32* result, const uint32* right);
    static int32 uECC_curve_private_key_size(const uECC_Curve* curve);
    static int32 uECC_curve_public_key_size(const uECC_Curve* curve);
    static void uECC_vli_clear(uint32* vli, const int32 num_words);
    static bool uECC_vli_isZero(const uint32* vli, const int32 num_words);
    static bool uECC_vli_testBit(const uint32* vli, int32 bit);
    static int32 vli_numDigits(const uint32* vli, const int32 max_words);
    static int32 uECC_vli_numBits(const uint32* vli, const int32 max_words);
    static void uECC_vli_set(uint32* dest, const uint32* src, const int32 num_words);
    static int32 uECC_vli_cmp_unsafe(const uint32* left, const uint32* right, const int32 num_words);
    static bool uECC_vli_equal(const uint32* left, const uint32* right, const int32 num_words);
    static int32 uECC_vli_cmp(const uint32* left, const uint32* right, const int32 num_words);
    static void uECC_vli_rshift1(uint32* vli, const int32 num_words);
    static bool uECC_vli_add(uint32* result, const uint32* left, const uint32* right, const int32 num_words);
    static bool uECC_vli_sub(uint32* result, const uint32* left, const uint32* right, const int32 num_words);
    static void muladd(const uint32 a, const uint32 b, uint32* r0, uint32* r1, uint32* r2);
    static void uECC_vli_mult(uint32* result, const uint32* left, const uint32* right, const int32 num_words);
    static void uECC_vli_modAdd(
        uint32* result,
        const uint32* left,
        const uint32* right,
        const uint32* mod,
        const int32 num_words);
    static void uECC_vli_modSub(
        uint32* result,
        const uint32* left,
        const uint32* right,
        const uint32* mod,
        const int32 num_words);
    static void uECC_vli_mmod(uint32* result, uint32* product, const uint32* mod, const int32 num_words);
    static void uECC_vli_modMult(
        uint32* result,
        const uint32* left,
        const uint32* right,
        const uint32* mod,
        const int32 num_words);
    static void uECC_vli_modMult_fast(uint32* result, const uint32* left, const uint32* right, const uECC_Curve* curve);
    static void uECC_vli_modSquare_fast(uint32* result, const uint32* left, const uECC_Curve* curve);
    static void vli_modInv_update(uint32* uv, const uint32* mod, const int32 num_words);
    static void uECC_vli_modInv(uint32* result, const uint32* input, const uint32* mod, const int32 num_words);
    static bool EccPoint_isZero(const uint32* point, const uECC_Curve* curve);
    static void apply_z(uint32* X1, uint32* Y1, const uint32* const Z, const uECC_Curve* curve);
    static void XYcZ_initial_double(
        uint32* X1,
        uint32* Y1,
        uint32* X2,
        uint32* Y2,
        const uint32* const initial_Z,
        const uECC_Curve* curve);
    static void XYcZ_add(uint32* X1, uint32* Y1, uint32* X2, uint32* Y2, const uECC_Curve* curve);
    static void XYcZ_addC(uint32* X1, uint32* Y1, uint32* X2, uint32* Y2, const uECC_Curve* curve);
    static int32 EccPoint_mult(
        uint32* result,
        const uint32* point,
        const uint32* scalar,
        const uint32* initial_Z,
        const int32 num_bits,
        const uECC_Curve* curve);
    static bool regularize_k(const uint32* const k, uint32* k0, uint32* k1, const uECC_Curve* curve);
    static bool EccPoint_compute_public_key(uint32* result, const uint32* private_key, const uECC_Curve* curve);
    static void uECC_vli_nativeToBytes(uint8* bytes, const int32 num_bytes, const uint32* native);
    static void uECC_vli_bytesToNative(uint32* native, const uint8* bytes, const int32 num_bytes);
    static bool uECC_generate_random_int(uint32* random, const uint32* top, const int32 num_words);
    static bool uECC_make_key(uint8* public_key, uint8* private_key, const uECC_Curve* curve);
    static bool uECC_shared_secret(
        const uint8* public_key,
        const uint8* private_key,
        uint8* secret,
        const uECC_Curve* curve);
    static void uECC_compress(const uint8* public_key, uint8* compressed, const uECC_Curve* curve);
    static void uECC_decompress(const uint8* compressed, uint8* public_key, const uECC_Curve* curve);
    static bool uECC_valid_point(const uint32* point, const uECC_Curve* curve);
    static bool uECC_valid_public_key(const uint8* public_key, const uECC_Curve* curve);
    static bool uECC_compute_public_key(const uint8* private_key, uint8* public_key, const uECC_Curve* curve);
    static void bits2int(uint32* native, const uint8* bits, uint32 bits_size, const uECC_Curve* curve);
    static bool uECC_sign_with_k(
        const uint8* private_key,
        const uint8* message_hash,
        const uint32 hash_size,
        uint32* k,
        uint8* signature,
        const uECC_Curve* curve);
    static bool uECC_sign(
        const uint8* private_key,
        const uint8* message_hash,
        const uint32 hash_size,
        uint8* signature,
        const uECC_Curve* curve);
    static void HMAC_init(const uECC_HashContext* hash_context, const uint8* K);
    static void HMAC_update(const uECC_HashContext* hash_context, const uint8* message, const uint32 message_size);
    static void HMAC_finish(const uECC_HashContext* hash_context, const uint8* K, uint8* result);
    static void update_V(const uECC_HashContext* hash_context, uint8* K, uint8* V);
    static bool uECC_sign_deterministic(
        const uint8* private_key,
        const uint8* message_hash,
        const uint32 hash_size,
        const uECC_HashContext* hash_context,
        uint8* signature,
        const uECC_Curve* curve);
    static bool uECC_verify(
        const uint8* public_key,
        const uint8* message_hash,
        const uint32 hash_size,
        const uint8* signature,
        const uECC_Curve* curve);

    static uECC_HashContext GetHashContextSha256(TArray<uint8>& Buffer);
    static uECC_HashContext GetHashContextSha512(TArray<uint8>& Buffer);
};