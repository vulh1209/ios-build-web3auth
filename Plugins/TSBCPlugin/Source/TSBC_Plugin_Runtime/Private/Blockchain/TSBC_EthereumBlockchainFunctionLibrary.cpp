// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Blockchain/TSBC_EthereumBlockchainFunctionLibrary.h"

#include "Crypto/Hash/TSBC_HashFunctionLibrary.h"
#include "Crypto/Hash/TSBC_Keccak256.h"
#include "Crypto/Encryption/TSBC_EcdsaSecp256k1.h"
#include "JsonRpc/Eth/TSBC_EthGetBalance.h"
#include "Util/TSBC_StringUtils.h"
#include "Util/TSBC_ByteUtils.h"

FString UTSBC_EthereumBlockchainFunctionLibrary::BlockIdentifierFromEnum(const ETSBC_EthBlockIdentifier BlockIdentifier)
{
    switch(BlockIdentifier)
    {
    case ETSBC_EthBlockIdentifier::Latest:
        {
            return "latest";
        }

    case ETSBC_EthBlockIdentifier::Earliest:
        {
            return "earliest";
        }

    case ETSBC_EthBlockIdentifier::Pending:
        {
            return "pending";
        }

    default:
        {
            checkNoEntry();
            return "";
        }
    }
}

FString UTSBC_EthereumBlockchainFunctionLibrary::EthereumUnitFromEnum(const ETSBC_EthereumUnit Unit)
{
    switch(Unit)
    {
    case ETSBC_EthereumUnit::Wei:
        {
            return "Wei";
        }

    case ETSBC_EthereumUnit::Kwei:
        {
            return "Kwei";
        }

    case ETSBC_EthereumUnit::Mwei:
        {
            return "Mwei";
        }

    case ETSBC_EthereumUnit::Gwei:
        {
            return "Gwei";
        }

    case ETSBC_EthereumUnit::Szabo:
        {
            return "Szabo";
        }

    case ETSBC_EthereumUnit::Finney:
        {
            return "Finney";
        }

    case ETSBC_EthereumUnit::Ether:
        {
            return "Ether";
        }

    case ETSBC_EthereumUnit::Kether:
        {
            return "Kether";
        }

    case ETSBC_EthereumUnit::Mether:
        {
            return "Mether";
        }

    case ETSBC_EthereumUnit::Gether:
        {
            return "Gether";
        }

    case ETSBC_EthereumUnit::Tether:
        {
            return "Tether";
        }

    default:
        {
            checkNoEntry();
            return "";
        }
    }
}

uint32 UTSBC_EthereumBlockchainFunctionLibrary::GetExponentToConvertFromWei(const ETSBC_EthereumUnit Unit)
{
    switch(Unit)
    {
    case ETSBC_EthereumUnit::Wei:
        {
            return 0;
        }

    case ETSBC_EthereumUnit::Kwei:
        {
            return 3;
        }

    case ETSBC_EthereumUnit::Mwei:
        {
            return 6;
        }

    case ETSBC_EthereumUnit::Gwei:
        {
            return 9;
        }

    case ETSBC_EthereumUnit::Szabo:
        {
            return 12;
        }

    case ETSBC_EthereumUnit::Finney:
        {
            return 15;
        }

    case ETSBC_EthereumUnit::Ether:
        {
            return 18;
        }

    case ETSBC_EthereumUnit::Kether:
        {
            return 21;
        }

    case ETSBC_EthereumUnit::Mether:
        {
            return 24;
        }

    case ETSBC_EthereumUnit::Gether:
        {
            return 27;
        }

    case ETSBC_EthereumUnit::Tether:
        {
            return 30;
        }

    default:
        {
            checkNoEntry();
            return 1;
        }
    }
}

void UTSBC_EthereumBlockchainFunctionLibrary::MakeEthValueFromString(
    const FString& InValue,
    const ETSBC_EthereumUnit Unit,
    bool& bSuccess,
    FTSBC_uint256& OutValue)
{
    bSuccess = OutValue.ParseFromString(InValue);
    if(bSuccess)
    {
        const uint32 Exponent = GetExponentToConvertFromWei(Unit);
        if(Exponent > 0)
        {
            const FTSBC_uint256 Multiplicand = FTSBC_uint256::Pow(10, Exponent);
            OutValue *= Multiplicand;
        }
    }
}

