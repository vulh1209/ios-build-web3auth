// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "TSBC_HashFunctionLibrary.generated.h"

/**
 * Hash Function Blueprint Library.
 *
 * Available hash functions:
 * - KECCAK-256
 * - RIPEMD-160
 * - SHA-256
 * - SHA-512
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_HashFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    const static FString CharSetDigits;
    const static FString CharSetHex;

public:
    UTSBC_HashFunctionLibrary();

    /**
     * Generates a Hash using KECCAK-256 algorithm from public key provided as a string (Text or Hex).
     * The result will be an Ethereum address without checksum applied.
     * 
     * @param Text The public key as String.
     * @param bIsHex Set to true, if the PublicKey is passed in hex format. Otherwise, PublicKey will be treated as text.
     * @returns Hash of the public key.
     */
    UFUNCTION(
        BlueprintPure,
        DisplayName="Generate Hash from String (KECCAK-256)",
        Category="3Studio|Cryptography|Hash Function")
    static FString Keccak256FromString(const FString& Text, const bool bIsHex);

    /**
     * Generates a hash using KECCAK-256 algorithm from public key provided as an Array of bytes.
     * The result will be an Ethereum address without checksum applied.
     * 
     * @param Bytes The public key as Array of Bytes.
     * @returns Hash of the public key.
     */
    UFUNCTION(
        BlueprintPure,
        DisplayName="Generate Hash from Byte Array (KECCAK-256)",
        Category="3Studio|Cryptography|Hash Function")
    static FString Keccak256FromBytes(const TArray<uint8>& Bytes);

    /**
     * Generates a hash using KECCAK-256 algorithm on Ethereum Address provided as a string in order to calculate the checksum.
     * The input String should be in text format only(Hex not allowed) inorder for the checksum algorithm be applied properly.
     * If the input string is not 20 Bytes long or contains non hex characters the return bool will be false and
     * the error message will be returned and the Checksum value will be empty.
     * 
     * @param KeccakHash The public key as String.
     * @param EthereumAddressWithChecksum Calculated checksum from provided Ethereum Address (non-checksummed).
     * @param ErrorMessage Error message returned if the input string is not 20 Bytes long or contains non hex characters
     * @returns True if Checksum is successfully generated.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Calculate Checksum from KECCAK-256 hash",
        Category="3Studio|Cryptography|Hash Function")
    static bool ChecksumFromKeccak256(
        const FString& KeccakHash,
        FString& EthereumAddressWithChecksum,
        FString& ErrorMessage);

    /**
     * Generates a hash using RIPEMD-160, always returns 20 bytes.
     *
     * @param ByteArray The bytes to hash.
     * @returns Hash (20 bytes)
     */
    UFUNCTION(
        BlueprintPure,
        DisplayName="Generate Hash (RIPEMD-160)",
        Category="3Studio|Cryptography|Hash Function")
    static TArray<uint8> Ripemd160(const TArray<uint8>& ByteArray);

    /**
     * Generates a SHA-256 hash, always returns 32 bytes.
     * 
     * @param ByteArray The bytes to hash.
     * @param Index Starting index.
     * @param Length Number of bytes.
     * @returns Hash (32 bytes)
     */
    UFUNCTION(
        BlueprintPure,
        DisplayName="Generate Hash (SHA-256)",
        Category="3Studio|Cryptography|Hash Function",
        Meta=(AdvancedDisplay = "1"))
    static TArray<uint8> Sha256(const TArray<uint8>& ByteArray, const int32 Index = 0, const int32 Length = 0x7FFFFFFF);

    /**
     * Generates a SHA-512 hash, always returns 64 bytes.
     * 
     * @param ByteArray The bytes to hash.
     * @param Index Starting index.
     * @param Length Number of bytes.
     * @returns Hash (64 bytes)
     */
    UFUNCTION(
        BlueprintPure,
        DisplayName="Generate Hash (SHA-512)",
        Category="3Studio|Cryptography|Hash Function",
        Meta=(AdvancedDisplay="1"))
    static TArray<uint8> Sha512(const TArray<uint8>& ByteArray, const int32 Index = 0, const int32 Length = 0x7FFFFFFF);
};