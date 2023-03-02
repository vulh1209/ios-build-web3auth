// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Crypto/Encryption/TSBC_EncryptionFunctionLibrary.h"

#include "Crypto/Encryption/TSBC_EcdsaSecp256k1.h"

bool UTSBC_EncryptionFunctionLibrary::Secp256k1_GeneratePrivateKey(TArray<uint8>& PrivateKey)
{
    return CTSBC_EcdsaSecp256k1::Secp256k1_GeneratePrivateKey(PrivateKey);
}

bool UTSBC_EncryptionFunctionLibrary::Secp256k1_GeneratePublicKey(
    const TArray<uint8>& PrivateKey,
    TArray<uint8>& PublicKey)
{
    return CTSBC_EcdsaSecp256k1::Secp256k1_GeneratePublicKey(
        PrivateKey,
        PublicKey);
}

bool UTSBC_EncryptionFunctionLibrary::Secp256k1_IsPublicKeyValid(const TArray<uint8>& PublicKey)
{
    return CTSBC_EcdsaSecp256k1::Secp256k1_IsPublicKeyValid(PublicKey);
}

bool UTSBC_EncryptionFunctionLibrary::Secp256k1_IsPrivateKeyValid(const TArray<uint8>& PrivateKey)
{
    return CTSBC_EcdsaSecp256k1::Secp256k1_IsPrivateKeyValid(PrivateKey);
}

bool UTSBC_EncryptionFunctionLibrary::Secp256k1_GenerateSharedSecret(
    const TArray<uint8>& PublicKey,
    const TArray<uint8>& PrivateKey,
    TArray<uint8>& Secret)
{
    return CTSBC_EcdsaSecp256k1::Secp256k1_GenerateSharedSecret(
        PublicKey,
        PrivateKey,
        Secret);
}

bool UTSBC_EncryptionFunctionLibrary::Secp256k1_CreateSignature(
    const TArray<uint8>& PrivateKey,
    const TArray<uint8>& Hash,
    TArray<uint8>& Signature)
{
    return CTSBC_EcdsaSecp256k1::Secp256k1_CreateSignature(
        PrivateKey,
        Hash,
        Signature);
}

bool UTSBC_EncryptionFunctionLibrary::Secp256k1_VerifySignature(
    const TArray<uint8>& PublicKey,
    const TArray<uint8>& Hash,
    const TArray<uint8>& Signature)
{
    return CTSBC_EcdsaSecp256k1::Secp256k1_VerifySignature(
        PublicKey,
        Hash,
        Signature);
}

bool UTSBC_EncryptionFunctionLibrary::Secp256k1_CompressPublicKey(
    const TArray<uint8>& PublicKey,
    TArray<uint8>& CompressedPublicKey)
{
    return CTSBC_EcdsaSecp256k1::Secp256k1_CompressPublicKey(
        PublicKey,
        CompressedPublicKey);
}

bool UTSBC_EncryptionFunctionLibrary::Secp256k1_DecompressPublicKey(
    const TArray<uint8>& CompressedPublicKey,
    TArray<uint8>& PublicKey)
{
    return CTSBC_EcdsaSecp256k1::Secp256k1_DecompressPublicKey(
        CompressedPublicKey,
        PublicKey);
}