void UTSBC_EthereumBlockchainFunctionLibrary::MakeEthValueStringWithFractionalValue(
    const FString& InValue,
    const ETSBC_EthereumUnit Unit,
    bool& bSuccess,
    FTSBC_uint256& OutValue)
{
    const uint32 Exponent = GetExponentToConvertFromWei(Unit);
    bSuccess = OutValue.ParseFromStringWithFractionalValue(InValue, Exponent);
}

FString UTSBC_EthereumBlockchainFunctionLibrary::EthValueToDec(
    const FTSBC_uint256& Value,
    const ETSBC_EthereumUnit ToUnit,
    int32 MinIntDigits,
    int32 MinFracDigits,
    int32 MaxFracDigits)
{
    const int32 Exponent = GetExponentToConvertFromWei(ToUnit);
    const FString ValueAsString = Value.ToDecString();

    if(MinIntDigits < 0)
    {
        MinIntDigits = 0;
    }
    if(MinFracDigits < 0)
    {
        MinFracDigits = 0;
    }
    if(MaxFracDigits < 0)
    {
        MaxFracDigits = 0;
    }

    if(Exponent <= 0)
    {
        FString Integral;
        const int32 MissingZerosInIntegralPart = MinIntDigits - ValueAsString.Len();
        if(MissingZerosInIntegralPart > 0)
        {
            Integral = FString::ChrN(MissingZerosInIntegralPart, '0');
        }
        Integral += ValueAsString;
        if(MinFracDigits > 0)
        {
            Integral += FString(".") + FString::ChrN(MinFracDigits, '0');
        }
        return Integral;
    }

    FString FinalValue;
    if(ValueAsString.Len() <= Exponent)
    {
        const int32 MissingZerosInFractionalPart = Exponent - ValueAsString.Len();
        FString Fractional = FString::ChrN(MissingZerosInFractionalPart, '0') + ValueAsString;
        Fractional.MidInline(0, MaxFracDigits);

        if(Fractional.EndsWith("0"))
        {
            // Find number of zeroes that need to be stripped
            int32 Index = Fractional.Len() - 1;
            while(Index >= 0 && Fractional[Index] == '0')
            {
                Index--;
            }
            // Strip all zeroes in one operation
            Fractional.MidInline(0, Index + 1);
        }

        if(MinFracDigits > Fractional.Len())
        {
            Fractional += FString::ChrN(MinFracDigits - Fractional.Len(), '0');
        }

        FinalValue = FString::ChrN(MinIntDigits, '0');

        if(Fractional.Len() > MaxFracDigits)
        {
            Fractional.MidInline(0, MaxFracDigits);
        }

        if(Fractional.Len() > 0)
        {
            FinalValue += FString(".") + Fractional;
        }

        return FinalValue;
    }

    const int32 IntegralPartLength = ValueAsString.Len() - Exponent;
    const FString IntegralPart = ValueAsString.Mid(0, IntegralPartLength);
    FString FractionalPart = ValueAsString.Mid(IntegralPartLength, ValueAsString.Len());

    if(FractionalPart.EndsWith("0"))
    {
        // Find number of zeroes that need to be stripped
        int32 Index = FractionalPart.Len() - 1;
        while(Index >= 0 && FractionalPart[Index] == '0')
        {
            Index--;
        }
        // Strip all zeroes in one operation
        FractionalPart.MidInline(0, Index + 1);
    }

    if(FractionalPart.Len() > MaxFracDigits)
    {
        FractionalPart.MidInline(0, MaxFracDigits);
    }

    FinalValue = IntegralPart;

    if(MinFracDigits >= FractionalPart.Len() || FractionalPart.Len() > 0)
    {
        const int32 MissingMinFractional = MinFracDigits - FractionalPart.Len();
        FractionalPart += FString::ChrN(MissingMinFractional < 0 ? 0 : MissingMinFractional, '0');
        if(FractionalPart.Len() > 0)
        {
            FinalValue += FString(".") + FractionalPart;
        }
    }

    return FinalValue;
}

