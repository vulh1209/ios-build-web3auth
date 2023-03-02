// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/TSBC_Types.h"
#include "Math/TSBC_uint256.h"

#include "TSBC_EthereumBlockchainFunctionLibrary.generated.h"

/**
 * Ethereum Blockchain Blueprint Function Library.
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_EthereumBlockchainFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * Gets configuration data for a specific Ethereum network needed to send RPC calls to.
     * 
     * @param EthereumNetwork The Ethereum network for which to get its configuration data.
     * @param BlockchainConfig Configuration data for a specific Ethereum network.
     */
    UFUNCTION(
        BlueprintPure,
        DisplayName="Get Blockchain configuration",
        Category="3Studio|Blockchain|Ethereum")
    static void GetBlockchainConfig(
        const ETSBC_EthereumNetwork EthereumNetwork,
        FTSBC_BlockchainConfig& BlockchainConfig);

    /**
     * Generates Ethereum Address using keccak-256 algorithm from public key provided as a string.
     * If the public key is not 64 Bytes (128 characters) long or contains non hex characters
     * the return bool will be false if failed to Generate Ethereum Address and an error message will be returned.
     * 
     * @param PublicKey The public key.
     * @param EthereumAddress returns false if the string provided isn't 40 Bytes or includes non Hex characters.
     * @param ErrorMessage returns false if the string provided isn't 40 Bytes or includes non Hex characters.
     * @returns True if EthereumAddress is successfully generated.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName = "Generate Ethereum Address from Public Key (String)",
        Category = "3Studio|Blockchain|Ethereum")
    static UPARAM(DisplayName="bSuccess") bool GenerateAddressFromPublicKeyAsString(
        const FString& PublicKey,
        FString& EthereumAddress,
        FString& ErrorMessage);

    /**
     * Generates Ethereum Address using keccak-256 algorithm from public key provided as an array of bytes.
     * If the public key is not 64 Bytes (128 characters) long or contains non hex characters
     * the return bool will be false if failed to Generate Ethereum Address and an error message will be returned.
     *  
     * @param PublicKey The public key.
     * @param EthereumAddress returns false if the string provided isn't 40 Bytes or includes non Hex characters.
     * @param ErrorMessage returns false if the string provided isn't 40 Bytes or includes non Hex characters.
     * @returns True if EthereumAddress is successfully generated.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName = "Generate Ethereum Address from Public Key (Array of bytes)",
        Category = "3Studio|Blockchain|Ethereum")
    static UPARAM(DisplayName="bSuccess") bool GenerateAddressFromPublicKeyAsBytes(
        const TArray<uint8>& PublicKey,
        FString& EthereumAddress,
        FString& ErrorMessage);

    /**
     * Validates an Ethereum address.
     * 
     * @param Address Input address to be validated.
     * @return True, if the input address is a valid Ethereum address.
     */
    UFUNCTION(
        BlueprintPure,
        DisplayName="Is Valid Ethereum Address",
        Category="3Studio|Blockchain|Ethereum")
    static UPARAM(DisplayName="bIsValid") bool IsValidEthereumAddress(const FString& Address);

    /**
     * Tries to parse the input string value to an Ethereum value stored in an uint256 data type.
     *
     * If the string value is prefixed with "0x" it will be interpreted as a hex value, otherwise as a decimal value.
     *
     * @param InValue The input value as string. Can be either in hex (prefixed with "0x") or decimal representation.
     * @param Unit Describes in what unit the string should be interpreted as.
     * @param bSuccess True, if the input string value could be parsed.
     * @param OutValue The parsed uint256 value.
     */
    UFUNCTION(
        BlueprintPure,
        DisplayName="Make Eth Value from String",
        Category="3Studio|Blockchain|Ethereum",
        Meta=(Keywords="parse"))
    static void MakeEthValueFromString(
        UPARAM(DisplayName="Value") const FString& InValue,
        const ETSBC_EthereumUnit Unit,
        bool& bSuccess,
        UPARAM(DisplayName="Value") FTSBC_uint256& OutValue);

    /**
     * Tries to parse the input string to an Ethereum value stored in an uint256 data type.
     * 
     * This function can handle fractional values as inputs but will eventually truncate the fractional part
     * after applying the unit conversion operation.
     *
     * The input string must be a decimal value (i.e. no hex string inputs allowed).
     *
     * @param InValue The input value as string. Must be a decimal value.
     * @param Unit Describes in what unit the string should be interpreted as.
     * @param bSuccess True, if the input string value could be parsed.
     * @param OutValue The parsed uint256 value.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Make Eth Value from String with Fractional Value",
        Category="3Studio|Blockchain|Ethereum",
        Meta=(Keywords="parse"))
    static void MakeEthValueStringWithFractionalValue(
        UPARAM(DisplayName="Value") const FString& InValue,
        const ETSBC_EthereumUnit Unit,
        bool& bSuccess,
        UPARAM(DisplayName="Value") FTSBC_uint256& OutValue);

    /**
     * Converts uint256 to String in decimal notation.
     *
     * @param Value The value to be converted.
     * @param ToUnit Describes in what unit the string should be interpreted as.
     * @param MinIntDigits Minimum number of integral digits for the returned value.
     *                     Digits will be zero-padded if value uses less digits than the minimum specified. (Default: 1)
     * @param MinFracDigits Minimum number of fractional digits for the returned value.
     *                      Digits will be zero-padded if value uses less digits than the minimum specified. (Default: 0)
     * @param MaxFracDigits Maximum number of fractional digits for the returned value.
     *                      Truncates fractional part to specified number of digits. (Default: 30)
     * @returns Value in decimal notation.
     */
    UFUNCTION(
        BlueprintPure,
        Category="3Studio|Blockchain|Ethereum",
        Meta=(AdvancedDisplay="MinIntDigits,MinFracDigits,MaxFracDigits"))
    static FString EthValueToDec(
        UPARAM(Ref) const FTSBC_uint256& Value,
        const ETSBC_EthereumUnit ToUnit = ETSBC_EthereumUnit::Wei,
        int32 MinIntDigits = 1,
        int32 MinFracDigits = 0,
        int32 MaxFracDigits = 30);

    /**
     * Gets the string representation of the given block identifier.
     *
     * @param BlockIdentifier The block number.
     * @returns The string representation of the given block identifier. 
     */
    static FString BlockIdentifierFromEnum(const ETSBC_EthBlockIdentifier BlockIdentifier);

    /**
     * Gets the string representation of the given Ethereum unit.
     *
     * @param Unit The Ethereum unit.
     * @returns The string representation of the given Ethereum unit. 
     */
    static FString EthereumUnitFromEnum(const ETSBC_EthereumUnit Unit);

    /**
     * Gets the exponent value for converting from Wei to the given Ethereum unit.
     *
     * @param Unit The Ethereum unit.
     * @returns The string representation of the given Ethereum unit. 
     */
    static uint32 GetExponentToConvertFromWei(const ETSBC_EthereumUnit Unit);

    /**
     * genarate signature hex string from private key hex string and json message string
     *
     * @param PrivateKey The private key hex string.
     * @param JSONMessage The stringtify of JSON message.
     * @returns The hex string of signature.
     */
    UFUNCTION(BlueprintPure, DisplayName = "Sign JSON Stringtify", Category = "Atherlabs|Blockchain|Ethereum")
    static FString GenerateSignatureJSONMessage(const FString& PrivateKey, const FString& JSONMessage);
    /**
     * verify signature hex string and json message string from publickey or not
     *
     * @param PublicKey The public key hex string.
     * @param JSONMessage The stringtify of JSON message.
     * @param Signature The signature hex string.
     * @returns bool verify signature from publickey or not.
     */
    UFUNCTION(BlueprintPure, DisplayName = "Verify Signature JSON", Category = "Atherlabs|Blockchain|Ethereum")
    static bool VerifySignatureJSONMessage(const FString& PublicKey, const FString& JSONMessage, const FString& Signature);
};