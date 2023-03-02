// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "DataAssets/TSBC_ContractAbiDataAsset.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "TSBC_EncodingFunctionLibrary.generated.h"

/**
 * Encoding Blueprint Function Library.
 *
 * Available encodings:
 * - Base58
 * - Ethereum-based Smart Contract ABI
 */
UCLASS()
class TSBC_PLUGIN_RUNTIME_API UTSBC_EncodingFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UTSBC_EncodingFunctionLibrary();

    /**
     * Converts a byte array into a Base58 string.
     * 
     * @param ByteArray The byte array to convert.
     * @returns A converted byte array into a Base58 string.
     */
    UFUNCTION(
        BlueprintPure,
        Category = "3Studio|Encoding|Base58",
        Meta = (DisplayName = "Bytes to Base58", Keywords = "byte array"))
    static FString EncodeBase58(const TArray<uint8>& ByteArray);

    /**
     * Tries to convert a Base58 string into a byte array.
     *
     * @param Base58 The Base58-encoded string to convert.
     * @returns An empty array on failure.
     */
    UFUNCTION(
        BlueprintPure,
        Category = "3Studio|Encoding|Base58",
        Meta = (DisplayName = "Base58 to Bytes", Keywords = "byte array"))
    static TArray<uint8> DecodeBase58(const FString& Base58);

    /**
     * Tries to parse an Ethereum-based Smart Contract ABI from the given JSON.
     * 
     * @param bSuccess True if the operation is successful.
     * @param ErrorMessage Contains an error message in case the operation fails. Otherwise, it will be empty.
     * @param ContractAbiJson The Ethereum Smart Contract ABI in JSON format to parse.
     * @param ContractAbi The Contract ABI.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Parse ABI from JSON",
        Category="Atherlabs|Blockchain|Ethereum|ABI")
    static void ParseAbiFromJson(
        bool& bSuccess,
        FString& ErrorMessage,
        const FString& ContractAbiJson,
        FTSBC_ContractAbi& ContractAbi);

    /**
     * Tries to parse an Ethereum-based Smart Contract ABI from the given JSON.
     * 
     * @param bSuccess True if the operation is successful.
     * @param ErrorMessage Contains an error message in case the operation fails. Otherwise, it will be empty.
     * @param ContractAbiJson The Ethereum Smart Contract ABI in JSON format to parse.
     * @param ContractAbi The Contract ABI.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Parse ABI from JSON",
        BlueprintInternalUseOnly)
    static void ParseAbiFromJson(
        bool& bSuccess,
        FString& ErrorMessage,
        const FString& ContractAbiJson,
        FTSBC_ContractAbi& ContractAbi);

    /**
     * Generates a "Function Selector" with argument encoding for a specific function of an Ethereum Smart Contract.
     *
     * @param bSuccess True if the operation is successful.
     * @param ErrorMessage Contains an error message in case the operation fails. Otherwise, it will be empty.
     * @param ContractAbiDataAsset The Contract ABI Data Asset.
     * @param FunctionName The function name to hash.
     * @param FunctionArguments Function arguments to encode.
     * @param FunctionHashAndEncodedArguments The "Function Selector" with encoded arguments.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Encode ABI",
        BlueprintInternalUseOnly)
    static void EncodeAbi(
        bool& bSuccess,
        FString& ErrorMessage,
        const UTSBC_ContractAbiDataAsset* ContractAbiDataAsset,
        const FString& FunctionName,
        const TArray<FTSBC_SolidityValueList>& FunctionArguments,
        FString& FunctionHashAndEncodedArguments);

    /**
     * Decodes the resulting data returned from an Ethereum RPC-call.
     * 
     * @param bSuccess True if the operation is successful.
     * @param ErrorMessage Contains an error message in case the operation fails. Otherwise, it will be empty.
     * @param ContractAbiDataAsset The Contract ABI Data Asset.
     * @param FunctionName The function name that was used to call the Ethereum Smart Contract function which returned
     *                     the data that should now be decoded here.
     * @param DataToDecode The data to decode.
     * @param DecodedAbiValues Array of the decoded values from the data.
     */
    UFUNCTION(
        BlueprintCallable,
        DisplayName="Decode ABI",
        BlueprintInternalUseOnly)
    static void DecodeAbi(
        bool& bSuccess,
        FString& ErrorMessage,
        const UTSBC_ContractAbiDataAsset* ContractAbiDataAsset,
        const FString& FunctionName,
        const FString& DataToDecode,
        TArray<FTSBC_SolidityValueList>& DecodedAbiValues);
};