void UTSBC_EthereumBlockchainFunctionLibrary::GetBlockchainConfig(
    const ETSBC_EthereumNetwork EthereumNetwork,
    FTSBC_BlockchainConfig& BlockchainConfig)
{
    switch(EthereumNetwork)
    {
    case ETSBC_EthereumNetwork::PolygonMainNet:
        {
            BlockchainConfig.RpcUrl = "https://polygon-rpc.com/";
            BlockchainConfig.NetworkName = "Polygon (MainNet)";
            BlockchainConfig.ChainId = 137;
            BlockchainConfig.Symbol = "MATIC";
        }
        break;
    case ETSBC_EthereumNetwork::EthMainNet:
        {
            BlockchainConfig.RpcUrl = "https://rpc.ankr.com/eth";
            BlockchainConfig.NetworkName = "Ethereum (MainNet)";
            BlockchainConfig.ChainId = 1;
            BlockchainConfig.Symbol = "ETH";
        }
        break;
    case ETSBC_EthereumNetwork::EthRopstenTestNet:
        {
            BlockchainConfig.RpcUrl = "https://rpc.ankr.com/eth_ropsten";
            BlockchainConfig.NetworkName = "Ethereum Ropsten (TestNet)";
            BlockchainConfig.ChainId = 3;
            BlockchainConfig.Symbol = "ETH";
        }
        break;
    case ETSBC_EthereumNetwork::EthRinkebyTestNet:
        {
            BlockchainConfig.RpcUrl = "https://rpc.ankr.com/eth_rinkeby";
            BlockchainConfig.NetworkName = "Ethereum Rinkeby (TestNet)";
            BlockchainConfig.ChainId = 4;
            BlockchainConfig.Symbol = "ETH";
        }
        break;
    default:
        checkNoEntry();
        break;
    }
}


bool UTSBC_EthereumBlockchainFunctionLibrary::GenerateAddressFromPublicKeyAsString(
    const FString& PublicKey,
    FString& EthereumAddress,
    FString& ErrorMessage)
{
    /**
     * If string length is not 64 Bytes, abort function and return false with error message in the returned string.
     * Each byte is 2 Hex digits so the PublicKey string should contain (2*64)128 Characters.
     */
    if(PublicKey.Len() != 128)
    {
        ErrorMessage = "PublicKey is not 64 Bytes long";
        return false;
    }

    const FString PublicKeyLowercase = PublicKey.ToLower();

    // Check if the public key contains any non Hex characters.
    for(int i = 0; i < PublicKeyLowercase.Len(); i++)
    {
        const char CurrentCharacter = PublicKeyLowercase[i];

        int32 CharFoundAtIndex;
        if(UTSBC_HashFunctionLibrary::CharSetDigits.FindChar(CurrentCharacter, CharFoundAtIndex))
        {
            continue;
        }

        if(UTSBC_HashFunctionLibrary::CharSetHex.FindChar(CurrentCharacter, CharFoundAtIndex))
        {
            continue;
        }

        ErrorMessage = "Non Hex character found";
        return false;
    }
    /**
     * Try creating the Ethereum address from public key, if creation was successful the address will be returned
     * with success true, otherwise the error will be returned instead of the Ethereum address with false state.
     */

    // Create hash from the public key which should be in Hex.
    const FString KeccakHash = CTSBC_Keccak256().KeccakFromString(PublicKey, true);

    // The address is the last 20 Bytes(40 Hex) of the keccak hash.
    const FString Last20Bytes = KeccakHash.RightChop(24);

    // Calculate the checksum of the ethereum address and return the address with prefix 0x if successful calculated.
    FString AddressWithChecksum;
    const bool bChecksumSuccess = UTSBC_HashFunctionLibrary::ChecksumFromKeccak256(
        Last20Bytes,
        AddressWithChecksum,
        ErrorMessage);
    if(!bChecksumSuccess)
    {
        return false;
    }

    EthereumAddress = FString("0x").Append(AddressWithChecksum);

    return true;
}

