// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "TSBC_EncryptionFunctionLibrary.generated.h"

/**
 * Encryption Blueprint Function Library.
 *
 * Available encryption functions:
 * - ECDSA secp256k1
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_EncryptionFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * Generates a private key for secp256k1.
     *
     * @param PrivateKey The generated private key.
     * @returns False on failure.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Generate Private Key (secp256k1)",
        Category="3Studio|Cryptography|Encryption|ECDSA",
        Meta=(Keywords="create"))
    static UPARAM(DisplayName="bSuccess") bool Secp256k1_GeneratePrivateKey(TArray<uint8>& PrivateKey);

    /**
     * Generates a public key for secp256k1.
     *
     * @param PrivateKey The private key to use for generating the public key.
     * @param PublicKey The generated public key.
     * @returns False on failure.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Generate Public Key (secp256k1)",
        Category="3Studio|Cryptography|Encryption|ECDSA",
        Meta=(Keywords="create"))
    static UPARAM(DisplayName="bSuccess") bool Secp256k1_GeneratePublicKey(
        const TArray<uint8>& PrivateKey,
        TArray<uint8>& PublicKey);

    /**
     * Checks if the given public key is valid for secp256k1.
     *
     * @param PublicKey The public key to validate.
     * @returns True if the given public key is valid for secp256k1.
     */
    UFUNCTION(
        BlueprintPure,
        DisplayName="Is Public Key Valid (secp256k1)",
        Category="3Studio|Cryptography|Encryption|ECDSA")
    static UPARAM(DisplayName="bIsValid") bool Secp256k1_IsPublicKeyValid(const TArray<uint8>& PublicKey);

    /**
     * Checks if the given private key is valid for secp256k1.
     * 
     * @param PrivateKey The private key to validate.
     * @returns True if the given private key is valid for secp256k1.
     */
    UFUNCTION(
        BlueprintPure,
        DisplayName="Is Private Key Valid (secp256k1)",
        Category="3Studio|Cryptography|Encryption|ECDSA")
    static UPARAM(DisplayName="bIsValid") bool Secp256k1_IsPrivateKeyValid(const TArray<uint8>& PrivateKey);

    /**
     * Generates shared secret for secp256k1.
     *
     * @param PublicKey The public key to use for generating the secret.
     * @param PrivateKey The private key to use for generating the secret.
     * @param Secret The generated secret.
     * @returns False on failure.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Generate Shared Secret (secp256k1)",
        Category="3Studio|Cryptography|Encryption|ECDSA",
        Meta=(Keywords="create"))
    static UPARAM(DisplayName="bSuccess") bool Secp256k1_GenerateSharedSecret(
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
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Create Signature (secp256k1)",
        Category="3Studio|Cryptography|Encryption|ECDSA",
        Meta=(Keywords="hash generate"))
    static UPARAM(DisplayName="bSuccess") bool Secp256k1_CreateSignature(
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
    UFUNCTION(
        BlueprintPure,
        DisplayName="Verify Signature (secp256k1)",
        Category="3Studio|Cryptography|Encryption|ECDSA",
        Meta=(Keywords="is validate correct"))
    static UPARAM(DisplayName="bIsValid") bool Secp256k1_VerifySignature(
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
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Compress Public Key (secp256k1)",
        Category="3Studio|Cryptography|Encryption|ECDSA")
    static UPARAM(DisplayName="bSuccess") bool Secp256k1_CompressPublicKey(
        const TArray<uint8>& PublicKey,
        TArray<uint8>& CompressedPublicKey);

    /**
     * Decompresses the public key.
     *
     * @param CompressedPublicKey The compressed public key.
     * @param PublicKey The uncompressed public key.
     * @returns False on failure. Also returns false when the decompressed public key isn't valid.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Decompress Public Key (secp256k1)",
        Category="3Studio|Cryptography|Encryption|ECDSA")
    static UPARAM(DisplayName="bSuccess") bool Secp256k1_DecompressPublicKey(
        const TArray<uint8>& CompressedPublicKey,
        TArray<uint8>& PublicKey);
};