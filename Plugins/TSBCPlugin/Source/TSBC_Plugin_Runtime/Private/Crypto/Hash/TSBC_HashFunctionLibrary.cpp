// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Crypto/Hash/TSBC_HashFunctionLibrary.h"

#include "Crypto/Hash/TSBC_Keccak256.h"
#include "Crypto/Hash/TSBC_Ripemd160.h"
#include "Crypto/Hash/TSBC_Sha256.h"
#include "Crypto/Hash/TSBC_Sha512.h"

const FString UTSBC_HashFunctionLibrary::CharSetDigits = ("0123456789");
const FString UTSBC_HashFunctionLibrary::CharSetHex = "abcdef";

UTSBC_HashFunctionLibrary::UTSBC_HashFunctionLibrary()
{
}

FString UTSBC_HashFunctionLibrary::Keccak256FromString(const FString& Text, const bool bIsHex)
{
    CTSBC_Keccak256 HashInstance = CTSBC_Keccak256();
    FString HashValue = HashInstance.KeccakFromString(Text, bIsHex);

    return HashValue;
}

FString UTSBC_HashFunctionLibrary::Keccak256FromBytes(const TArray<uint8>& Bytes)
{
    CTSBC_Keccak256 HashInstance = CTSBC_Keccak256();
    FString HashValue = HashInstance.KeccakFromBytes(Bytes);

    return HashValue;
}

bool UTSBC_HashFunctionLibrary::ChecksumFromKeccak256(
    const FString& KeccakHash,
    FString& EthereumAddressWithChecksum,
    FString& ErrorMessage)
{
    // If string length is not 40 Bytes, abort function and return false with error message in the returned string.
    if(KeccakHash.Len() != 40)
    {
        ErrorMessage = "Input hash is not 40 Bytes long, make sure to provide ethereum address without \"0x\" prefix";
        return false;
    }

    // Make sure all characters in the hash are lower cased.
    const FString KeccakHashLowercase = KeccakHash.ToLower();

    // Apply keccak256 algorithm on the ethereum address(without 0x prefix).
    const FString ChecksumReference = Keccak256FromString(KeccakHashLowercase, false);

    // Check if the KeccakHashLowercase doesn't contain any non hex character.
    FString CalculatedChecksum = "";
    for(int i = 0; i < KeccakHashLowercase.Len(); i++)
    {
        const char CurrentCharacter = KeccakHashLowercase[i];

        int32 CharFoundAtIndex;
        if(CharSetDigits.FindChar(CurrentCharacter, CharFoundAtIndex))
        {
            CalculatedChecksum.AppendChar(CurrentCharacter);
            continue;
        }

        if(CharSetHex.FindChar(CurrentCharacter, CharFoundAtIndex))
        {
            /**
             * If the current character is in the CharSetHex so its decimal value
             * should be between 97 and 102. 
             * Check if at the current index the CurrentCharacter and the ChecksumReference both contain a character
             * from CharSetHex(abcdef), if true make the character in CalculatedChecksum uppercased.
             */
            const int32 NumericValueOfHexDigit = CharSetHex[5] /* 'f' */ - ChecksumReference[i];
            if(NumericValueOfHexDigit < 7)
            {
                CalculatedChecksum.AppendChar(toupper(CurrentCharacter));
            }
            else
            {
                CalculatedChecksum.AppendChar(CurrentCharacter);
            }
        }
        else
        {
            ErrorMessage = "Non Hex character found";
            return false;
        }
    }

    EthereumAddressWithChecksum = CalculatedChecksum;

    return true;
}

TArray<uint8> UTSBC_HashFunctionLibrary::Ripemd160(const TArray<uint8>& ByteArray)
{
    return CTSBC_Ripemd160::Hash(ByteArray);
}

TArray<uint8> UTSBC_HashFunctionLibrary::Sha256(
    const TArray<uint8>& ByteArray,
    const int32 Index,
    const int32 Length)
{
    return CTSBC_Sha256::Hash(ByteArray, Index, Length);
}

TArray<uint8> UTSBC_HashFunctionLibrary::Sha512(
    const TArray<uint8>& ByteArray,
    const int32 Index,
    const int32 Length)
{
    return CTSBC_Sha512::Hash(ByteArray, Index, Length);
}