bool UTSBC_EthereumBlockchainFunctionLibrary::GenerateAddressFromPublicKeyAsBytes(
    const TArray<uint8>& PublicKey,
    FString& EthereumAddress,
    FString& ErrorMessage)
{
    // If string length is not 64 Bytes, abort function and return false with error message in the returned string.
    if(PublicKey.Num() != 64)
    {
        ErrorMessage = "PublicKey is not 64 Bytes long";
        return false;
    }

    // Convert the array of bytes to hex characters.
    const FString HexValue = BytesToHex(&PublicKey[0], PublicKey.Num());

    const FString PublicKeyLowercase = HexValue.ToLower();

    // Check if the public key contains any non Hex characters.
    for(int i = 0; i < PublicKeyLowercase.Len(); i++)
    {
        const char CurrentCharacter = PublicKeyLowercase[i];

        int32 CharFoundAtIndex;
        if(UTSBC_HashFunctionLibrary::CharSetDigits.FindChar(CurrentCharacter, CharFoundAtIndex))
        {
            continue;
        }

        if(UTSBC_HashFunctionLibrary::CharSetHex.FindChar(CurrentCharacter, CharFoundAtIndex))
        {
            continue;
        }

        ErrorMessage = "Non Hex character found";
        return false;
    }

    /**
     * Try creating the Ethereum address from public key, if creation was successful the address will be returned
     * with success true, otherwise the error will be returned instead of the Ethereum address with false state.
     */


    // Create hash from the public key.
    const FString KeccakHash = CTSBC_Keccak256().KeccakFromBytes(PublicKey);

    // The address is the last 20 Bytes(40 Hex) of the keccak hash.
    const FString Last20Bytes = KeccakHash.RightChop(24);

    // Calculate the checksum of the ethereum address and return the address with prefix 0x if successful calculated.
    FString AddressWithChecksum;
    if(!UTSBC_HashFunctionLibrary::ChecksumFromKeccak256(Last20Bytes, AddressWithChecksum, ErrorMessage))
    {
        return false;
    }

    EthereumAddress = FString("0x").Append(AddressWithChecksum);

    return true;
}

bool UTSBC_EthereumBlockchainFunctionLibrary::IsValidEthereumAddress(const FString& Address)
{
    return TSBC_StringUtils::RegexMatch(Address, "^0x[a-fA-F0-9]{40}$");
}

FString UTSBC_EthereumBlockchainFunctionLibrary::GenerateSignatureJSONMessage(
    const FString& PrivateKey,
    const FString& JSONMessage)
{
    //bytes of prefix '\x19Ethereum Signed Message:\n'
    TArray<uint8> PrefixAsBytes = {
        25,  69, 116, 104, 101, 114, 101,
        117, 109,  32,  83, 105, 103, 110,
        101, 100,  32,  77, 101, 115, 115,
        97, 103, 101,  58,  10
    }; 
    TArray<uint8> JSONMessageAsBytes = TSBC_StringUtils::StringToBytesUtf8(JSONMessage);
    TArray<uint8> MsgLenAsBytes = TSBC_StringUtils::StringToBytesUtf8(FString::FromInt(JSONMessage.Len()));
    TArray<uint8> MergePrefixAndLenAsBytes = TSBC_ByteUtils::MergeBytes(PrefixAsBytes,MsgLenAsBytes);
    TArray<uint8> MessageToHashAsBytes = TSBC_ByteUtils::MergeBytes(MergePrefixAndLenAsBytes,JSONMessageAsBytes);
    TArray<uint8> PrivateKeyAsBytes = TSBC_StringUtils::HexStringToBytes(PrivateKey);
    FString KeccakHash = CTSBC_Keccak256().KeccakFromBytes(MessageToHashAsBytes);
    TArray<uint8> HashAsBytes = TSBC_StringUtils::HexStringToBytes(KeccakHash);
    // Sign Message Hash
    TArray<uint8> Signature;
    bool bSignatureCalculated = false;
    bSignatureCalculated = CTSBC_EcdsaSecp256k1::Secp256k1_CreateSignature(
        PrivateKeyAsBytes,
        HashAsBytes,
        Signature);
    if(!bSignatureCalculated)
        return "0x";
        
    return TSBC_StringUtils::BytesToHex(Signature,true);
}

bool UTSBC_EthereumBlockchainFunctionLibrary::VerifySignatureJSONMessage(
    const FString& PublicKey,
    const FString& JSONMessage,
    const FString& Signature)
{
    FString KeccakHash = CTSBC_Keccak256().KeccakFromString(JSONMessage,false);
    TArray<uint8> HashAsBytes = TSBC_StringUtils::HexStringToBytes(KeccakHash);
    TArray<uint8> PublicKeyAsBytes = TSBC_StringUtils::HexStringToBytes(PublicKey);
    TArray<uint8> SignatureAsBytes = TSBC_StringUtils::HexStringToBytes(Signature);
    return CTSBC_EcdsaSecp256k1::Secp256k1_VerifySignature(
        PublicKeyAsBytes,
        HashAsBytes,
        SignatureAsBytes);
}